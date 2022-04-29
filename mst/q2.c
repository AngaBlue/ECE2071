#include <stdio.h>
#include <ctype.h>

int Count_and_Compare(const char** strings, int size)
{
    int alphabet_count = 0;
    int digit_count = 0;

    // For each string in the array
    for (int i = 0; i < size; i++)
    {
        int j = 0;
        char c;
        // While not null
        while ((c = strings[i][j++]) != '\0')
        {
            if (isalpha(c))
            {
                alphabet_count++;
            }
            else if (isdigit(c))
            {
                digit_count++;
            }
        }
    }

    // Compute return value
    if (digit_count > alphabet_count) return 1;
    else if (digit_count < alphabet_count) return -1;
    else return 0;
}

int main(void) {
    const char *strings[] = {"abcds", "123883"};
    printf("%d\n", Count_and_Compare(strings, sizeof(strings) / sizeof(strings[0])));
}
