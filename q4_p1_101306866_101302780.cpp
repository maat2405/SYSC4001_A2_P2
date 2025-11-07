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
    int shmid;
    int *shared;  
    pid_t childpid;

    shmid = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0600);
    if (shmid < 0) {
        perror("shmget error");
        exit(1);
    }


    shared = (int*)shmat(shmid, NULL, 0);
    if (shared == (void*)-1) {
        perror("shmat error");
        exit(1);
    }

    shared[0] = 3;  
    shared[1] = 0;  

    setvbuf(stdout, NULL, _IONBF, 0); 

    childpid = fork();
    if (childpid < 0) {
        perror("fork failed");
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    if (childpid == 0) {
        char buf[32];
        snprintf(buf, sizeof buf, "%d", shmid);
        execl("./q4_p2", "q4_p2", buf, (char*)NULL);
        perror("execl failed");
        _exit(127);
    }

    while (1) {
        int v = shared[1];
        int m = shared[0];

        if (v % m == 0)
            printf("P1: counter=%d (multiple of %d)\n", v, m);
        else
            printf("P1: counter=%d\n", v);

        shared[1] = v + 1;

        if (shared[1] > 500) break;
        sleep(1);
    }

    int status;
    waitpid(childpid, &status, 0);
    shmdt(shared);
    shmctl(shmid, IPC_RMID, NULL);

    printf("P1: done (P2 exited with status=%d)\n", status);
    return 0;
}
