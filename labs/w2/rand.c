#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("RAND_MAX: %d\n", RAND_MAX);

    int median = RAND_MAX / 2;

    unsigned int plus_count, minus_count = 0;

    for (unsigned int i = 0; i < 500; i++) {
        int rand_num = rand();
        if (rand_num > median) {
            plus_count++;
        } else {
            minus_count++;
        }

        printf("Difference: %d\n", plus_count - minus_count);
    }

    printf("Above median: %d\n", plus_count);
    printf("Below median: %d\n", minus_count);

    return 0;
} 
