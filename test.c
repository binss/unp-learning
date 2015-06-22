#include <stdio.h>
#include <unistd.h>
#include    <limits.h>
int
main(int argc, char **argv)
{

    int open_max = sysconf(_SC_OPEN_MAX);  
    printf("%d\n", open_max);
    printf("%d\n", OPEN_MAX);

    return 0;
}