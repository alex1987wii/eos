
#include "stc89c52.h"
#include "hw_timer.h"

s8 timer0_init(timer_mode_t mode)
{
	TMOD &= 0xf0;
	TMOD |= mode & 0x3;
	TMOD |= BIT(3);
	return 0;
}

s8 timer1_init(timer_mode_t mode)
{
	TMOD &= 0xf;
	TMOD |= (mode & 0x3) << 4;
	TMOD |= BIT(7);
	return 0;
}



