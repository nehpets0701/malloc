#ifndef MALLOC_H
#define MALLOC_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * struct heap_info_s - stores info about heap
 * @heap_start: start of heap
 * @heap_end: end of heap
 */
typedef struct heap_info_s
{
	void *heap_start;
	void *heap_end;
} heap_info;

/**
 * struct block_info_s - stores info block
 * @prev: prev
 * @size: size
 */
typedef struct block_info_s
{
	size_t prev;
	size_t size;
} block_info;

#define align(offset, align) ((offset + (align - 1)) & -align)
#define ALIGNMENT sizeof(void *)
#define MIN_SIZE sizeof(void *)
#define PAGESIZE sysconf(_SC_PAGESIZE)
#define HDR_SZ sizeof(block_info)
#define align_up(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

#define LSB_ZERO_MASK 0xfffffffffffffffe
#define _GET_SIZE(p) (((block_info *)(p))->size)
#define _GET_PREV(p) (((block_info *)(p))->prev)
#define GET_SIZE(p) ((((block_info *)(p))->size) & LSB_ZERO_MASK)
#define GET_PREV(p) ((((block_info *)(p))->prev) & LSB_ZERO_MASK)


#endif
