#include	"unp.h"

int
main(int argc, char **argv)
{
	// 成员共用一片存储空间（为union中占空间最大的成员大小）
	union {
	  short  s;
      char   c[sizeof(short)];
    } un;

	un.s = 0x0102;
	printf("%s: ", CPU_VENDOR_OS);
	if (sizeof(short) == 2) {
		// 高位数据存在低地址，大端
		if (un.c[0] == 1 && un.c[1] == 2)
			printf("big-endian\n");
		// 低位数据存在低地址，小端
		else if (un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknown\n");
	} else
		printf("sizeof(short) = %lu\n", sizeof(short));
     
	exit(0);
}
