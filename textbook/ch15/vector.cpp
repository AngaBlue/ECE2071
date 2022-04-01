#include <vector>
#include <iostream>
#include <stdio.h>

int main()
{
    std::vector<int> numbers(10);

    numbers.at(0) = 1;
    numbers[1] = 2;

    std::cout << "Array size: " << numbers.size() << std::endl;
    std::cout << numbers.at(0) << " " << numbers.at(1) << std::endl;

    printf("Array size: %d\n", numbers.size());

    try
    {
        numbers.at(50);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        // vector::_M_range_check: __n (which is 50) >= this->size() (which is 10)
    }

    return 0;
}
