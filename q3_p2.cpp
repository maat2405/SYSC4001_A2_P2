#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    int counter = 0;

    while (counter > -500) {
        counter--;
        if (counter % 3 == 0)
            printf("Cycle number: %d - %d is a multiple of 3\n", counter, counter);
        else
            printf("Cycle number: %d\n", counter);
        sleep(1);
    }

    return 0;
}
