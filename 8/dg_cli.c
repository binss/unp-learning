#include	"unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

    // 每当从标准输入读入文本行
	while (Fgets(sendline, MAXLINE, fp) != NULL) {
        // 发送文本行
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        // 读取套接字（服务端），不关心发送者信息
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        // 添加结束符
		recvline[n] = 0;	/* null terminate */
        // 输出文本行
		Fputs(recvline, stdout);
	}
}
