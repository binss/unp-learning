#include	"unp.h"
#include	<syslog.h>

extern int	daemon_proc;	/* defined in error.c */

// 用于由inetd启动的服务器程序
void
daemon_inetd(const char *pname, int facility)
{
	daemon_proc = 1;		/* for our err_XXX() functions */
	openlog(pname, LOG_PID, facility);
}
