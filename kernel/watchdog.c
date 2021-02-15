
#include "stc89c52.h"
#include "watchdog.h"

void enable_wdt(u8 timeout, u8 idle_count)
{
	u8 value;
	assert(timeout < 8);
	value = timeout;
	if (idle_count)
		value |= BIT(3); /* idle bit */

	value |= BIT(5); /* enable bit */
	WDT_CONTR = value;
}

void feed_dog(void)
{
	SET_BIT(WDT_CONTR, 4);
}