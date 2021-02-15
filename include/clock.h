#ifndef _EOS_CLOCK_H
#define _EOS_CLOCK_H

#include "types.h"

#define OSC_CLK	12000000	  /* HZ */
#define PLL		12	/* 12T */
#define MCLK	((OSC_CLK) / (PLL))
#define INSTRUCTIONS_PER_SEC	 MCLK
#define INSTRUCTIONS_PER_MS		((INSTRUCTIONS_PER_SEC) / 1000)
#define CLK_PER_MS	((OSC_CLK) / 1000)
#define MCLK_PER_MS	((CLK_PER_MS) / (PLL))

#define MS_TO_OSC_CLK(ms)	((ms) * CLK_PER_MS)
#define MS_TO_MCLK(ms)		((ms) * MCLK_PER_MS)
#define SEC_TO_OSC_CLK(sec)	((sec) * MS_TO_OSC_CLK(1000))
#define SEC_TO_MCLK(sec)	((sec) * MS_TO_MCLK(1000))
	
#define delay_1us()	_nop_()

void udelay(u8);
void mdelay(u16);
void delay(u16);

void clock_init(void);

#endif