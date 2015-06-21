/* include signal */
#include	"unp.h"

// 自定义signal取代sigaction，便于调用
Sigfunc *
signal(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

    // 设置信号处理函数
	act.sa_handler = func;
    // 设置阻塞信号集为空
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
    // 返回信号原有的处理函数
	return(oact.sa_handler);
}
/* end signal */

// 包裹函数
Sigfunc *
Signal(int signo, Sigfunc *func)	/* for our signal() function */
{
	Sigfunc	*sigfunc;

	if ( (sigfunc = signal(signo, func)) == SIG_ERR)
		err_sys("signal error");
	return(sigfunc);
}
