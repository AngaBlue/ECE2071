#include <stdio.h>

/**
 * Expected output:
 * abc = 0
 */

int main(void) {
    int a, b, c, abc = 0;
    a = b = c = 40;

    if (b > 20) {
        int abc;
        abc = a*c;
        abc = abc + b;
    }

    printf("%d\n", abc);
    return 0;
}
