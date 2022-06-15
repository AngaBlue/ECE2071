#include <stdio.h>

int count_e(char *ArrStr[], int ArrSize)
{
    int total = 0;

    // Iterate through each string in the array
    for (int i = 0; i < ArrSize; i++)
    {
        // Iterate through each character in the string, stopping at the null terminator
        // A null character can be either 0, '\0' or NULL
        for (int j = 0; ArrStr[i][j] != 0; j++)
        {
            if (ArrStr[i][j] == 'e')
            {
                total++;
            }
        }
    }

    return total;
}

int main(void)
{
    char *strings[] = {"test", "more strings", "hello!!", "okay we're done"};
    printf("%d\n", count_e(strings, 4));
}
