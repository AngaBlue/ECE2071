#include <stdio.h>

int main(void) {
    unsigned int total = 0;
    unsigned int counter = 1;
    unsigned int grade;

    const int MAX = 10;

    while (counter <= MAX) {
        printf("[%d/%d] Enter a grade: \n", counter, MAX);
        scanf("%d", &grade);

        total += grade;
        counter++;

        int numerator = 10;
        int denominator = 3;
        float result = (float) numerator / denominator;
        printf("%f\n", result);
    }

    printf("Average: %.2f\n", (float) total / MAX);
}
