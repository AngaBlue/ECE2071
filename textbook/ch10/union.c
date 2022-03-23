union number
{
    int x;
    double y;
};

union number num = {.x = 5};

struct BitCard
{
    unsigned int face : 4;  // 4 bits; 0-15
    unsigned int suit : 2;  // 2 bits; 0-3
    unsigned int color : 1; // 1 bit; 0-1
};

struct Example
{
    unsigned int a : 13;
    unsigned int : 0;
    unsigned int : 4;
};
