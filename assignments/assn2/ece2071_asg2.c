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
    uint_fast32_t x, y;
} Point;

typedef struct PointNode
{
    uint_fast32_t x, y, distance;
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
 * @param path A pointer to the top most node on the solution path
 */
static inline void solve_maze(uint_fast32_t *const maze, register const uint_fast32_t size, register const Point start, register const Point target, PointNode **path);

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
    PointNode *path = NULL;
    solve_maze(maze, size, start, target, &path);

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
    uint_fast32_t file_size = ftell(file);
    *size = floor(sqrt(file_size));
    fseek(file, 0, SEEK_SET);

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
            start->x = i % *size;
            start->y = i / *size;
            (*maze)[i++] = 0;
            break;
        case TARGET_CHAR:
            target->x = i % *size;
            target->y = i / *size;
            (*maze)[i++] = UINT32_MAX;
            break;
        }
    }

    free(file_data);
    fclose(file);
}

static inline void solve_maze(uint_fast32_t *const maze, register const uint_fast32_t size, register const Point start, register const Point target, PointNode **path)
{
    // Point Queue
    register PointQueue queue = {NULL, NULL};
    uint_fast32_t length;

    // Create the start node
    queue.head = queue.tail = (PointNode *)xmalloc(sizeof(PointNode));
    queue.head->x = start.x;
    queue.head->y = start.y;
    queue.head->distance = 0;
    queue.head->next = NULL;

    // Adjacent node macro, skip cells that fall outside the bounds
#define ADJACENT_NODE     \
    switch (i)            \
    {                     \
    case 0:               \
        x = node->x + 1;  \
        if (x == size)    \
            continue;     \
        y = node->y;      \
        break;            \
    case 1:               \
        if (node->y == 0) \
            continue;     \
        ;                 \
        x = node->x;      \
        y = node->y - 1;  \
        break;            \
    case 2:               \
        if (node->x == 0) \
            continue;     \
        ;                 \
        x = node->x - 1;  \
        y = node->y;      \
        break;            \
    case 3:               \
        y = node->y + 1;  \
        if (y == size)    \
            continue;     \
        x = node->x;      \
        break;            \
    }

    // Search the queue
    register PointNode *node;
    register uint_fast32_t i, x, y;
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
            node->next = NULL;
            *path = node;
            length = node->distance;
            break;
        }

        // Check all adjacent nodes
        for (i = 0; i < 4; ++i)
        {
            // Get the adjacent node
            ADJACENT_NODE

            // Check if the node is valid
            if (maze[x + y * size] > node->distance)
            {
                // Create the adjacent node
                PointNode *const adjacent = (PointNode *)xmalloc(sizeof(PointNode));
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
        puts("No path found.");
        return;
    }

    // Reverse traversal of the path until start is met
    while ((*path)->x != start.x || (*path)->y != start.y)
    {
        node = *path;
        // Check all adjacent nodes
        for (i = 0; i < 4; ++i)
        {
            // Get the adjacent node
            ADJACENT_NODE

            // Find smaller (non-zero) node or start node
            if ((maze[x + y * size] == node->distance - 1 && maze[x + y * size] != 0) || (x == start.x && y == start.y))
            {
                // Print path
                printf("%d,%d ", y + 1, x + 1);

                // Create pointless stack
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
