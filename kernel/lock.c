
#include "lock.h"

static data atomic_t lock_level = 0;


void lock_kernel(void)
{
	irq_disable();	
	++lock_level;	 
}

void unlock_kernel(void)
{
	--lock_level;
	if (lock_level == 0)
		irq_enable();
}