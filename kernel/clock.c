
#include "clock.h"

#define ASM_DELAY

void clock_init(void)
{
	/* TODO */
}

void udelay(u8 usecs)
{
#ifdef ASM_DELAY
#pragma asm
	MOV A, R7
	CLR C
	RRC A
	MOV R0, A
LOOP:
	DJNZ R0, LOOP
#pragma endasm
#else
	usecs = usecs >> 1;
	while (usecs--)
		;
#endif
}

/* equation: x * y * n + y * m = MCLK_PER_MS;
 * x -> inner loop times
 * y -> outer loop times
 * n -> inner loop instructions MCLK
 * m -> outer loop instructions	MCLK
 */
void mdelay(u16 msecs)
{
#ifdef ASM_DELAY
	while (msecs--) {
#pragma asm
	MOV R0,#028H
	MOV R1,#0CH
D_LOOP1:
	DJNZ R0,D_LOOP1
	MOV R0,#028H
	DJNZ R1,D_LOOP1
#pragma endasm
	} 	
#else
	u16 i;
	while (msecs--) {
		 i = 498;
		 while (i--)
		 	;
	}
#endif
}

void delay(u16 secs)
{
	while(secs--)
		mdelay(1000);
}