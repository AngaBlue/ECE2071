#include <iostream>

int sum(int a = 0, int b = 0)
{
    return a + b;
}

int global = 1;

int main()
{
    int a, b, sum;

    std::cout << "Enter first integer: ";
    std::cin >> a;

    std::cout << "Enter second integer: ";
    std::cin >> b;

    std::cout << "Sum is " << a + b + ::global << std::endl;
    return 0;
}

