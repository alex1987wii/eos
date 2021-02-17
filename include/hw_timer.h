#ifndef _EOS_HW_TIMER_H
#define _EOS_HW_TIMER_H

#include <reg52.h>
#include "types.h"
#include "clock.h"
#include "irq.h"

typedef enum {
	TIMER_MODE0 = 0,
	TIMER_MODE1,
	TIMER_MODE2,
	TIMER_MODE3,
}timer_mode_t;

enum {
	TIMER0 = 0,
	TIMER1,
	TIMER2,
};

#define set_timer0_counter(expire)		do { u16 value = 0xffff - expire + 1; TH0 = value >> 8; TL0 = value & 0xff; } while (0)
/* CAUTIONS: must pass variable name to this macro, and the value will store into it */
#define get_timer0_counter(value)		do { (value) = 0xffff - (TH0 << 8); ++(value); (value) -= TL0; } while (0)
		
#define enable_timer0()					(TR0 = 1)				
#define disable_timer0()				(TR0 = 0)
#define enable_timer1()					(TR1 = 1)
#define disable_timer1()				(TR1 = 0)

/* init timer0, need enable it by user */
s8 timer0_init(timer_mode_t);

/* init timer1, need enable it by user */
s8 timer1_init(timer_mode_t);

/* these function are conflict with timerx_init, should not be used for now */
s8 counter0_init(timer_mode_t);
s8 counter1_init(timer_mode_t);

#endif