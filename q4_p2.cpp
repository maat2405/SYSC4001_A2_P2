//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780
#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <shmid>\n", argv[0]);
        return 1;
    }

    int shmid = atoi(argv[1]);
    int *shared = (int*)shmat(shmid, NULL, 0);
    if (shared == (void*)-1) {
        perror("shmat error");
        return 1;
    }

    setvbuf(stdout, NULL, _IONBF, 0); 


    while (shared[1] <= 100) {
        sleep(1);
    }


    int last_seen = -1;
    while (shared[1] <= 500) {
        int cur = shared[1];
        int m   = shared[0];

        if (cur != last_seen) {
            if (cur % m == 0)
                printf("P2: saw counter=%d (multiple of %d)\n", cur, m);
            else
                printf("P2: saw counter=%d\n", cur);
            last_seen = cur;
        }
        sleep(1);
    }

    shmdt(shared);
    return 0;
}
