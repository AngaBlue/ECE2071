// GradeBook.h
#include <iostream>
#include <string>

class GradeBook
{
private:
    std::string name;

public:
    GradeBook(std::string name)
    {
        this->name = name;
    }

    void setCourseName(std::string name)
    {
        this->name = name;
    }

    std::string getCourseName()
    {
        return name;
    }

    void displayMessage()
    {
        std::cout << "Welcome to the grade book for\n" << name << "!" << std::endl;
    }
};
