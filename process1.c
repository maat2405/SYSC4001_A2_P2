#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        execl("./process2", "process2", NULL);
        printf("Error: exec failed\n");
        return 1;
    }
    else {
        int counter = 0;
        int cycle = 0;

        while (1) {
            if (counter % 3 == 0)
                printf("Cycle %d: %d is a multiple of 3\n", cycle, counter);
            else
                printf("Cycle %d\n", cycle);
            counter++;
            cycle++;
            sleep(1);
        }
    }
}