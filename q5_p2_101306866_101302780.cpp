//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780
#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int sem_wait_op(int semid) { 
    struct sembuf op = {0, -1, 0}; return semop(semid, &op, 1); }
static int sem_signal_op(int semid){ 
    struct sembuf op = {0, +1, 0}; return semop(semid, &op, 1); }

int main(int argc, char **argv) {
    if (argc != 3) { fprintf(stderr, "Usage: %s <shmid> <semid>\n", argv[0]); return 1; }

    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);

    int *shared = (int*)shmat(shmid, NULL, 0);
    if (shared == (void*)-1) { perror("shmat error"); return 1; }

    setvbuf(stdout, NULL, _IONBF, 0);

    while (1) {
        int v;
        if (sem_wait_op(semid) == -1) { perror("sem_wait_op"); break; }
        v = shared[1];
        if (sem_signal_op(semid) == -1) { perror("sem_signal_op"); break; }
        if (v > 100) break;
        sleep(1);
    }

    int last_seen = -1;
    while (1) {
        int cur, m, done;

        if (sem_wait_op(semid) == -1) { perror("sem_wait_op"); break; }
        cur = shared[1];
        m   = shared[0];
        done = (cur > 500);
        if (!done && cur != last_seen) {
            if (cur % m == 0) printf("P2: saw counter=%d (multiple of %d)\n", cur, m);
            else              printf("P2: saw counter=%d\n", cur);
            last_seen = cur;
        }
        if (sem_signal_op(semid) == -1) { perror("sem_signal_op"); break; }

        if (done) break;
        sleep(1);
    }

    if (shmdt(shared) == -1) perror("shmdt");
    return 0;
}
