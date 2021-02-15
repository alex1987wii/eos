#ifndef _EOS_LOCK_H
#define _EOS_LOCK_H

#include "irq.h"

typedef volatile s8 atomic_t;

struct spin_lock { 
	atomic_t lock;	
};

void lock_kernel(void);
void unlock_kernel(void);

#endif