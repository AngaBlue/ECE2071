#include <stdio.h>
#include <math.h>

int main(void)
{
    double principal = 1000.0;
    double rate = 0.05;

    printf("%4s%10s\n", "Year", "Amount");

    for (unsigned int year = 1; year <= 10; year++)
    {
        double amount = principal * pow(1 + rate, year);

        printf("%4u%10.2f\n", year, amount);
    }
}
