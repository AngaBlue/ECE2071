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
#include <string.h>

#define FILENAME "pi_50m.txt"

typedef struct
{
    uint_fast32_t *distances;
    size_t size;
    size_t length;
    size_t predicted_length;
    size_t last_index;
} Palindrome;

typedef struct
{
    Palindrome *values;
    size_t size;
    size_t length;
} Palindromes;

/**
 * @brief Loads the digits from the file
 * @return The digits as a string
 */
void load_file(char *filename, char **content, size_t *length);

/**
 * @brief Stores a palindrome of the given length and calculates the distance since the last palindrome of the same length
 *
 * @param index Start index of the palindrome
 * @param length Length of the palindrome
 */
void store_palindrome(size_t index, uint_fast32_t length);

/**
 * @brief Safely allocates memory
 *
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void *xmalloc(size_t size);

/**
 * @brief Safely reallocates memory
 *
 * @param ptr Pointer to the memory to reallocate
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void *xrealloc(void *ptr, size_t size);

/**
 * @brief A fast unsigned integer radix sort implementation adapted from https://github.com/AwardOfSky/Fast-Radix-Sort
 *
 * @param vector Values to sort
 * @param size The length of the vector
 */
void radix_sort(register uint_fast32_t vector[], register const uint_fast32_t size);

Palindromes palindromes;

char *digits;
size_t digits_length;

int main(void)
{
    load_file(FILENAME, &digits, &digits_length);

    if (digits == NULL)
    {
        printf("Failed to load digits from file \"%s\".", FILENAME);
        return EXIT_FAILURE;
    }

    // Initialise palindromes, allocating a predicted amount of memory
    palindromes.size = log10(digits_length) * 2;
    palindromes.values = (Palindrome *)xmalloc(palindromes.size * sizeof(Palindrome)),
    palindromes.length = 0;

    // Loop through the digits
    for (size_t i = 0; i < digits_length; i++)
    {
        // Even Palindromes
        int radius = 1;
        while (true)
        {
            size_t start = i - radius;
            size_t end = i + radius - 1;

            if (start < 0 || end > digits_length)
                break;

            if (digits[start] != digits[end])
                break;

            // Valid palindrome
            store_palindrome(start, 2 * radius);
            radius++;
        }

        // Odd Palindromes
        radius = 1;
        while (true)
        {
            size_t start = i - radius;
            size_t end = i + radius;

            if (start < 0 || end > digits_length)
                break;

            if (digits[start] != digits[end])
                break;

            // Valid palindrome
            store_palindrome(start, 2 * radius + 1);
            radius++;
        }
    }

    free(digits);

    // Output the palindromes
    printf("%-13s %-13s %-12s\n", "Palindrome", "Number of", "Median Digit");
    printf("%-13s %-13s %-12s\n", "Length", "Palindromes", "Distance");
    printf("%-13s %-13s %-12s\n", "-----------", "-----------", "------------");

    for (size_t i = 2; i < palindromes.length; i++)
    {
        Palindrome *palindrome = &palindromes.values[i];

        // Sort the distances & find median distance
        radix_sort(palindrome->distances, palindrome->length);
        uint_fast32_t median_distance = palindrome->distances[palindrome->length / 2];

        // Output the palindromes
        printf("%11d %13d %14d\n", i, palindrome->length + 1, palindrome->length == 0 ? 0 : median_distance);
    }

    return EXIT_SUCCESS;
}

void load_file(char *filename, char **content, size_t *length)
{
    *content = 0;
    FILE *file = fopen(filename, "rb");

    if (file)
    {
        // Navigate to end of file to get file length
        fseek(file, 0, SEEK_END);
        *length = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Allocate memory for file content
        *content = (char *)xmalloc((*length + 1) * sizeof(char));
        if (*content)
        {
            // Read each character from file into digits
            fread(*content, sizeof(char), *length, file);
        }

        // Close file
        fclose(file);
    }
}

void store_palindrome(size_t index, uint_fast32_t length)
{
    // Check if palindrome is already stored
    if (palindromes.size <= length)
    {
        // Allocate more space for new palindrome length
        palindromes.values = (Palindrome *)xrealloc(palindromes.values, (length + 1) * sizeof(Palindrome));
        palindromes.size = length + 1;
    }

    if (palindromes.length <= length)
    {
        // Initialize new palindrome lengths
        for (uint_fast32_t i = palindromes.length >= 2 ? palindromes.length : 2; i <= length; i++)
        {
            Palindrome *palindrome = &palindromes.values[i];

            // Predict length of palindrome and allocate memory
            palindrome->size = palindrome->predicted_length = digits_length / pow(10, (int)i / 2);
            palindrome->distances = (uint_fast32_t *)xmalloc(palindrome->predicted_length * sizeof(uint_fast32_t));
            palindrome->length = 0;
            palindrome->last_index = 0;
        }

        palindromes.length = length + 1;
    }

    // Store the palindrome
    Palindrome *palindrome = &palindromes.values[length];

    // Resize dynamic array
    if (palindrome->length == palindrome->size)
    {
        palindrome->size += (palindrome->predicted_length / 100) + 1;
        palindrome->distances = (uint_fast32_t *)xrealloc(palindrome->distances, palindrome->size * sizeof(uint_fast32_t));
    }

    // If exist last index, calculate distance
    if (palindrome->last_index != 0)
    {
        palindrome->distances[palindrome->length] = index - palindrome->last_index;
        palindrome->length++;
    }

    // Update last index
    palindrome->last_index = index;
}

