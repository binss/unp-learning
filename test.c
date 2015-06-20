#include <stdio.h>

int
main(int argc, char **argv)
{
    char str[128];
    str[0] = 'a';
    str[1] = 'b';
    str[2] = 0;
    printf("%s", str+1);
    return 0;
}