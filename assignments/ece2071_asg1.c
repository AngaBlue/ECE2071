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
void load_file(const char *filename, char **content, size_t *length);

/**
 * @brief Stores a palindrome of the given length and calculates the distance since the last palindrome of the same length
 *
 * @param index Start index of the palindrome
 * @param length Length of the palindrome
 */
void store_palindrome(const size_t index, const uint_fast32_t length);

/**
 * @brief Safely allocates memory
 *
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void *xmalloc(const size_t size);

/**
 * @brief Safely reallocates memory
 *
 * @param ptr Pointer to the memory to reallocate
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void *xrealloc(void *ptr, const size_t size);

/**
 * @brief Comparison function for qsort
 *
 * @param a Pointer to the first element
 * @param b Pointer to the second element
 * @return int -1 if a < b, 0 if a == b, 1 if a > b
 */
int compare(const void *a, const void *b);

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

    // Sort palindromes
    for (size_t i = 2; i < palindromes.length; i++)
    {
        Palindrome *palindrome = &palindromes.values[i];

        qsort(palindrome->distances, palindrome->length, sizeof(uint_fast32_t), compare);
    }

    // Output the palindromes
    printf("%-13s %-13s %-12s\n", "Palindrome", "Number of", "Median Digit");
    printf("%-13s %-13s %-12s\n", "Length", "Palindromes", "Distance");
    printf("%-13s %-13s %-12s\n", "-----------", "-----------", "------------");

    for (size_t i = 2; i < palindromes.length; i++)
    {
        Palindrome *palindrome = &palindromes.values[i];

        // Find median distance
        double median_distance;
        if (palindrome->length % 2 == 0)
        {
            median_distance = (double)(palindrome->distances[palindrome->length / 2] + palindrome->distances[palindrome->length / 2 - 1]) / 2;
        }
        else
        {
            median_distance = palindrome->distances[palindrome->length / 2];
        }

        char median[14];
        if (ceil(median_distance) == median_distance)
        {
            sprintf(median, "%14i", palindrome->length == 0 ? 0 : (uint_fast32_t)median_distance);
        }
        else
        {
            sprintf(median, "%14.1f", median_distance);
        }

        // Output the palindromes
        printf("%11zi %13zi %s\n", i, palindrome->length + 1, median);
    }

    return EXIT_SUCCESS;
}

void load_file(const char *filename, char **content, size_t *length)
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

void store_palindrome(const size_t index, const uint_fast32_t length)
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

void *xrealloc(void *ptr, const size_t size)
{
    ptr = realloc(ptr, size);
    if (ptr == NULL)
    {
        puts("Error reallocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

int compare(const void *a, const void *b)
{
    return (*(uint_fast32_t *)a - *(uint_fast32_t *)b);
}
