/**
 * @file ece2071_asg1.c
 * @author Angus Bosmans (abos0006@student.monash.edu)
 * @brief ECE2071 Assignment 1
 * @date 2022-03-10
 *
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define FILENAME "pi_50m.txt"

typedef struct
{
    uint_fast32_t *distances;
    size_t length;
    size_t last_index;
} Palindrome;

typedef struct
{
    Palindrome *values;
    size_t length;
} Palindromes;

/**
 * @brief Loads the digits from the file
 *
 * @param filename The name of the file to load
 * @param content A pointer to the pointer of the string to store the digits in
 * @param length A pointer to the size of the string
 * @return The digits as a string
 */
static inline void load_file(const char *const filename, char **const content, size_t *const length);

/**
 * @brief Finds all palindromes and calculates the distance since the last palindrome of the same length
 */
static inline void find_palindromes();

/**
 * @brief Finds the median of an array using a modified counting sort
 *
 * @param array The array to find the median of
 * @param size The size of the array
 * @return uint_fast32_t
 */
static inline uint_fast32_t counting_median(const uint_fast32_t *const array, register const uint_fast32_t size);

/**
 * @brief Comparison function for qsort
 *
 * @param a
 * @param b
 * @return int
 */
static inline int qsort_compare(const void *a, const void *b);

/**
 * @brief Safely allocates memory
 *
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void *xmalloc(const size_t size);

/**
 * @brief Safely allocates memory
 *
 * @param length The length of the array to allocate
 * @param size The size of each element
 * @return void* Pointer to the new memory
 */
void *xcalloc(const size_t length, const size_t element_size);

/**
 * Global variables
 */

Palindromes palindromes;

char *digits;
size_t digits_length;

/**
 * Begin program
 */

int main(void)
{
    load_file(FILENAME, &digits, &digits_length);

    if (digits == NULL)
    {
        printf("Failed to load digits from file \"%s\".", FILENAME);
        return EXIT_FAILURE;
    }

    // Initialise palindrome lengths, allocating a predicted amount of memory
    palindromes.length = (log10(digits_length) + 1) * 2;
    palindromes.values = (Palindrome *)xmalloc(palindromes.length * sizeof(Palindrome));

    // Initialize new palindrome length
    for (uint_fast32_t i = 2; i <= palindromes.length; ++i)
    {
        Palindrome *palindrome = &palindromes.values[i];

        // Predict length of palindrome and allocate memory
        uint_fast32_t predicted_length = ceil((digits_length / pow(10, (int)i / 2) * 1.1));
        palindrome->distances = (uint_fast32_t *)xmalloc(predicted_length * sizeof(uint_fast32_t));
        palindrome->length = 0;
        palindrome->last_index = 0;
    }

    find_palindromes();

    // Output the palindromes
    printf("%-13s %-13s %-12s\n", "Palindrome", "Number of", "Median Digit");
    printf("%-13s %-13s %-12s\n", "Length", "Palindromes", "Distance");
    printf("%-13s %-13s %-12s\n", "-----------", "-----------", "------------");

    for (size_t i = 2; i < palindromes.length; ++i)
    {
        Palindrome *palindrome = &palindromes.values[i];

        if (palindrome->length == 0)
        {
            printf("%11zi %13zi %14c\n", i, 0, '-');
            break;
        }

        // Find median distance
        uint_fast32_t median_distance;

        // Use either counting sort or qsort to find median
        if (digits_length / palindrome->length < palindrome->length)
        {
            // Faster counting sort
            median_distance = counting_median(palindrome->distances, palindrome->length);
        }
        else
        {
            // Memory efficient qsort
            qsort(palindrome->distances, palindrome->length, sizeof(uint_fast32_t), qsort_compare);
            if (palindrome->length % 2 == 0)
            {
                median_distance = palindrome->distances[palindrome->length / 2 - 1];
            }
            else
            {
                median_distance = palindrome->distances[palindrome->length / 2];
            }
        }

        // Output the palindromes
        printf("%11zi %13zi %14ld\n", i, palindrome->length + 1, median_distance - 1);
    }

    return EXIT_SUCCESS;
}

static inline void load_file(const char *const filename, char **const content, size_t *const length)
{
    *content = 0;
    FILE *file = fopen(filename, "rb");

    if (file)
    {
        // Navigate to end of file to get file length
        fseek(file, 0, SEEK_END);
        *length = ftell(file);
        fseek(file, 2, SEEK_SET);

        // Allocate memory for file content
        *content = (char *)xmalloc((*length + 1) * sizeof(char));

        // Read each character from file into digits
        fread(*content + 1, sizeof(char), *length - 2, file);

        (*content)[0] = '!';
        (*content)[*length] = '@';

        // Close file
        fclose(file);
    }
}

static inline void find_palindromes()
{
    register uint_fast32_t radius, length;
    register size_t start, end, i;
    register Palindrome *palindrome;

/**
 * @brief Stores a palindrome of the given length and calculates the distance since the last palindrome of the same length
 *
 * @param index Start index of the palindrome
 * @param length Length of the palindrome
 */
#define STORE_PALINDROME(index, length)                                             \
    /* Store the palindrome */                                                      \
    palindrome = &palindromes.values[length];                                       \
    /* If exist last index, calculate distance */                                   \
    if (palindrome->last_index != 0)                                                \
    {                                                                               \
        palindrome->distances[palindrome->length] = index - palindrome->last_index; \
        ++palindrome->length;                                                       \
    }                                                                               \
    /* Update last index */                                                         \
    palindrome->last_index = index;

    // Loop through the digits
    for (i = 1; i < digits_length; ++i)
    {
        // Even Palindromes
        radius = 1;
        while (true)
        {
            start = i - radius;

            if (digits[start] != digits[i + radius - 1])
                break;

            // Valid palindrome
            length = radius * 2;
            STORE_PALINDROME(start, length);
            ++radius;
        }

        // Odd Palindromes
        radius = 1;
        while (true)
        {
            start = i - radius;

            if (digits[start] != digits[i + radius])
                break;

            // Valid palindrome
            length = radius * 2 + 1;
            STORE_PALINDROME(start, length);
            ++radius;
        }
    }

    free(digits);
#undef STORE_PALINDROME
}

static inline uint_fast32_t counting_median(const uint_fast32_t *const array, register const uint_fast32_t size)
{
    // Find max
    register uint_fast32_t i, max = 0;

    for (i = 0; i < size; ++i)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }

    // Create a histogram, initialised with 0 values
    uint_fast32_t *buckets = xcalloc(max + 1, sizeof(uint_fast32_t));

    // Count the number of occurrences of each value
    for (i = 0; i < size; ++i)
    {
        ++buckets[array[i]];
    }

    register uint_fast32_t total = 0;
    register uint_fast32_t target = size % 2 == 0 ? (size / 2) - 1 : size / 2;

    // Sum buckets until the median threshold is met
    for (i = 1; i <= max; ++i)
    {
        total += buckets[i];
        if (total >= target)
        {
            // Median found
            free(buckets);
            return i;
        }
    }

    // Median not found
    free(buckets);
    return 0;
}

static inline int qsort_compare(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
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
