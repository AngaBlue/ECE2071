#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define SIZE 12

// Frame time in milliseconds
#define FRAME_TIME 100

typedef struct
{
    uint_fast32_t row;
    uint_fast32_t col;
} Point;

void traverse_maze(char maze[SIZE][SIZE], Point position);
void print_maze(char maze[SIZE][SIZE]);

int main()
{
    // Simple maze
    /*
    char maze[SIZE][SIZE] = {
        {'#', '#', '#', '.', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#'},
        {'.', '.', '#', '.', '#', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '#', '#', '.', '#', '.', '.', '.', '.', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '#', '#', '#', '.', '#', '.', '#'},
        {'#', '#', '#', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        {'#', '.', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        {'#', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}};
*/
    // Complex maze
    char maze[SIZE][SIZE] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.', '#'},
        {'.', '.', '#', '.', '#', '.', '#', '#', '#', '#', '.', '#'},
        {'#', '#', '#', '.', '#', '.', '.', '.', '.', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '#', '#', '#', '.', '#', '.', '.'},
        {'#', '#', '#', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        {'#', '.', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        {'#', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '#', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '.', '#', '#', '#', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '#', '.', '.', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}};

    // Find start
    Point start;
    for (uint_fast32_t row = 0; row < SIZE - 1; ++row)
    {
        if (maze[row][0] == '.')
        {
            start.row = row;
            start.col = 0;
            break;
        }
    }

    if (start.row == 0 && start.col == 0)
    {
        puts("No start found.");
        return EXIT_FAILURE;
    }

    // Traverse maze
    srand(time(NULL));
    traverse_maze(maze, start);
    return EXIT_SUCCESS;
}

void traverse_maze(char maze[SIZE][SIZE], Point position)
{
    // Print maze
    maze[position.row][position.col] = 'o';
    print_maze(maze);

    // Check if we are at the end
    if (position.row == 0 || position.row == SIZE - 1 || position.col == SIZE - 1)
        return;

    // Mark as visited
    maze[position.row][position.col] = 'x';

    // Find random valid position
    Point positions[4];
    uint_fast32_t positions_count = 0;

    // Check up
    if (position.row > 0 && maze[position.row - 1][position.col] != '#')
    {
        positions[positions_count].row = position.row - 1;
        positions[positions_count].col = position.col;
        positions_count++;
    }

    // Check down
    if (position.row < SIZE - 1 && maze[position.row + 1][position.col] != '#')
    {
        positions[positions_count].row = position.row + 1;
        positions[positions_count].col = position.col;
        positions_count++;
    }

    // Check left
    if (position.col > 0 && maze[position.row][position.col - 1] != '#')
    {
        positions[positions_count].row = position.row;
        positions[positions_count].col = position.col - 1;
        positions_count++;
    }

    // Check right
    if (position.col < SIZE - 1 && maze[position.row][position.col + 1] != '#')
    {
        positions[positions_count].row = position.row;
        positions[positions_count].col = position.col + 1;
        positions_count++;
    }

    // Check if we found any valid positions
    if (positions_count == 0)
    {
        puts("No valid positions found.");
        return;
    }

    // Choose random position
    uint_fast32_t random_position = rand() % positions_count;
    traverse_maze(maze, positions[random_position]);

    return;
}

void print_maze(char maze[SIZE][SIZE])
{
    // Clear output
    system("cls");

    // Print maze
    for (uint_fast32_t i = 0; i < SIZE; i++)
    {
        for (uint_fast32_t j = 0; j < SIZE; j++)
        {

            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }

    // Wait...
    usleep(FRAME_TIME * 1000);
    return;
}
