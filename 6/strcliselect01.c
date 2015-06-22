#include	"unp.h"

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	// 初始化描述符集为全0
	FD_ZERO(&rset);
	for ( ; ; ) {
		// 设置关注的描述符，包括标准输入和网络套接字
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		// maxfdp1只是设置数目，测试从0到maxfdp1-1，因此需要+1
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		// 只关注可读
		Select(maxfdp1, &rset, NULL, NULL, NULL);
		// select返回
		// 如果是套接字可读，读取数据并写到标准输出
		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		}
		// 如果是标准输入可读，读取数据并写到套接字
		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
