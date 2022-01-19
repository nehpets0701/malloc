#include "malloc.h"

/**
 * _free - free
 * @ptr: block to free
*/
void _free(void *ptr)
{
	char *p, *next;

	if (!ptr)
		return;
	p = ptr;
	p -= HDR_SZ;
	next = p + GET_SIZE(p);
	if (_GET_SIZE(next) & 1)
	{
		putchar('\n');
		kill(getpid(), SIGABRT);
	}
	((block_info *)next)->prev = GET_SIZE(p);
	((block_info *)next)->size |= 1;
}
