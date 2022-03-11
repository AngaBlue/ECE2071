#include <stdio.h>

int main(void) {
    int input_value;
    int seconds, minutes, hours;
    puts("Enter a number of seconds: ");
    scanf("%d", &input_value);
    hours = input_value / 3600;
    minutes = input_value / 60 % 60;
    seconds = input_value % 60;
    printf("%d seconds is equivalent to:\n%d hours, %d minutes and %d seconds.\n", input_value, hours, minutes, seconds);

    return 0;
}
