#include <stdio.h>

int main(void) {
    int grade;

    printf("%s", "Grade: ");
    scanf("%d", &grade);

    if (grade >= 50) {
        puts("Passed!");
    } else {
        puts("Failed!");
    }

    puts(grade >= 50 ? "Passed!" : "Failed!");
}
