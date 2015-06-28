#include	"unp.h"

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE];

	for ( ; ; ) {
		len = clilen;
        // 读取下一个接收到的数据
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        // 将读取到的数据回射给原发送者
		Sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}
