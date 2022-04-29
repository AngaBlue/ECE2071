#include <stdlib.h>
#include <string.h>

struct interface {
    int line_speed;
    float up_time;
    char interface_type[30];
};

struct Switch
{
    struct interface *If;
    char *dns_name;
    unsigned int packets_sent;
    unsigned int packets_received;
    unsigned int checksum_errors;
};

int main()
{
    char type[30] = "ethernet";
    struct interface *p = malloc(sizeof(struct interface));
    (*p).line_speed = 1e06;
    (*p).up_time = 125.67;
    strcpy(p->interface_type, type);

    struct Switch *syd_melb = malloc(sizeof(struct Switch));
    syd_melb->If = p;
    syd_melb->dns_name = malloc(sizeof(char) * 30);
    syd_melb->packets_sent = 5343677;
    syd_melb->packets_received = 393393;
    syd_melb->checksum_errors = 345;
}
