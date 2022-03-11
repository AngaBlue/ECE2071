#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define FILENAME "pi_50m.txt"

/**
 * Function declarations
 */
char* load_digits(uint_fast32_t *length);
void store_palindrome(uint_fast32_t index, uint_fast32_t length);
int compare(const void* a, const void* b);
void* xmalloc(size_t size);
void* xrealloc(void* ptr, size_t size);

typedef struct {
    uint_fast32_t* distances;
    uint_fast32_t size;
    uint_fast32_t length;
    uint_fast32_t last_index;
} Palindrome;

Palindrome* palindromes;
uint_fast32_t palindromes_length = 0; 

int main(void) {
    uint_fast32_t length = 0;
    char *digits = load_digits(&length);
    if (digits == NULL) {
        puts("Error loading digits from file.");
        return EXIT_FAILURE;
    }

    printf("Loaded %s in ms.\n", FILENAME);
    puts("Finding palindromes...");	
    clock_t find_palindromes = clock();

    // Loop through the digits
    for (uint_fast32_t i = 0; i < length; i++) {
        uint_fast32_t radius = 1;
        bool odd = true;
        bool even = true;

        // Use the current digit as the middle element of the palindrome (right middle if even)
        while (odd || even) {
            int_fast32_t start = i - radius;
            // Check if start is out of bounds
            if (start < 0) break;

            // Even palindrone
            int_fast32_t end = i + radius - 1;
            if (end > length) break;
            if (digits[start] == digits[end]) {
                // Valid palindrome
                store_palindrome(i, 2 * radius);
            } else {
                even = false;
            }

            // Odd palindrone
            end++;
            if (end > length) break;
            if (digits[start] == digits[end]) {
                // Valid palindrome
                store_palindrome(i, 2 * radius + 1);
            } else {
                odd = false;
            }

            radius++;
        }
    }

    free(digits);

    printf("Found palindromes in %d ms.\n", (int)(clock() - find_palindromes) * 1000 / CLOCKS_PER_SEC);

    // Output the palindromes
    printf("%-13s %-13s %-12s\n", "Palindrome", "Number of", "Median Digit");
    printf("%-13s %-13s %-12s\n", "Length", "Palindromes", "Distance");
    printf("%-13s %-13s %-12s\n", "-----------", "-----------", "------------");

    clock_t sort_time = clock();

    for (uint_fast32_t i = 2; i < palindromes_length; i++) {
        Palindrome* palindrome = &palindromes[i];

        // Sort the distances & find median distance
        qsort(palindrome->distances, palindrome->length, sizeof(uint_fast32_t), compare);
        uint_fast32_t median_distance = palindrome->distances[palindrome->length / 2];

        // Output the palindromes
        printf("%11d %13d %14d\n", i, palindrome->length + 1, palindrome->length == 0 ? 0 : median_distance);
    }

    printf("Sorted palindromes in %d ms.\n", (int)(clock() - sort_time) * 1000 / CLOCKS_PER_SEC);

    return EXIT_SUCCESS;
}

/**
 * @brief Loads the digits from the file
 * @return The digits as a string
 */
char* load_digits(uint_fast32_t *length)
{
    char* digits = 0;
    FILE *file = fopen(FILENAME, "rb");

    if (file) {
        // Navigate to end of file to get file length
        fseek(file, 0, SEEK_END);
        *length = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Allocate memory for file content
        digits = (char*) malloc((*length + 1) * sizeof(char));
        if (digits) {
            // Read each character from file into digits
            fread(digits, sizeof(char), *length, file);
        }

        // Close file
        fclose(file);
    }

    // Add NULL terminator to end string
    digits[*length] = '\0';

    return digits;
}

/**
 * @brief Stores a palindrome of the given length and calculates the distance since the last palindrome of the same length
 * 
 * @param index Start index of the palindrome
 * @param length Length of the palindrome
 */
void store_palindrome(uint_fast32_t index, uint_fast32_t length) {
    // Check if palindrome is already stored
    if (palindromes_length <= length) {
        // Allocate more space for new palindrome length
        palindromes = (Palindrome*) xrealloc(palindromes, (length + 1) * sizeof(Palindrome));

        // Initialize new palindromes
        for (uint_fast32_t i = palindromes_length; i <= length; i++) {
            palindromes[i].distances = (uint_fast32_t*) xmalloc(sizeof(uint_fast32_t));
            palindromes[i].size = 1;
            palindromes[i].length = 0;
            palindromes[i].last_index = 0;
        }

        palindromes_length = length + 1;
    }

    // Store the palindrome
    Palindrome* palindrome = &palindromes[length];

    // Resize dynamic array
    if (palindrome->length == palindrome->size) {
        palindrome->size *= 2;
        palindrome->distances = (uint_fast32_t*) xrealloc(palindrome->distances, palindrome->size * sizeof(uint_fast32_t));
    }

    // If exist last index, calculate distance 
    if (palindrome->last_index != 0) {
        palindrome->distances[palindrome->length] = index - palindrome->last_index;
        palindrome->length++;
    }

    // Update last index 
    palindrome->last_index = index;

    return;
}

/**
 * @brief Comparison function for sorting
 */
int compare(const void* a, const void* b) {
     uint_fast32_t int_a = * ((uint_fast32_t*) a);
     int int_b = * ((uint_fast32_t*) b);

     if (int_a == int_b) return 0;
     else if (int_a < int_b) return -1;
     else return 1;
}

/**
 * @brief Safely allocates memory
 * 
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void* xmalloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        puts("Error allocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

/**
 * @brief Safely reallocates memory
 * 
 * @param ptr Pointer to the memory to reallocate
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void* xrealloc(void* ptr, size_t size) {
    ptr = realloc(ptr, size);
    if (ptr == NULL) {
        puts("Error reallocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}
