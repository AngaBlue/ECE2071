#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NUMBERS 100
#define WORDS 80

int *read_numbers(char *file_name, int size)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    int *numbers = (int *)malloc(size * sizeof(int));
    size_t i = 0;
    while (fscanf(file, "%d", &(numbers[i])) != EOF)
    {
        ++i;
    }

    return numbers;
}

double calc_mean(int *numbers, int size)
{
    // Sum numbers
    int sum = 0;
    for (size_t i = 0; i < size; ++i)
    {
        sum += numbers[i];
    }

    return (double)sum / size;
}

double calc_std_dev(int *numbers, int size)
{
    double mean = calc_mean(numbers, size);

    // Calculate standard deviation
    double sum = 0;
    for (size_t i = 0; i < size; ++i)
    {
        // Square difference between value and mean
        sum += (numbers[i] - mean) * (numbers[i] - mean);
    }

    return sqrt(sum / size);
}

char **read_words(char *file_name, int size)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    // Read file into string
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    rewind(file);
    char *file_data = malloc(sizeof(char) * length);
    fread(file_data, length, 1, file);

    // Split string by space into array of strings
    char **words = malloc(sizeof(char *) * size);
    const char delim = ' ';
    size_t i = 1;
    words[0] = strtok(file_data, &delim);
    char *token;
    while (token != NULL)
    {
        strcpy(words[i++], token);
        token = strtok(NULL, &delim);
    }

    return words;
}

void find_word(char **words, int size)
{
    // Take input
    char word[20];
    printf("Enter a word: ");
    scanf_s("%s", word, 20);

    for (size_t i = 0; i < size; ++i)
    {
        if (strcmp(words[i], word) == 0)
        {
            printf("%s found at index %zu\n", word, i);
            return;
        }
    }

    printf("%s not found\n", word);
}

int main()
{
    // Numbers
    int *numbers = read_numbers("List of Numbers.txt", NUMBERS);

    double mean = calc_mean(numbers, NUMBERS);
    printf("Mean: %f\n", mean);

    double std_dev = calc_std_dev(numbers, NUMBERS);
    printf("Standard Deviation: %f\n", std_dev);

    // Words
    char **words = read_words("List of Words.txt", WORDS);
    printf("%s\n", words[1]);
    find_word(words, WORDS);
}
