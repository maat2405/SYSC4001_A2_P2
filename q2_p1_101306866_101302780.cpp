//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork(); //fork a child

    if (pid < 0) { //If the pid < 0 there is an error
        printf("Fork failed\n");
        return 1;
    }
    else if (pid == 0) { //when pid = 0 its the child process; Child = process 2
        execl("./q2_p2", "q2_p2", NULL);//if successful, never returns
        printf("Error: exec failed\n"); //run if the exec fails
        return 1;
    }
    else { //parent process; runs forever and prints the cycles
        int counter = 0; 
        int cycle = 0;

        while (1) {
            if (counter % 3 == 0)
                printf("Cycle number: %d - %d is a multiple of 3\n", cycle, counter); //prints multiples of three based on the cycle number
            else
                printf("Cycle number: %d\n", cycle);
            counter++; //counts up
            cycle++; //tracks the loop count
            sleep(1); //sleep slows down the output by one second
        }
    }
}