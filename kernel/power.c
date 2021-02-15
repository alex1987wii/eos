
#include "stc89c52.h"
#include "power.h"

void power_down(void)
{
	SET_BIT(PCON, 1);
}

void idle(void)
{
	SET_BIT(PCON, 0);
}

void set_reset_reason(u8 reason)
{
	u8 reg = PCON;
	assert(reason < 4);
	reg |= (reason & 0x3) << 2;	/* use GF0,GF1 to store reset reason */
	PCON = reg;	
}

u8 get_reset_reason(void)
{
	if (is_cold_start())
		return RESET_REASON_COLD;
	return (PCON >> 2) & 0x3; /* use GF0,GF1 to store reset reason */
}

void soft_reset(void)
{
	set_reset_reason(RESET_REASON_SOFT);
	SET_BIT(ISP_CONTR, 5);
}