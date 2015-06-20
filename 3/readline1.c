/* include readline */
#include	"unp.h"

// 非常慢！
/* PAINFULLY SLOW VERSION -- example only */
// 一次一字节地读取一行文本
ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	// 循环至读完一行或中断
	for (n = 1; n < maxlen; n++) {
again:
		if ( (rc = read(fd, &c, 1)) == 1) {
			*ptr++ = c;
			// 读到换行符，结束
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} 
		// 若到文件末尾，结束
		else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} 
		else {
			// 如果该慢系统调用被一个捕获的信号中断，继续
			if (errno == EINTR)
				goto again;
			return(-1);		/* error, errno set by read() */
		}
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}
/* end readline */

// 包裹函数
ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return(n);
}
