#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    int v = 0;
    if (argc > 1) {
        v = atoi(argv[1]);
    }
    char *old, *new;
    old = sbrk(0);
    printf("before sbrk: %p\n", old);
    sbrk(v);
    new = sbrk(0);
    printf("after  sbrk: %p\n", new);
    exit(0);
}