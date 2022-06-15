#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(void)

{
    // Seed the random number generator with the current time
    // This will ensure that the random numbers are different every run
    srand(time(NULL));

    int number = rand();
    int square = number * number;

    printf("Random number: %d\n", number);

    // Open files for writing, w = write
    FILE *correct_file = fopen("Correct.txt", "w");
    FILE *wrong_file = fopen("Wrong.txt", "w");

    while (1)
    {
        printf("Please input the square of %d: ", number);

        // Read a digit from stdin (console input) to guess
        int guess;
        scanf("%d", &guess);

        if (guess == 0)
            break;

        if (guess == square)
        {
            puts("Correct!");
            fprintf(correct_file, "%d\n", guess);
        }
        else
        {
            puts("Wrong!");
            fprintf(wrong_file, "%d\n", guess);
        }
    }

    // Close the files now that we are done 
    fclose(correct_file);
    fclose(wrong_file);

    return 0;
}
