#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int shmid = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0600);
    if (shmid < 0) { perror("shmget"); return 1; }


    int *shared = (int*)shmat(shmid, NULL, 0);
    if (shared == (void*)-1) { perror("shmat"); return 1; }
    shared[0] = 3;  
    shared[1] = 0;   


    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }
    if (pid == 0) {
        char buf[32];
        snprintf(buf, sizeof buf, "%d", shmid);
        execl("./q4_p2", "q4_p2", buf, (char*)NULL);
        perror("execl");
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

    int status = 0;
    if (waitpid(pid, &status, 0) == -1) perror("waitpid");

    if (shmdt(shared) == -1) perror("shmdt");
    if (shmctl(shmid, IPC_RMID, NULL) == -1) perror("shmctl IPC_RMID");

    printf("P1: done (P2 status=%d).\n", status);
    return 0;
}
