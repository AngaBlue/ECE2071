#include <stdio.h>
#include <stdint.h>

int square(int x);

int main(void) {
    for (int i = 1; i <= 10; i++) {
        printf("%d\n", square(i));
    }
}

int square(int x) {
    return x * x;
}
