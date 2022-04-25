/**
 * @file ece2071_asg2.c
 * @author Angus Bosmans (abos0006@student.monash.edu)
 * @brief ECE2071 Assignment 2
 * @date 2022-04-04
 *
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define PROGRAM_NAME "ece2071_asg2"
#define WALL_CHAR '#'
#define PATH_CHAR '.'
#define SOURCE_CHAR 'S'
#define TARGET_CHAR 'T'

typedef struct Point
{
    uint_fast32_t row, col;
} Point;

typedef struct PointNode
{
    uint_fast32_t row, col, distance;
    struct PointNode *next;
} PointNode;

typedef struct PointQueue
{
    PointNode *head, *tail;
} PointQueue;

/**
 * @brief Reads and resolves command line arguments
 *
 * @param argc Number of arguments supplied
 * @param argv Arguments supplied
 * @param file_name A pointer to the file name
 */
static inline void read_args(const int argc, const char const *argv[], char **const file_name);

/**
 * @brief Loads the maze from the file into memory
 *
 * @param file_name The name of the file to load
 * @param maze A pointer to a flattened maze array
 * @param size A pointer to the size of the maze
 * @param start A pointer to the start of the maze
 * @param target A pointer to the target of the maze
 */
static inline void load_maze(const char *const file_name, uint_fast32_t **const maze, uint_fast32_t *const size, Point *const start, Point *const target);

/**
 * @brief Solves the maze
 *
 * @param maze The maze to solve
 * @param size The size of the maze
 * @param start The start of the maze
 * @param target The target of the maze
 */
static inline void solve_maze(uint_fast32_t *const maze, register const uint_fast32_t size, register const Point start, register const Point target);

/**
 * @brief Safely allocates memory
 *
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
static inline void *xmalloc(const size_t size);

int main(int argc, char const *argv[])
{
    // Read the command line arguments
    char *file_name;
    read_args(argc, argv, &file_name);

    // Store the maze as flattened array, where 0 denotes a wall and non-zero denotes a path
    // This will reduce comparisons when solving the maze
    uint_fast32_t size;
    uint_fast32_t *maze;
    Point start, target;
    load_maze(file_name, &maze, &size, &start, &target);

    // Solve maze & print results
    solve_maze(maze, size, start, target);

    return EXIT_SUCCESS;
}

static inline void read_args(const int argc, const char const *argv[], char **const file_name)
{
    // Check if correct number of arguments are supplied
    if (argc != 2)
    {
        printf("Received %d arguments, expected 1\n", argc - 1);
        printf("Usage: %s <maze.txt>\n", PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }

    // Copy file name
    *file_name = xmalloc(strlen(argv[1]) + 1);
    strcpy(*file_name, argv[1]);
}

static inline void load_maze(const char *const file_name, uint_fast32_t **const maze, uint_fast32_t *const size, Point *const start, Point *const target)
{
    FILE *const file = fopen(file_name, "rb");
    if (file == NULL)
    {
        printf("Failed to open file %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    // Read the size of the maze
    fseek(file, 0, SEEK_END);
    register uint_fast32_t file_size = ftell(file);
    *size = floor(sqrt(file_size));
    rewind(file);

    // Allocate memory for the file & maze
    char *file_data = xmalloc(file_size);
    *maze = (uint_fast32_t *)xmalloc(*size * *size * sizeof(uint_fast32_t));

    // Read file and construct the maze
    fread(file_data, file_size, 1, file);
    register size_t i = 0, j;
    for (j = 0; j < file_size; ++j)
    {
        switch (file_data[j])
        {
        case WALL_CHAR:
            (*maze)[i++] = 0;
            break;
        case PATH_CHAR:
            (*maze)[i++] = UINT32_MAX;
            break;
        case SOURCE_CHAR:
            start->row = i / *size;
            start->col = i % *size;
            (*maze)[i++] = 0;
            break;
        case TARGET_CHAR:
            target->row = i / *size;
            target->col = i % *size;
            (*maze)[i++] = UINT32_MAX;
            break;
        }
    }

    free(file_data);
    fclose(file);
}

static inline void solve_maze(uint_fast32_t *const maze, register const uint_fast32_t size, register const Point start, register const Point target)
{
    // Point Queue
    register PointQueue queue = {NULL, NULL};
    register uint_fast32_t length;

    // Create the start node
    queue.head = queue.tail = (PointNode *)xmalloc(sizeof(PointNode));
    queue.head->row = start.row;
    queue.head->col = start.col;
    queue.head->distance = 0;
    queue.head->next = NULL;

    // Adjacent node macro, skip cells that fall outside the bounds
#define ADJACENT_NODE(r, c) \
    switch (i)              \
    {                       \
    case 0:                 \
        col = c + 1;        \
        if (col == size)    \
            continue;       \
        row = r;            \
        break;              \
    case 1:                 \
        if (r == 0)         \
            continue;       \
        row = r - 1;        \
        col = c;            \
        break;              \
    case 2:                 \
        if (c == 0)         \
            continue;       \
        row = r;            \
        col = c - 1;        \
        break;              \
    case 3:                 \
        row = r + 1;        \
        if (row == size)    \
            continue;       \
        col = c;            \
        break;              \
    }

    // Search the queue
    register PointNode *node;
    register Point current = {0, 0};
    register size_t index;
    register uint_fast32_t i, row, col;
    while (queue.head != NULL)
    {
        // Pop the head
        node = queue.head;
        queue.head = node->next;
        if (queue.head == NULL)
        {
            queue.tail = NULL;
        }

        // Check if the node is the target
        if (node->row == target.row && node->col == target.col)
        {
            node->next = NULL;
            current.row = node->row;
            current.col = node->col;
            length = node->distance;
            break;
        }

        // Check all adjacent nodes
        for (i = 0; i < 4; ++i)
        {
            // Get the adjacent node
            ADJACENT_NODE(node->row, node->col)

            // Check if the node is valid
            index = row * size + col;
            if (maze[index] > node->distance)
            {
                // Create the adjacent node
                PointNode *const adjacent = (PointNode *)xmalloc(sizeof(PointNode));
                adjacent->row = row;
                adjacent->col = col;
                adjacent->distance = maze[index] = node->distance + 1;
                adjacent->next = NULL;

                // Add the adjacent node to the queue
                if (queue.tail == NULL)
                {
                    queue.head = queue.tail = adjacent;
                }
                else
                {
                    queue.tail->next = adjacent;
                    queue.tail = adjacent;
                }
            }
        }

        // Free node
        free(node);
    }

    // If target was never found
    if (current.row == 0 && current.col == 0)
    {
        puts("No path found.");
        return;
    }

    // Reverse traversal of the path until start is met
    printf("%d,%d", target.row + 1, target.col + 1);
    while (--length)
    {
        // Check all adjacent nodes
        for (i = 0; i < 4; ++i)
        {
            // Get the adjacent node
            ADJACENT_NODE(current.row, current.col)

            if (maze[row * size + col] == length)
            {
                // Print path
                printf(" %d,%d", row + 1, col + 1);
                current.row = row;
                current.col = col;
                break;
            }
        }
    }
    printf(" %d,%d", start.row + 1, start.col + 1);
}

static inline void *xmalloc(const size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        puts("Error allocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}
