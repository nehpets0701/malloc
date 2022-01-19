#include "malloc.h"

static heap_info heap;

/**
 * printHeap - prints heap
 */
void printHeap(void)
{
	char *p = heap.heap_start;
	size_t i = 0;

	if (!p)
		return;
	while (GET_SIZE(p) >= HDR_SZ + MIN_SIZE)
	{
		p += GET_SIZE(p);
		++i;
	}
	if (p + HDR_SZ != sbrk(0))
		exit(1);
}

/**
 * addHeader - add header info
 * @address: header address
 * @size: size block
 * @prev: previous chunk
 */
void addHeader(void *address, size_t size, size_t prev)
{
	block_info *block;

	block = address;
	block->size = prev ? size + HDR_SZ + 1 : size + HDR_SZ;
	block->prev = prev;
}

/**
 * expand - expand heap
 * @size: size
 * Return: memory block
 */
void *expand(size_t size)
{
	size_t pg_sz;
	char *p, *tmp;

	pg_sz = 2 * HDR_SZ + MIN_SIZE + size;
	pg_sz += heap.heap_start ? 0 : HDR_SZ;
	pg_sz = align_up(pg_sz, PAGESIZE);
	p = sbrk(pg_sz);
	if (p == (void *)-1 && errno == ENOMEM)
		return (NULL);
	if (heap.heap_start == NULL)
	{
		heap.heap_start = p;
		addHeader(p, size, 0);
		addHeader(p + GET_SIZE(p), pg_sz - size - 3 * HDR_SZ, 0);
		addHeader(p + pg_sz - HDR_SZ, 0, pg_sz - size - 2 * HDR_SZ);
		return (p);
	}
	else
	{
		p -= HDR_SZ;
		tmp = p;
		addHeader(p, size, GET_PREV(p));
		p += GET_SIZE(p);
		addHeader(p, pg_sz - size - 2 * HDR_SZ, 0);
		addHeader(p + GET_SIZE(p), 0, pg_sz - size - HDR_SZ);
		return (tmp);
	}
}

/**
 * findBlock - find block
 * @size: size
 * Return: memory block
 */
void *findBlock(size_t size)
{
	char *p = heap.heap_start, *tmp;
	size_t req_sz = size + HDR_SZ;

	if (!p)
		return (NULL);

	while (GET_SIZE(p) >= HDR_SZ + MIN_SIZE)
	{
		p += GET_SIZE(p);
		if ((_GET_SIZE(p) & 1) && GET_SIZE(p - GET_PREV(p)) >= req_sz)
		{
			tmp = p - GET_PREV(p);
			if (GET_SIZE(tmp) >= req_sz + HDR_SZ + MIN_SIZE)
			{
				addHeader(tmp + req_sz, GET_SIZE(tmp) - req_sz - HDR_SZ, 0);
				((block_info *)tmp)->size = req_sz;
				((block_info *)p)->prev = GET_SIZE(tmp + req_sz);
			}
			else
			{
				((block_info *)p)->size &= LSB_ZERO_MASK;
				((block_info *)p)->prev = 0;
			}

			return (tmp);
		}
	}
	return (NULL);
}

/**
 * _malloc - malloc implementation
 * @size: size
 * Return: pointer to new memory or NULL
 */
void *_malloc(size_t size)
{
	char *p;

	size = align_up(size, ALIGNMENT);
	if (size == 0)
		return (NULL);
	p = findBlock(size);
	if (!p)
		p = expand(size);
	if (!p)
		return (NULL);
	return (p + HDR_SZ);
}
