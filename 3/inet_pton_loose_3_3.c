#include    "unp.h"

int inet_pton_loose(int family, const char *strptr, void *addrptr){
    if ( family == AF_INET && inet_pton(family, strptr, addrptr) == 0 ) {
        return inet_aton(strptr, addrptr);
    }
    else if( family == AF_INET6 && inet_pton(family, strptr, addrptr) == 0 ){
        if (inet_aton(strptr, addrptr)) {
            char * ipv4_mapped_ipv6 = malloc(strlen(strptr) + 7 );
            strcpy(ipv4_mapped_ipv6, "::FFFF:");
            strcpy((ipv4_mapped_ipv6 + 7), strptr);
            // printf("%s\n", ipv4_mapped_ipv6);
            return inet_pton(family, ipv4_mapped_ipv6, addrptr);
        }
        return 0;
    }
    return -1;
}

int
main(int argc, char **argv)
{
    char *strptr = "192.168.1.1";
    char addr6str[INET6_ADDRSTRLEN];
    struct sockaddr addrptr;

    if(inet_pton_loose(AF_INET6, strptr, &addrptr)){
        inet_ntop(AF_INET6, &addrptr, addr6str, INET6_ADDRSTRLEN);
        printf("%s\n", addr6str);
    }
}