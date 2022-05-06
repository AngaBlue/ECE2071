#include <stdio.h>

int square(int n) {
    if (n == 1) {
        return 1;
    } else {
        return square(n - 1) + 2 * (n - 1) + 1;
    }
}

int main() {
    printf("%d\n", square(5));
}
