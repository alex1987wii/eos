#ifndef _EOS_TIMER_H
#define _EOS_TIMER_H

#include "clock.h"
#include "irq.h"

#define MS_TO_EXPIRE(ms)					((ms) * 1000)
#define EXPIRE_TO_MS_ROUND_DOWN(expire)		((expire) / 1000)
#define EXPIRE_ROUND_MS_ADJUST				(500)
#define HW_TIMER_EXPIRE_MAX					(32000) /* do NOT modify it */
#define TIMER_LOAD_MS_MAX					((HW_TIMER_EXPIRE_MAX) / 1000)
			


typedef void (*timer_fn_t)(void *);

struct timer {
	u16 expire;		 /* microseconds for timer */  
	timer_fn_t timer_fn;  /* timer_fn will be called in interrupt context after it expired */
	void *private;	  /* argument for timer_fn */
	struct timer *next;
};

void init_timer(struct timer *timer, timer_fn_t timer_fn, void *private);

s8 add_timer(struct timer *timer, u16 ms);
/**@brief	delete a timer from timer_list
 * @param[in]	timer	timer to delete
 * @return	if the timer is active.
 * @retval	1	timer is active before.
 * @retval	0	timer is in-active before.
 */	
s8 del_timer(struct timer *timer);
s8 mod_timer(struct timer *timer, u16 ms);

s8 timer_init(void);

#endif