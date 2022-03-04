#include <stdio.h>

int main(void) {
    int int1, int2;

    printf("%s", "Enter first integer: ");
    scanf("%d", &int1);

    printf("%s", "Enter second integer: ");
    scanf("%d", &int2);

    int sum = int1 + int2;

    printf("Sum: %d\n", sum);

    printf("Floating point division: %f\n", (float) int1 / int2);
}
