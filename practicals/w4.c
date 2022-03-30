#include <stdio.h>

#define FILENAME "students.txt"
#define MAX_NAME_LENGTH 20
#define MAX_CLASS_SIZE 10

typedef struct
{
    unsigned int id;
    char name[MAX_NAME_LENGTH];
} Student;

typedef struct
{
    unsigned int size;
    Student students[MAX_CLASS_SIZE];
} Class;

void read_file(Class *const class, const char *const filename);

void print_class(const Class *const class);

int main(void)
{
    Class class;
    class.size = 0;

    read_file(&class, FILENAME);

    printf("Class size: %d\n", class.size);
    print_class(&class);

    return 0;
}

void read_file(Class *const class, const char *const filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        puts("Error opening file");
        return;
    }

    while (fscanf(file, "%d %s", &class->students[class->size].id, class->students[class->size].name) != EOF)
    {
        printf("%d %s\n", class->students[class->size].id, class->students[class->size].name);
        class->size++;
    }

    fclose(file);

    return;
}

void print_class(const Class *const class)
{
    printf("%10s %20s\n", "Student ID", "Name");

    for (int i = 0; i < class->size; i++)
    {
        printf("%10d %20s\n", class->students[i].id, class->students[i].name);
    }

    return;
}
