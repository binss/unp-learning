/* include readn */
#include	"unp.h"

ssize_t						/* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = vptr;
	nleft = n;
	// 循环至读完所有字节或中断
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			// 如果该慢系统调用被一个捕获的信号中断，继续
			if (errno == EINTR)
				nread = 0;		/* and call read() again */
			else
				return(-1);
		}
		// 若到文件末尾，结束
		else if (nread == 0)
			break;				/* EOF */

		// 未读字节减少，开始位置后移
		nleft -= nread;
		ptr   += nread;
	}
	// 返回已读的字节数
	return(n - nleft);		/* return >= 0 */
}
/* end readn */

// 包裹函数
ssize_t
Readn(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = readn(fd, ptr, nbytes)) < 0)
		err_sys("readn error");
	return(n);
}
