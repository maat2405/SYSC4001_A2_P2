#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) {
        int counter = 0;
        while (1) {
            printf("Child counter = %d\n", counter++);
            sleep(1);
        }
    }
    else {
        int counter = 0;
        while (1) {
            printf("Parent counter = %d\n", counter++);
            sleep(1);
        }
    }
return 0;
}