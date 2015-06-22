/* include fig01 */
#include	"unp.h"

// select服务端的实现
int
main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	// 初始化客户数组的值为-1代表空
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	// 把监听描述符加入读取集
	FD_SET(listenfd, &allset);
/* end fig01 */

/* include fig02 */
	for ( ; ; ) {
		rset = allset;		/* structure assignment */
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);
		// 如果有套接字可读
		// 如果是监听套接字可读，代表有新客户连接
		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
					Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif
			// 寻找client数组第一个空位并设置为该客户的套接字描述符
			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
			// 如果client数组已满，报错
			if (i == FD_SETSIZE)
				err_quit("too many clients");
			// 把该客户的套接字加入读取集
			FD_SET(connfd, &allset);	/* add new descriptor to set */
			// 设置检查的套接字数目（+1）
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			// 设置client中最大的描述符序号
			if (i > maxi)
				maxi = i;				/* max index in client[] array */
			// 如果没有其他描述符就绪，返回（等待下一次select返回
			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}
		// 遍历client数组（前maxi个）
		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			// 空（-1），继续
			if ( (sockfd = client[i]) < 0)
				continue;
			// 如果该描述符就绪，读取并回显写入
			if (FD_ISSET(sockfd, &rset)) {
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
						/*4connection closed by client */
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else
					Writen(sockfd, buf, n);

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
/* end fig02 */
