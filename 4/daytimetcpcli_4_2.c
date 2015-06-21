#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	// 新建套接字
	// AF_INET：IPv4 SOCK_STREAM：字节流
	// 第三个参数为protocol。常用协议有IPPROTO_TCP、IPPROTO_UDP、IPPROTO_SCTP、IPPROTO_TIPC等，对应TCP、UDP、STCP、TIPC。
	// type和protocol不可以随意组合，如SOCK_STREAM不可以跟IPPROTO_UDP组合。当第三个参数为0时，会自动选择第二个参数类型对应的默认协议。
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	// 置字节字符串前n个字节为零，等价于
	// memset(servaddr, 0, sizeof(servaddr))
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	// 将端口号从主机字节序转换成网络字节序
	servaddr.sin_port   = htons(55383);	/* daytime server */
	// 将IP从点分十进制转换成网络字节序
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);
	// SA -> struct sockaddr
	// 连接服务器
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");


	// 查看赋予的的ip和端口号
	struct sockaddr localaddr;
	socklen_t addrlen;
	addrlen = sizeof(localaddr);
	getsockname(sockfd, (SA *) &localaddr, &addrlen);
	printf("Listen in %s", sock_ntop(&localaddr, addrlen));
	// Listen in 127.0.0.1:55384

	
	// 读取内容并输出到stdout
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		printf("read byte: %c \n", n);
        fflush(stdout);
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);
}
