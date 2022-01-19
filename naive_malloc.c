#include "malloc.h"

/**
 * setHeader - set chunk headers
 * @chunk: chunk
 * @size: size of chunk
 * @extra: extra memory
 */
void setHeader(char *chunk, size_t size, size_t *extra)
{
	*extra -= size;
	*(size_t *)chunk = size;
	*(size_t *)(chunk + *(size_t *)chunk) = *extra;
}

/**
 * findExtra - find extra chunk block
 * @heap_start: heap start
 * @numCalls: number of times malloc has been called
 * Return: extra chunk
 */
void *findExtra(char *heap_start, size_t numCalls)
{
	while (numCalls > 0)
	{
		heap_start += *(size_t *)heap_start;
		--numCalls;
	}
	return (heap_start);
}

/**
 * extend - uses sbrk to extend break
 * @size: size
 * @extra: extra memory
 * Return: new chunk or null
 */
void *extend(size_t size, size_t *extra)
{
	void *chunk;

	chunk = sbrk(align(size, sysconf(_SC_PAGESIZE)));
	if (chunk == (void *) -1)
		return (NULL);
	*extra += align(size, sysconf(_SC_PAGESIZE));
	return (chunk);
}

/**
 * naive_malloc - naive version of malloc
 * @size: number of bytes to allocate
 * Return: new address or null
 */
void *naive_malloc(size_t size)
{
	void *chunk;
	static void *heap_start;
	static size_t numCalls;
	size_t header_size, chunkSize, extra;

	header_size = sizeof(size_t);
	chunkSize = align(size, sizeof(void *)) + header_size;
	if (!heap_start)
	{
		extra = 0;
		heap_start = chunk = extend(chunkSize, &extra);
		if (!chunk)
			return (NULL);
	}
	else
	{
		chunk = findExtra(heap_start, numCalls);
		extra = *(size_t *)chunk;
		if (extra < chunkSize + header_size)
			if (!extend(chunkSize, &extra))
				return (NULL);
	}
	setHeader(chunk, chunkSize, &extra);
	++numCalls;
	return ((char *)chunk + header_size);
}
