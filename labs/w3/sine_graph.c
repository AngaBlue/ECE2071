#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define LENGTH 360
#define ROWS (LENGTH / 5)
#define WIDTH 30
#define VERTICAL_DIVIDERS 10
#define HORIZONTAL_DIVIDERS 45
#define ROW_SIZE 2 * WIDTH + 1

#define HORIZONTAL_DIVIDER '-'
#define VERTICAL_DIVIDER '|'
#define INTERSECTION '+'
#define MARK 'o'

int main(void)
{
    char *row = malloc(sizeof(char) * (ROW_SIZE + 1));
    if (row == NULL)
    {
        printf("Error allocating memory\n");
        return EXIT_FAILURE;
    }

    printf("%3s %-*d0%*d\n", "", WIDTH, -1, WIDTH, 1);

    for (int i = 0; i <= ROWS; i++)
    {
        const int x = i * (LENGTH / ROWS);

        if (x % HORIZONTAL_DIVIDERS == 0)
        {
            // Horizontal divider line
            for (int j = 0; j < ROW_SIZE; j++)
                row[j] = j % VERTICAL_DIVIDERS == 0 ? INTERSECTION : HORIZONTAL_DIVIDER;
        }
        else
        {
            // Blank line
            for (int j = 0; j < ROW_SIZE; j++)
                row[j] = j % VERTICAL_DIVIDERS == 0 ? VERTICAL_DIVIDER : ' ';
        }

        // Sine point
        const int y = round(sin(x * M_PI / 180) * WIDTH) + WIDTH;
        row[y] = MARK;

        // Terminate String
        row[ROW_SIZE] = '\0';

        printf("%3d %s\n", x, row);
    }

    return EXIT_SUCCESS;
}
