#include	"unp.h"

// 由于Unix信号默认是不排队的，所以若同时接收到多个SIGCHLD信号，该函数也可能只调用一次
void
sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

    // 所以采用while+waitpid来处理，在一次触发中处理就绪的所有子进程，WNOHANG指明非阻塞
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}
