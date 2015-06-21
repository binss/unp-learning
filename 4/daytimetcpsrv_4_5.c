#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	socklen_t			len;
	struct sockaddr_in	servaddr, cliaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(9999);	/* daytime server */

	// 注释掉bind的结果是套接字没有转换为被动套接字
	// Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));


	Listen(listenfd, LISTENQ);

	// 查看绑定的ip和端口号
	struct sockaddr_in localaddr;
	socklen_t addrlen;
	addrlen = sizeof(localaddr);
	getsockname(listenfd, (SA *) &localaddr, &addrlen);
	printf("Listen in %s, port %d\n", 			   
			Inet_ntop(AF_INET, &localaddr.sin_addr, buff, sizeof(buff)),
			ntohs(localaddr.sin_port));

	// Listen in 0.0.0.0, port 55353
	// 可见绑定到通配ip和随机端口上了

	for ( ; ; ) {
		len = sizeof(cliaddr);
		// 由于listenfd不是被动套接字，所以accept报错：accept error: Invalid argument
		connfd = Accept(listenfd, (SA *) &cliaddr, &len);
		printf("connection from %s, port %d\n",
			   Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			   ntohs(cliaddr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
