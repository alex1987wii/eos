
#include "stc89c52.h"

static data u8 free_index;
static pdata u8 heap[HEAP_SIZE];  /* memory pool */

/* free list use index to link next free node(unsigned char), and it alway link from low to high,
 * this characteristic make it more easy to manage, but more work when free memory. */

void *malloc(u8 size)
{
	u8 max_size = 1; /* max size of chunk, initial to 1 */
	u8 index, last_free_index;

	assert(size);
	assert(!in_interrupt());  /* should not call it from interrupt context, it might cause some problem */

	++size; /* alloc a extra byte(the first byte) to stroe the chunk size */
	index = last_free_index = free_index;
	/* 1. find the node */
	while (index < HEAP_SIZE) {
		if (max_size == size) { /* already found available node */
			/* 2. delete the node from free list */
			if (last_free_index == free_index)
				free_index = heap[index];
			else
				heap[last_free_index] = heap[index];

			/* 3. return the node */
			index = heap[last_free_index];
			heap[index++] = size - 1; /* store the size allocated */
			return &heap[index];
		}
		if (heap[index] == index + 1) { /* next free node is the node after current index */
			++max_size;
		} else {
			max_size = 1;
			last_free_index = index;
		}
		index = heap[index];
	}
	/* no memory available */
	return NULL;	
}

void free(void *p)
{
	u8 index, i, j;
	if (p <= heap || p >= heap + HEAP_SIZE)  /* pointer not in heap */
		return;

	index = p - heap - 1;
	j = index + heap[index]; /* the chunk size */

	/* link the inside chunk, leave the last node to be fill */
	for (i = index; i < j; ++i)
		heap[i] = i + 1;
	
	if (index < free_index) { /* chunk is ahead of free_index */
		heap[i] = free_index;
		free_index = index;
		return;
	} 
	
	for (j = free_index; j < HEAP_SIZE; j = heap[j]) {
		if (heap[j] > index) { /* found link position */
			heap[i] = heap[j];
			heap[j] = index;
			return; 
		}
	}
	heap[i] = HEAP_SIZE;	
}

void memory_init(void)
{
	u8 i;
	for (i = 0; i < HEAP_SIZE; ++i)
		heap[i] = i + 1;

	free_index = 0;	
}