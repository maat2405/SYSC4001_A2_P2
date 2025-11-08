//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    int counter = 0; //set counter 0

    while (counter > -500) { //stops when <= -500
        counter--;
        if (counter % 3 == 0)
            printf("Cycle number: %d - %d is a multiple of 3\n", counter, counter);
        else
            printf("Cycle number: %d\n", counter);
        sleep(1); //sleep slows down the output by one second
    }

    return 0;
}
