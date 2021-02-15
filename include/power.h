#ifndef _EOS_POWER_H
#define _EOS_POWER_H

#include <reg52.h>
#include "bitops.h"
#include "types.h"

enum {
	RESET_REASON_COLD = 0,
	RESET_REASON_HW,
	RESET_REASON_SOFT,
	RESET_REASON_WDT,
	RESET_REASON_USER,
};

sfr ISP_CONTR	= 0xE7;

#define is_cold_start()		TEST_BIT(PCON, 4)

void power_down(void);
void idle(void);
void soft_reset(void);

void set_reset_reason(u8 reason);
u8 get_reset_reason(void);

#endif