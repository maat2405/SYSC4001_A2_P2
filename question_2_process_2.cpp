#include <stdio.h>
#include <unistd.h>

int main() {
    int counter = 0;
    int cycle = 0;

    while (1) {
        counter--;
        if (counter % 3 == 0)
            printf("Cycle number: %d - %d is a multiple of 3\n", cycle, counter);
        else
            printf("Cycle number: %d\n", cycle);
        cycle++;
        sleep(1);
    }

    return 0;
}
