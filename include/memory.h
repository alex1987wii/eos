#ifndef _EOS_MEMORY_H
#define _EOS_MEMORY_H

#include "types.h"

/* memory manage for pdata ram, MAX_SIZE is 256 bytes */

#define HEAP_SIZE		128

void memory_init(void);
void *malloc(u8 size);
void free(void *p);

#endif
