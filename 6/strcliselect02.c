#include	"unp.h"

// strcliselect01.c的改正版，解决批量输入的问题
// 使用read和write来避免readline、Fputs和Fgets的缓冲
void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, stdineof;
	fd_set		rset;
	char		buf[MAXLINE];
	int		n;

	stdineof = 0;
	FD_ZERO(&rset);
	for ( ; ; ) {
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			// 改用read而不是readline
			if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
				// 如果读到EOF（网络套接字没得读）时stdineof为1（没有要发送的了），返回
				if (stdineof == 1)
					return;		/* normal termination */
				// 否则就是服务端过早终止，报错
				else
					err_quit("str_cli: server terminated prematurely");
			}
			// 改用write而不是Fputs
			Write(fileno(stdout), buf, n);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			// 改用read而不是Fgets
			// 如果读到EOF，关闭描述符的写端，停止监听标准输入。设置stdineof为1而不是直接返回（若直接返回，网络套接字可能有数据还未读出）。
			if ( (n = Read(fileno(fp), buf, MAXLINE)) == 0) {
				stdineof = 1;
				Shutdown(sockfd, SHUT_WR);	/* send FIN */
				FD_CLR(fileno(fp), &rset);
				continue;
			}

			Writen(sockfd, buf, n);
		}
	}
}
