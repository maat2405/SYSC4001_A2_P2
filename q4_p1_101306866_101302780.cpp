//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780
#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int shmid;  //ID for the shared memory segment
    int *shared;  //pointer to shared ints; [0]=multiple, [1]=counter
    pid_t childpid;

    // Create shared memory of two ints
    shmid = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0600);
    if (shmid < 0) {
        perror("shmget error");
        exit(1);
    }

    // Attach it in this process
    shared = (int*)shmat(shmid, NULL, 0);
    if (shared == (void*)-1) {
        perror("shmat error");
        exit(1);
    }
    //Initilaze
    shared[0] = 3;  //multiple value
    shared[1] = 0;  //counter value

    setvbuf(stdout, NULL, _IONBF, 0); //no buffering

    childpid = fork(); //fork a child that will exec program 2
    if (childpid < 0) {
        perror("fork failed");
        shmdt(shared);                  //detach an error
        shmctl(shmid, IPC_RMID, NULL);  //delete a segment
        exit(1); 
    }

    if (childpid == 0) { //child: exec and pass shmid as argv
        char buf[32]; 
        snprintf(buf, sizeof buf, "%d", shmid);
        execl("./q4_p2", "q4_p2", buf, (char*)NULL);
        perror("execl failed"); //only run if exec fials
        _exit(127);
    }
      // Parent loop: red, print, then increment shared counter until > 500
    while (1) {
        int v = shared[1];
        int m = shared[0];

        if (v % m == 0)
            printf("P1: counter=%d (multiple of %d)\n", v, m);
        else
            printf("P1: counter=%d\n", v);

        shared[1] = v + 1; //update counter

        if (shared[1] > 500) break; //stop condition
        sleep(1); //sleep slows down the output by one second
    }

    int status;
    waitpid(childpid, &status, 0); //wait for the child process to exit
    shmdt(shared); //detach
    shmctl(shmid, IPC_RMID, NULL); //remove the shared segment

    printf("P1: done (P2 exited with status=%d)\n", status);
    return 0;
}
