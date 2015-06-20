#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */
	// 绑定任意地址
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	// 转换成监听套接字，LISTENQ指定允许排队的最大连接数
	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		// 阻塞到连接到达，返回已连接的描述符
		connfd = Accept(listenfd, (SA *) NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        // Write(connfd, buff, strlen(buff));
        // 按字节输出
        for(int i=0; i<strlen(buff); i++){
        	Write(connfd, &buff[i], 1);
    	}
    	Close(connfd);
	}

}
