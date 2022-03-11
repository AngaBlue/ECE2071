#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define FILENAME "pi_50m.txt"

typedef struct {
    uint_fast32_t* distances;
    uint_fast32_t size;
    uint_fast32_t length;
    uint_fast32_t last_index;
} Palindrome;

typedef struct {
    Palindrome* values;
    uint_fast32_t length;
} Palindromes;

/**
 * @brief Loads the digits from the file
 * @return The digits as a string
 */
void load_file(char* filename, char **content, uint_fast32_t *length);

/**
 * @brief Stores a palindrome of the given length and calculates the distance since the last palindrome of the same length
 * 
 * @param index Start index of the palindrome
 * @param length Length of the palindrome
 */
void store_palindrome(uint_fast32_t index, uint_fast32_t length);

/**
 * @brief Comparison function for sorting
 */
int compare(const void* a, const void* b);

/**
 * @brief Safely allocates memory
 * 
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void* xmalloc(size_t size);

/**
 * @brief Safely reallocates memory
 * 
 * @param ptr Pointer to the memory to reallocate
 * @param size Size of the new memory
 * @return void* Pointer to the new memory
 */
void* xrealloc(void* ptr, size_t size);

/**
 * Globals
 */
Palindromes palindromes;

int main(void) {
    uint_fast32_t length;
    char *digits;
    
    load_file(FILENAME, &digits, &length);

    if (digits == NULL) {
        puts("Error loading digits from file.");
        return EXIT_FAILURE;
    }

    puts("Finding palindromes...");	

    // Initialise palindromes
    palindromes = (Palindromes) {
        .values = (Palindrome*) xmalloc(sizeof(Palindrome)),
        .length = 0
    };

    // Loop through the digits
    for (uint_fast32_t i = 0; i < length; i++) {
        // Even Palindromes 
        int radius = 1;
        while (true) {
            int_fast32_t start = i - radius;
            int_fast32_t end = i + radius - 1;

            if (start < 0 || end > length) break;

            if (digits[start] != digits[end]) break; 

            // Valid palindrome
            store_palindrome(start, 2 * radius);
            radius++;
        }

        // Odd Palindromes 
        radius = 1;
        while (true) {
            int_fast32_t start = i - radius;
            int_fast32_t end = i + radius;

            if (start < 0 || end > length) break;

            if (digits[start] != digits[end]) break; 

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

    for (uint_fast32_t i = 2; i < palindromes.length; i++) {
        Palindrome* palindrome = &palindromes.values[i];

        // Sort the distances & find median distance
        qsort(palindrome->distances, palindrome->length, sizeof(uint_fast32_t), compare);
        uint_fast32_t median_distance = palindrome->distances[palindrome->length / 2];

        // Output the palindromes
        printf("%11d %13d %14d\n", i, palindrome->length + 1, palindrome->length == 0 ? 0 : median_distance);
    }

    return EXIT_SUCCESS;
}

void load_file(char *filename, char **content, uint_fast32_t *length)
{
    *content = 0;
    FILE *file = fopen(filename, "rb");

    if (file) {
        // Navigate to end of file to get file length
        fseek(file, 0, SEEK_END);
        *length = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Allocate memory for file content
        *content = (char*) xmalloc((*length + 1) * sizeof(char));
        if (*content) {
            // Read each character from file into digits
            fread(*content, sizeof(char), *length, file);
        }

        // Close file
        fclose(file);
    }
}

void store_palindrome(uint_fast32_t index, uint_fast32_t length) {
    // Check if palindrome is already stored
    if (palindromes.length <= length) {
        // Allocate more space for new palindrome length
        palindromes.values = (Palindrome*) xrealloc(palindromes.values, (length + 1) * sizeof(Palindrome));

        // Initialize new palindromes
        for (uint_fast32_t i = palindromes.length; i <= length; i++) {
            palindromes.values[i].distances = (uint_fast32_t*) xmalloc(sizeof(uint_fast32_t));
            palindromes.values[i].size = 1;
            palindromes.values[i].length = 0;
            palindromes.values[i].last_index = 0;
        }

        palindromes.length = length + 1;
    }

    // Store the palindrome
    Palindrome* palindrome = &palindromes.values[length];

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
}

int compare(const void* a, const void* b) {
     uint_fast32_t int_a = * ((uint_fast32_t*) a);
     int int_b = * ((uint_fast32_t*) b);

     if (int_a == int_b) return 0;
     else if (int_a < int_b) return -1;
     else return 1;
}

void* xmalloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        puts("Error allocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void* xrealloc(void* ptr, size_t size) {
    ptr = realloc(ptr, size);
    if (ptr == NULL) {
        puts("Error reallocating memory.");
        exit(EXIT_FAILURE);
    }

    return ptr;
}
