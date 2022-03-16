#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // Memory allocation
    puts("Memory Allocations\n");
    char a, b, c, *p, *q, *r;

    printf("%p\n%p\n%p\n%p\n%p\n%p\n", &a, &b, &c, &p, &q, &r);

    // Array linear search
    puts("\n\nArray Linear Search");
    int array[] = {3, 2, 10, 4, 5};
    int length = sizeof(array) / sizeof(int);


    int largest_value = array[0];
    for (int i = 0; i < length; i++)
    {
        if (array[i] > largest_value)
        {
            largest_value = array[i];
        }
    }

    printf("Largest value: %d\n", largest_value);

    // Array mean
    puts("\n\nArray Mean");

    int total = 0;
    for (int i = 0; i < length; i++)
    {
            total += array[i];
    }

    printf("Array mean: %lf\n", (double) total / sizeof(array));

    // Array reverse
    puts("\n\nArray Reverse");

    for(int i = i; i<length/2; i++)
    {
        int temp = array[i];
        array[i] = array[length - i - 1];
        array[length - i - 1] = temp;        
    }

    // Print array
    for (int i = 0; i < length; i++)
    {
        printf("%d ", array[i]);
    }
    puts("");

    return 0;
}
