#include	"unp.h"
#include	<syslog.h>

#define	MAXFD	64

extern int	daemon_proc;	/* defined in error.c */

// 将进程转化为守护进程
int
daemon_init(const char *pname, int facility)
{
	int		i;
	pid_t	pid;

	// fork后终止父进程
	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* parent terminates */

	/* child 1 continues... */
	// 创建新会话（会话id通过setsid返回），且子进程成为新会话的首进程，从而和原来的控制终端分离
	if (setsid() < 0)			/* become session leader */
		return (-1);

	// 忽略SIGHUP（避免再次fork后的子进程因父进程的终止而终止）
	Signal(SIGHUP, SIG_IGN);
	// 再次fork后终止父进程
	// 这样做的目的是确保即使以后打开了一个终端也不会自动获得控制终端（非会话首进程不会自动获取控制终端）
	if ( (pid = Fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* child 1 terminates */


	/* child 2 continues... */
	// 告知err_XXX函数调用syslog而不是fprintf到stderr
	daemon_proc = 1;			/* for err_XXX() functions */
	// 改变工作目录为根目录
	chdir("/");				/* change working directory */
	/* close off file descriptors */
	// 关闭所有打开的文件描述符（从父进程继承来的）
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* redirect stdin, stdout, and stderr to /dev/null */
	// 打开并重定向标准输入、输出、错误到/dev/null（丢弃），避免守护进程的库函数在用到他们的时候因他们未打开而产生错误
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	// 启动syslog
	openlog(pname, LOG_PID, facility);

	return (0);				/* success */
}
