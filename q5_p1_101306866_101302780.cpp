//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780
#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

union semun { int val; struct semid_ds *buf; unsigned short *array; };

static int sem_wait_op(int semid) {         
    struct sembuf op = {0, -1, 0};
    return semop(semid, &op, 1);
}
static int sem_signal_op(int semid) { 
    struct sembuf op = {0, +1, 0};
    return semop(semid, &op, 1);
}

int main(void) {
    int shmid, semid;
    int *shared;      // shared[0]=multiple, shared[1]=counter      
    pid_t childpid;

    // Make shared memory, 2 ints
    shmid = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0600);
    if (shmid < 0) { perror("shmget error"); exit(1); }

    //attaches it
    shared = (int*)shmat(shmid, NULL, 0);
    if (shared == (void*)-1) { perror("shmat error"); exit(1); }

    // Make a 1-semaphore set (mutex), init to 1 → 
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (semid < 0) { perror("semget"); exit(1); }
    union semun arg; arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) < 0) { perror("semctl SETVAL"); exit(1); }

    setvbuf(stdout, NULL, _IONBF, 0);

    if (sem_wait_op(semid) == -1) perror("sem_wait_op");
    shared[0] = 3;   //multiple
    shared[1] = 0;  //counter
    if (sem_signal_op(semid) == -1) perror("sem_signal_op");

    // Fork child and pass shmid & semid via exec
    childpid = fork();
    if (childpid < 0) {
        perror("fork failed");
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
        exit(1);
    }
    if (childpid == 0) {
        char shm_buf[32], sem_buf[32];
        snprintf(shm_buf, sizeof shm_buf, "%d", shmid);
        snprintf(sem_buf, sizeof sem_buf, "%d", semid);
        execl("./q5_p2", "q5_p2", shm_buf, sem_buf, (char*)NULL);
        perror("execl failed");
        _exit(127);
    }

    // Parent loop: lock, read/print/update, unlock, until counter > 500
    while (1) {
        int v, m, done;

        if (sem_wait_op(semid) == -1) { perror("sem_wait_op"); break; }
        v = shared[1];
        m = shared[0];

        if (v % m == 0) printf("P1: counter=%d (multiple of %d)\n", v, m);
        else            printf("P1: counter=%d\n", v);

        shared[1] = v + 1;
        done = (shared[1] > 500);
        if (sem_signal_op(semid) == -1) { perror("sem_signal_op"); break; }

        if (done) break;
        sleep(1);
    }

    //cleanup, detach & remove SHM and semaphore
    int status;
    waitpid(childpid, &status, 0);
    if (shmdt(shared) == -1) perror("shmdt");
    if (shmctl(shmid, IPC_RMID, NULL) == -1) perror("shmctl IPC_RMID");
    if (semctl(semid, 0, IPC_RMID) == -1) perror("semctl IPC_RMID");

    printf("P1: done (P2 exited with status=%d)\n", status);
    return 0;
}
