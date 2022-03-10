#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME "pi_50m.txt"

/**
 * Function declarations
 */
char* load_digits(unsigned long *length);
void store_palindrome(unsigned int index, unsigned int length);
int compare(const void* a, const void* b);
bool is_power_of_two(unsigned int x);

typedef struct {
    unsigned int count;
    unsigned int* distances;
    unsigned int last_index;
} Palindrome;

Palindrome* palindromes;
unsigned int palindrome_count = 0; 

int main(void) {
    unsigned long length = 0;
    char *digits = load_digits(&length);
    if (digits == NULL) {
        puts("Error loading digits from file.");
        return EXIT_FAILURE;
    }

    printf("Loaded %s.\n", FILENAME);

    // Loop through the digits
    for (unsigned int i = 0; i < length; i++) {
        int radius = 1;
        bool odd = true;
        bool even = true;

        // Use the current digit as the middle element of the palindrome (right middle if even)
        while (odd && even) {
            int start = i - radius;
            // Check if start is out of bounds
            if (start < 0) break;

            // Even palindrone
            int end = i + radius - 1;
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

    // Output the palindromes
    printf("%-13s %-13s %-12s\n", "Palindrome", "Number of", "Median Digit");
    printf("%-13s %-13s %-12s\n", "Length", "Palindromes", "Distance");
    printf("%-13s %-13s %-12s\n", "-----------", "-----------", "------------");


    for (unsigned int i = 2; i < palindrome_count; i++) {
        Palindrome* palindrome = &palindromes[i];

        // Sort the distances & find median distance
        qsort(palindrome->distances, palindrome->count, sizeof(int), compare);
        int median_distance = palindrome->distances[palindrome->count / 2];

        // Output the palindromes
        printf("%13d %13d %12d\n", i, palindrome->count + 1, median_distance);
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Loads the digits from the file
 * @return The digits as a string
 */
char* load_digits(unsigned long *length)
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
void store_palindrome(unsigned int index, unsigned int length) {
    // Check if palindrome is already stored
    if (palindrome_count <= length) {
        // Allocate more space for new palindrome length
        palindromes = (Palindrome*) realloc(palindromes, (length + 1) * sizeof(Palindrome));

        // Initialize new palindromes
        for (int i = palindrome_count; i <= length; i++) {
            palindromes[i].count = 0;
            palindromes[i].distances = (unsigned int*) malloc(sizeof(unsigned int));
            palindromes[i].last_index = 0;
        }

        palindrome_count = length + 1;
    }

    // Store the palindrome
    Palindrome* palindrome = &palindromes[length];

    // Resize dynamic array
    if (is_power_of_two(palindrome->count)) {
        palindrome->distances = (unsigned int*) realloc(palindrome->distances, (palindrome->count == 0 ? 1 : palindrome->count * 2) * sizeof(unsigned int));
    }

    // If exist last index, calculate distance 
    if (palindrome->last_index != 0) {
        palindrome->distances[palindrome->count] = index - palindrome->last_index;
        palindrome->count++;
    }

    // Update last index 
    palindrome->last_index = index;

    return;
}

/**
 * @brief Comparison function for sorting
 */
int compare(const void* a, const void* b) {
     unsigned int int_a = * ((unsigned int*) a);
     int int_b = * ((unsigned int*) b);

     if (int_a == int_b) return 0;
     else if (int_a < int_b) return -1;
     else return 1;
}

// https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
bool is_power_of_two(unsigned int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}
