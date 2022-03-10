#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Function declarations
 */
char* load_digits();
void store_palindrome(unsigned int index, unsigned int length);
int compare(const void* a, const void* b);

typedef struct {
    unsigned int* distances;
    unsigned int last_index;
} Palindrome;

Palindrome* palindromes;

int main(void) {
    char *digits = load_digits();
    if (digits == NULL) {
        puts("Error loading digits from file.");
        return EXIT_FAILURE;
    }

    unsigned int length = strlen(digits);
    printf("Loaded %d characters.\n", length);

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

    int palindromes_length = sizeof(palindromes) / sizeof(Palindrome);

    for (unsigned int i = 2; i < palindromes_length; i++) {
        Palindrome* palindrome = &palindromes[i];

        // Sort the distances & find median distance
        int count = sizeof(palindrome->distances) / sizeof(int);
        qsort(palindrome->distances, count, sizeof(int), compare);
        int median_distance = palindrome->distances[count / 2];

        // Output the palindromes
        printf("%13d %13d %12d\n", i, count + 1, median_distance);
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Loads the digits from the file
 * @return The digits as a string
 */
char* load_digits()
{
    char* digits = 0;
    long length;
    FILE *file = fopen("pi_50m.txt", "rb");

    if (file) {
        // Navigate to end of file to get file length
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek (file, 0, SEEK_SET);

        // Allocate memory for file content
        digits = (char*) malloc((length + 1) * sizeof(char));
        if (digits) {
            // Read each character from file into digits
            fread(digits, sizeof(char), length, file);
        }

        // Close file
        fclose(file);
    }

    // Add NULL terminator to end string
    digits[length] = '\0';

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
    int size = sizeof(palindromes) / sizeof(Palindrome);
    if (size <= length) {
        // Allocate more space for new palindrome length
        palindromes = (Palindrome*) realloc(palindromes, (length + 1) * sizeof(Palindrome));

        for (int i = size; i <= length; i++) {
            palindromes[i].last_index = 0;
        }
    }

    // Store the palindrome
    Palindrome* palindrome = &palindromes[length];
    int distances_size = sizeof(palindrome->distances) / sizeof(unsigned int);
    palindrome->distances = (unsigned int*) realloc(palindrome->distances, (distances_size + 1) * sizeof(unsigned int));

    // If exist last index, calculate distance 
    if (palindrome->last_index != 0) {
        palindrome->distances[distances_size] = index - palindrome->last_index;
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
