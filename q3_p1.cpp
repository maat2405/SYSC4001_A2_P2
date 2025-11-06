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
        execl("./q3_p2", "q3_p2", NULL);
        printf("Error: exec failed\n");
        return 1;
    }
    else { 
        wait(&pid);
        printf("Process 2 Completed... Exiting Process 1");
}
    }
