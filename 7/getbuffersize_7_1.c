#include    "unp.h"

int main(int argc, char **argv){
    int fd = Socket(AF_INET, SOCK_STREAM, 0);
    int val;
    socklen_t len = sizeof(val);

    if (getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &val, &len) == -1) {
        err_ret("getsockopt error");
    } else {
        printf("TCP default receive buffer = %d\n", val);
    }
    if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &val, &len) == -1) {
        err_ret("getsockopt error");
    } else {
        printf("TCP default send buffer = %d\n", val);
    }
    fd = Socket(AF_INET, SOCK_DGRAM, 0);
    if (getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &val, &len) == -1) {
        err_ret("getsockopt error");
    } else {
        printf("UDP default receive buffer = %d\n", val);
    }
    if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &val, &len) == -1) {
        err_ret("getsockopt error");
    } else {
        printf("UDP default send buffer = %d\n", val);
    }
}

// result:
// TCP default receive buffer = 131072
// TCP default send buffer = 131072
// UDP default receive buffer = 196724
// UDP default send buffer = 9216