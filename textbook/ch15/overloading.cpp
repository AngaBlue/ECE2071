#include <iostream>

using namespace std;

template <class T>
T square(T a)
{
    return a * a;
}

int main()
{
    int a = 1;
    float b = 1.5;
    double c = 1.75;

    cout << "Squaring numbers using overloading\n";
    cout << square(a) << " " << square(b) << " " << square(c) << endl;
}
