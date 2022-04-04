#include "GradeBook.h"

int main()
{
    GradeBook book("CS101 C++ Programming");
    book.displayMessage();

    GradeBook *gb= new GradeBook("CS101 C++ Programming");
    gb->displayMessage();
    delete gb;
    return 0;
}
