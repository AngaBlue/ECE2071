#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("%.2f\n", strtod("51.2% are admitted", NULL)); // 51.20

    puts("Enter a line of text:");
    char line[80];
    fgets(line, sizeof(line), stdin);

    puts("Enter a line of text:");
    char sentence[80];

    int c, i = 0;
    while ((i < sizeof(sentence) - 1) && (c = getchar()) != '\n')
    {
        sentence[i++] = c;
    }

    sentence[i] = '\0';

    char message[10];
    sprintf(message, "%.5f", 2.0 / 7); // 0.28571

    return 0;
}
