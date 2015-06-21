#include <stdio.h>

int
main(int argc, char **argv)
{

    int a = -1627389954;
    if( a < 0 ){
        a = ~(-a)+0x01;
    }
    printf("%d\n", a);
    printf("0x%08x\n",a);

    return 0;
}