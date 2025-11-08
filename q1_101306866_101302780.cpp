//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780


#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
    pid_t pid;
    pid = fork();   // Create a new process
    if (pid < 0) { //If the pid < 0 there is an error
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { //when pid = 0 it is the child process
        int counter = 0;
        while (1) { //makes it run forever
            printf("Child counter = %d\n", counter++);
            sleep(1); //sleep slows down the output by one second
        }
    }
    else { // if the pid > 0 it is the parent process
        int counter = 0;
        while (1) {
            printf("Parent counter = %d\n", counter++);
            sleep(1);
        }
    }
return 0;
}