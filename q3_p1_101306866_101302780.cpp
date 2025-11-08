//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();  //create the child

    if (pid < 0) { //if pid < 0 creates an error
        printf("Fork failed\n");
        return 1;
    }
    else if (pid == 0) { //when pid = 0 its the child process; Child = process 2 
        execl("./q3_p2", "q3_p2", NULL);
        printf("Error: exec failed\n"); //run this only if exec fails
        return 1;
    }
    else {  //parent function
        wait(&pid); // waits for child to finish
        printf("Process 2 Completed... Exiting Process 1");
}
    }
