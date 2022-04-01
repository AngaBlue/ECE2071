/**
 * @file mat_power.c
 * @author Angus Bosmans (abos0006@student.monash.edu)
 * @brief Raise a matrix in a file to the nth power
 * @date 2022-04-01
 * 
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_NAME "mat_power"

void print_matrix(const double *const matrix, const int size);
void read_args(const int argc, const char const *argv[], int *const power, char **const file_name);
void *xmalloc(const size_t size);

int main(int argc, char const *argv[])
{
    int power;
    char *file_name;
    read_args(argc, argv, &power, &file_name);

    // Read file
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
    {
        printf("Could not open file %s\n", file_name);
        return 1;
    }

    // Count spaces
    int size = 1;
    char c;
    while ((c = fgetc(file)) != '\n')
    {
        if (c == ' ')
        {
            ++size;
        }
    }

    printf("Raising a %dx%d matrix from %s to the power of %d\n", size, size, file_name, power);

    // Allocate memory
    double *matrix_original = (double *)xmalloc(sizeof(double) * size * size);
    double *matrix = (double *)xmalloc(sizeof(double) * size * size);
    double *matrix_temp = (double *)xmalloc(sizeof(double) * size * size);

    // Rewind to start
    fseek(file, 0, SEEK_SET);

    // Read matrix into memory
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            fscanf(file, "%lf", &matrix_original[i * size + j]);
        }
    }

    // Close file
    fclose(file);

    // Copy original matrix to operation matrix
    memcpy(matrix, matrix_original, sizeof(double) * size * size);

    print_matrix(matrix, size);

    // Matrix power iterations
    for (int i = 1; i < power; ++i)
    {
        // Compute matrix multiplication of self
        for (int j = 0; j < size; ++j)
        {
            for (int k = 0; k < size; ++k)
            {
                // For each cell...
                matrix_temp[j * size + k] = 0;
                for (int l = 0; l < size; ++l)
                {
                    // Add the product of row x column of working array and original array
                    matrix_temp[j * size + k] += matrix[j * size + l] * matrix_original[l * size + k];
                }
            }
        }

        // Copy results from matrix_temp to matrix
        memcpy(matrix, matrix_temp, sizeof(double) * size * size);
    }

    print_matrix(matrix, size);

    return EXIT_SUCCESS;
}

void read_args(const int argc, const char const *argv[], int *const power, char **const file_name)
{
    if (argc != 3)
    {
        printf("Received %d args, expected 2\n", argc - 1);
        printf("Usage: %s <power> <matrix.txt>\n", PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }

    *power = atoi(argv[1]);
    if (*power < 0)
    {
        printf("Power must be non-negative\n");
        exit(EXIT_FAILURE);
    }

    // Copy file name
    *file_name = xmalloc(strlen(argv[2]) + 1);
    strcpy(*file_name, argv[2]);

    return;
}

void print_matrix(const double *const matrix, const int size)
{
    puts("");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%8.2lf ", matrix[i * size + j]);
        }
        printf("\n");
    }

    return;
}

void *xmalloc(const size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        printf("Could not allocate memory\n");
        exit(1);
    }
    return ptr;
}
