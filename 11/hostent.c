#include	"unp.h"

int
main(int argc, char **argv)
{
	char			*ptr, **pptr;
	char			str[INET_ADDRSTRLEN];
	struct hostent	*hptr;

	// 对每个命令行参数调用gethostbyname
	while (--argc > 0) {
		ptr = *++argv;
		if ( (hptr = gethostbyname(ptr)) == NULL) {
			// 调用hstrerror来解析整型值h_errno
			err_msg("gethostbyname error for host: %s: %s",
					ptr, hstrerror(h_errno));
			continue;
		}
		printf("official hostname: %s\n", hptr->h_name);
		// 遍历输出别名列表
		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("\talias: %s\n", *pptr);
		switch (hptr->h_addrtype) {
		case AF_INET:
			pptr = hptr->h_addr_list;
			// 遍历输出IP地址列表
			for ( ; *pptr != NULL; pptr++)
				printf("\taddress: %s\n",
					Inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
			break;

		default:
			err_ret("unknown address type");
			break;
		}
	}
	exit(0);
}

// official hostname: www.a.shifen.com
//     alias: www.baidu.com
//     address: 112.80.248.73
//     address: 112.80.248.74
