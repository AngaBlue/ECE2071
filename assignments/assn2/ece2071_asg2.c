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
#include <stdbool.h>

#define PROGRAM_NAME "ece2071_asg2"

typedef struct
{
    uint_fast32_t x;
    uint_fast32_t y;
} Point;

typedef struct PointNode
{
    uint_fast32_t x;
    uint_fast32_t y;
    uint_fast32_t distance;
    struct PointNode *next;
} PointNode;

typedef struct PointQueue
{
    PointNode *head;
    PointNode *tail;
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
 * @param path A pointer to the top most node on the solution path
 */
static inline void solve_maze(register uint_fast32_t *const maze, register const uint_fast32_t size, register const Point start, register const Point target, register PointNode **path, uint_fast32_t *length);

/**
 * @brief Prints a stack of points to the console
 *
 * @param top The top of the stack
 */
static inline void print_points(const PointNode *top, const uint_fast32_t length);

/**
 * @brief Safely allocates memory
 *
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void *xmalloc(const size_t size);

static inline void print_maze(const uint_fast32_t *const maze, const uint_fast32_t size)
{
    for (register uint_fast32_t i = 0; i < size; i++)
    {
        for (register uint_fast32_t j = 0; j < size; j++)
        {
            if (maze[i * size + j] == 0)
            {
                printf("%c", '#');
            }
            else
            {
                if (maze[i * size + j] == UINT32_MAX)
                {
                    printf("%c", '.');
                }
                else
                {
                    printf("%d", maze[i * size + j] % 10);
                }
            }
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    // Read the command line arguments
    char *file_name;
    read_args(argc, argv, &file_name);

    // Store the maze as flattened array, where 0 denotes a wall and non-zero denotes a path
    uint_fast32_t size;
    uint_fast32_t *maze;
    Point start, target;
    load_maze(file_name, &maze, &size, &start, &target);

    printf("Loaded %dx%d maze from %s\n", size, size, file_name);
    printf("Pathfinding from (%d, %d) to (%d, %d)...\n", start.x, start.y, target.x, target.y);

    // Solve maze & print results
    PointNode *path = NULL;
    uint_fast32_t length;
    solve_maze(maze, size, start, target, &path, &length);
    print_points(path, length);

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
    return;
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
    *size = floor(sqrt(ftell(file)));
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the maze
    *maze = (uint_fast32_t *)xmalloc(*size * *size * sizeof(uint_fast32_t));

    // Read character by character from file and construct the maze
    register size_t i = 0;
    register char c;
    while ((c = fgetc(file)) != EOF)
    {
        switch (c)
        {
        case '#':
            // Wall
            (*maze)[i++] = 0;
            break;
        case '.':
            // Path
            (*maze)[i++] = UINT32_MAX;
            break;
        case 'S':
            // Start
            start->x = i % *size;
            start->y = i / *size;
            (*maze)[i++] = 0;
            break;
        case 'T':
            // Target
            target->x = i % *size;
            target->y = i / *size;
            (*maze)[i++] = UINT32_MAX;
            break;
        }
    }

    fclose(file);
    return;
}

static inline void solve_maze(register uint_fast32_t *const maze, register const uint_fast32_t size, register const Point start, register const Point target, register PointNode **path, uint_fast32_t *length)
{
    // Point Queue
    register PointQueue queue = {
        .head = NULL,
        .tail = NULL};

    // Create the start node
    queue.head = queue.tail = (PointNode *)xmalloc(sizeof(PointNode));
    queue.head->x = start.x;
    queue.head->y = start.y;
    queue.head->distance = 0;
    queue.head->next = NULL;

    // Search the queue
    register PointNode *node;
    register uint_fast32_t x, y;
    register uint_fast32_t i;
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
        if (node->x == target.x && node->y == target.y)
        {
            *path = node;
            *length = node->distance;
            break;
        }

        // Check all adjacent nodes
        for (i = 0; i < 4; ++i)
        {
            // Get the adjacent node
            switch (i)
            {
            case 0:
                x = node->x + 1;
                y = node->y;
                break;
            case 1:
                x = node->x;
                y = node->y - 1;
                break;
            case 2:
                x = node->x - 1;
                y = node->y;
                break;
            case 3:
                x = node->x;
                y = node->y + 1;
                break;
            }

            // Check if the node is valid
            if (x != UINT32_MAX && x < size && y != UINT32_MAX && y < size && maze[x + y * size] > node->distance)
            {
                // Create the adjacent node
                register PointNode *const adjacent = (PointNode *)xmalloc(sizeof(PointNode));
                adjacent->x = x;
                adjacent->y = y;
                adjacent->distance = maze[x + y * size] = node->distance + 1;
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
    if (*path == NULL)
    {
        return;
    }

    // Reverse traversal of the path
    while ((*path)->x != start.x || (*path)->y != start.y)
    {
        node = *path;
        // Check all adjacent nodes
        for (i = 0; i < 4; i++)
        {
            // Get the adjacent node
            switch (i)
            {
            case 0:
                x = node->x + 1;
                y = node->y;
                break;
            case 1:
                x = node->x;
                y = node->y - 1;
                break;
            case 2:
                x = node->x - 1;
                y = node->y;
                break;
            case 3:
                x = node->x;
                y = node->y + 1;
                break;
            }

            // Find smaller node
            if (x != UINT32_MAX && x < size && y != UINT32_MAX && y < size && (maze[x + y * size] < node->distance && maze[x + y * size] != 0) || (x == start.x && y == start.y))
            {
                // Create the new node
                PointNode *const new_node = (PointNode *)xmalloc(sizeof(PointNode));
                new_node->x = x;
                new_node->y = y;
                new_node->distance = maze[x + y * size];
                new_node->next = *path;
                *path = new_node;
                break;
            }
        }
    }

    // Found shortest path
    return;
}

static inline void print_points(const PointNode *top, const uint_fast32_t length)
{
    if (top == NULL)
    {
        printf("No path found\n");
    }
    else
    {
        printf("The shortest path is %d steps long\n", length);
        do
        {
            printf("%d,%d ", top->x, top->y);
            top = top->next;
        } while (top != NULL);
    }
}

void *xmalloc(const size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        puts("Error allocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *xcalloc(const size_t length, const size_t element_size)
{
    void *ptr = calloc(length, element_size);
    if (ptr == NULL)
    {
        puts("Error allocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}
