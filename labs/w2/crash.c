#include <stdio.h>

int main(void) {
    int count = 5, a;
    do {
        a/=count;
    } while (count--);

    printf("%d\n", a);

    return 0;
}
