struct employee
{
    char firstName[20];
    char lastName[20];
    unsigned int age;
    char gender;
    double hourlySalary;
    struct employee *teamLeader;
};

struct card
{
    char *face;
    char *suit;
};

struct card a_card = {"Queen", "Hearts"};
