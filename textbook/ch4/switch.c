#include <stdio.h>
#include <ctype.h>

int main(void) {
    char grade;

    printf("%s", "Enter a letter grade: ");


    while ((grade = getchar()) != EOF) {
        grade = toupper(grade);

        if (grade != '\n') printf("%c: ", grade);

        switch (grade) {
            case 'A':
                puts("Excellent!");
                break;
            case 'B':
                puts("Good job!");
                break;
            case 'C':
                puts("Passed!");
                break;
            case 'D':
            case 'F':
                puts("Failed!");
                break;
            case '\n':
                printf("%s", "Enter a letter grade: ");
                break;
            default:
                puts("Invalid grade.");
                break;
        }
    }
}
