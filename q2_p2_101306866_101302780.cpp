//Sonai Haghgooie 101306866
//Maathusan Sathiendran 101302780
#include <stdio.h>
#include <unistd.h>

int main() {
    int counter = 0; //initialized counter to 0

    while (1) { //run forever
        counter--; //counts down
        if (counter % 3 == 0) 
            printf("Cycle number: %d - %d is a multiple of 3\n", counter, counter); 
        else
            printf("Cycle number: %d\n", counter);
        sleep(1); //sleep slows down the output by one second
    }

    return 0;
}