void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        puts("Error allocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if (ptr == NULL)
    {
        puts("Error reallocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void radix_sort(register uint_fast32_t vector[], register const uint_fast32_t size)
{

    /* Support for variable sized integers without overflow warnings */
    const int MAX_UINT = ((((1 << ((sizeof(uint_fast32_t) << 3) - 2)) - 1) << 1) + 1);
    const int LAST_EXP = (sizeof(uint_fast32_t) - 1) << 3;

    /* Define std preliminary, constrain and expression to check if all bytes are sorted */
#define PRELIMINARY 100
#define MISSING_BITS exp<(sizeof(uint_fast32_t) << 3) && (max >> exp)> 0
    /* Check for biggest integer in [a, b[ array segment */
#define LOOP_MAX(a, b)                               \
    for (s = &vector[a], k = &vector[b]; s < k; ++s) \
    {                                                \
        if (*s > max)                                \
        {                                            \
            max = *s;                                \
        }                                            \
    }

    /* b = helper array pointer ; s, k and i = array iterators */
    /* exp = bits sorted, max = maximun range in array         */
    /* point = array of pointers to the helper array           */
    register uint_fast32_t *b, *s, *k;
    register uint_fast32_t exp = 0;
    register uint_fast32_t max = exp;
    uint_fast32_t i, *point[0x100];
    int swap = 0;

    /* Set preliminary according to size */
    const uint_fast32_t preliminary = (size > PRELIMINARY) ? PRELIMINARY : (size >> 3);

    /* If we found a integer with more than 24 bits in preliminar, */
    /* will have to sort all bytes either way, so max = MAX_UINT */
    LOOP_MAX(1, preliminary);
    if (max <= (MAX_UINT >> 7))
    {
        LOOP_MAX(preliminary, size);
    }

    /* Helper array initialization */
    b = (uint_fast32_t *)malloc(sizeof(uint_fast32_t) * size);

    /* Core algorithm: for a specific byte, fill the buckets array, */
    /* rearrange the array and reset the initial array accordingly. */
#define SORT_BYTE(vec, bb, shift)                                        \
    uint_fast32_t bucket[0x100] = {0};                                   \
    register unsigned char *n = (unsigned char *)(vec) + (exp >> 3), *m; \
    for (m = (unsigned char *)(&vec[size & 0xFFFFFFFC]); n < m;)         \
    {                                                                    \
        ++bucket[*n];                                                    \
        n += sizeof(int);                                                \
        ++bucket[*n];                                                    \
        n += sizeof(int);                                                \
        ++bucket[*n];                                                    \
        n += sizeof(int);                                                \
        ++bucket[*n];                                                    \
        n += sizeof(int);                                                \
    }                                                                    \
    for (n = (unsigned char *)(&vec[size & 0xFFFFFFFC]) + (exp >> 3),    \
        m = (unsigned char *)(&vec[size]);                               \
         n < m;)                                                         \
    {                                                                    \
        ++bucket[*n];                                                    \
        n += sizeof(int);                                                \
    }                                                                    \
    s = bb;                                                              \
    int next = 0;                                                        \
    for (i = 0; i < 0x100; ++i)                                          \
    {                                                                    \
        if (bucket[i] == size)                                           \
        {                                                                \
            next = 1;                                                    \
            break;                                                       \
        }                                                                \
    }                                                                    \
    if (next)                                                            \
    {                                                                    \
        exp += 8;                                                        \
        continue;                                                        \
    }                                                                    \
    for (i = 0; i < 0x100; s += bucket[i++])                             \
    {                                                                    \
        point[i] = s;                                                    \
    }                                                                    \
    for (s = vec, k = &vec[size]; s < k; ++s)                            \
    {                                                                    \
        *point[(*s shift) & 0xFF]++ = *s;                                \
    }                                                                    \
    swap = 1 - swap;                                                     \
    exp += 8;

    /* Sort each byte (if needed) */
    while (MISSING_BITS)
    {
        if (exp)
        {
            if (swap)
            {
                SORT_BYTE(b, vector, >> exp);
            }
            else
            {
                SORT_BYTE(vector, b, >> exp);
            }
        }
        else
        {
            SORT_BYTE(vector, b, );
        }
    }

    if (swap)
    {
        memcpy(vector, b, sizeof(uint_fast32_t) * size);
    }

    /* Free helper array */
    free(b);

/* Undefine function scoped macros for eventual later use */
#undef PRELIMINARY
#undef MISSING_BITS
#undef LOOP_MAX
#undef SORT_BYTE
}
