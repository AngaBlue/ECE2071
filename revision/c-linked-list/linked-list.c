#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    char data;
    struct Node *next;
} Node;

typedef struct
{
    Node *tail, *head;
} LinkedList;

LinkedList list;

void create_list()
{
    char input;
    while ((input = getchar()) != '\n')
    {
        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->data = input;

        if (list.tail == NULL)
        {
            list.tail = new_node;
            list.head = new_node;
        }
        else
        {
            list.tail->next = new_node;
            list.tail = new_node;
        }
    }

    list.tail->next = NULL;
}

void print_list()
{
    Node *current = list.head;
    while (current != NULL)
    {
        printf("%c", current->data);
        current = current->next;
    }
    puts("");
}

void delete_duplicates()
{
    Node *prev = list.head;
    while (prev->next != NULL)
    {
        Node *search = list.head;
        while (search != prev->next)
        {
            if (search->data == prev->next->data)
            {
                Node *temp = prev->next;
                prev->next = prev->next->next;
                free(temp);
            }
            search = search->next;
        }
        prev = prev->next;
    }
}

void check_string()
{
    char input;
    Node *current = list.head;
    while ((input = getchar()) != '\n')
    {
        if (current == NULL || current->data != input)
        {
            puts("Your input does not match the string.");
            return;
        }
        current = current->next;
    }
    if (current == NULL)
    {
        puts("Your input matches the string.");
    }
    else
    {
        puts("Your input does not match the string.");
    }
}

void sort_data()
{
    Node *current = list.head;
    while (current->next != NULL)
    {
        Node *search = current->next;
        while (search != NULL)
        {
            if (current->data > search->data)
            {
                char temp = current->data;
                current->data = search->data;
                search->data = temp;
            }
            search = search->next;
        }
        current = current->next;
    }
}

void reverse_linked_list()
{
    Node *current = list.head;
    Node *prev = NULL;
    while (current != NULL)
    {
        Node *temp = current->next;
        current->next = prev;
        prev = current;
        current = temp;
    }
    list.head = prev;
}

int main(void)
{
    puts("Enter a string:");
    create_list();

    printf("You entered: ");
    print_list();

    delete_duplicates();
    printf("After removing duplicates: ");
    print_list();

    puts("Please enter a string to compare:");
    check_string();

    sort_data();
    puts("Sorted string:");
    print_list();

    reverse_linked_list();
    puts("Reversed string:");
    print_list();

    return 0;
}
