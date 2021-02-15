
#include "stc89c52.h"
#include "hw_timer.h"

static data volatile struct timer *timer_list;

/* use timer0 as the hw_timer */
#define hw_timer_start()				enable_timer0()
#define hw_timer_stop()					disable_timer0()
#define hw_timer_load_expire(expire)	do {hw_timer_stop(); set_timer0_counter(expire); hw_timer_start();} while(0)
#define hw_timer_get_expire(expire)		get_timer0_counter(expire)


void init_timer(struct timer *timer, timer_fn_t timer_fn, void *private)
{
	assert(timer != NULL);
	timer->	expire = 0;
	timer->timer_fn = timer_fn;
	timer->private = private;
	timer->next = NULL;
}

static irq_return_t timer_interrupt_handler(void *private)
{
	struct timer *timer_to_load, *timer;	
	u16 hw_expire;

	if (timer_list == NULL)
		return IRQ_NONE;
	
	if (timer_list->expire > TIMER_LOAD_MS_MAX) { /* not expire yet */
		timer_list->expire -= TIMER_LOAD_MS_MAX;

		if (timer_list->expire >= TIMER_LOAD_MS_MAX)
			hw_expire = HW_TIMER_EXPIRE_MAX;
		else
			hw_expire = MS_TO_EXPIRE(timer_list->expire);

		hw_timer_load_expire(hw_expire);		
		return IRQ_HANDLED;		
	}
	/* timer expired */
	timer_list->expire = 0;
	timer_to_load = timer_list->next;
	while (timer_to_load && timer_to_load->expire == 0)  /* skip all the timer expired */
		timer_to_load = timer_to_load->next;
	
	if (timer_to_load == NULL) /* no need to reload */
		goto do_timer_call;

	/* load hw_timer as soon as possible */
	if (timer_to_load->expire >= TIMER_LOAD_MS_MAX)
		hw_expire = HW_TIMER_EXPIRE_MAX;
	else
		hw_expire = MS_TO_EXPIRE(timer_to_load->expire);
	hw_timer_load_expire(hw_expire);

do_timer_call:
	while (timer_list != timer_to_load && timer_list != NULL) { /* CAUTIONS: need think about reentrant of *timer_list*, *timer_to_load* might been deleted by timer_fn also */
		timer = timer_list;
		timer_list = timer_list->next; /* save next timer before call timer_fn, in case of timer_fn modified/released timer */
		timer->timer_fn(timer->private);
	}	
	return IRQ_HANDLED;
}
	
s8 add_timer(struct timer *timer, u16 ms)
{
	struct timer **timer_iter;
	u16 hw_remain;		/* hw_remain for timer */
	u16 expire;		/* expire for the timer */
	assert(timer);
	assert(timer->timer_fn);	

	if (ms == 0) {
		/* try to call timer_fn now */
		timer->expire = 0;						
		timer->timer_fn(timer->private);
		return 0;
	}
	/* caculate expire for timer */
	if (ms > TIMER_LOAD_MS_MAX)
		expire = HW_TIMER_EXPIRE_MAX;
	else
		expire = MS_TO_EXPIRE(ms);

	lock_kernel();

	/* find the insert position, if exsit pending expired timers, just skip them */
	timer_iter = &timer_list;
	while (*timer_iter && (*timer_iter)->expire == 0)
		timer_iter = &(*timer_iter)->next;

	if (*timer_iter == NULL) { /* no pending timers, need to reload hw_timer */
		timer->next = NULL;
		timer->expire = ms;
		*timer_iter = timer;

		hw_timer_load_expire(expire);		
		goto out;
	}

	/* found the counting timers now */
	/* adjust expire for the already counting timer */
	hw_timer_get_expire(hw_remain);
	
	if (expire < hw_remain) { /* need reload expire and insert timer */		
		timer->expire = ms;
		timer->next = *timer_iter;
		/* adjust expire after the timer */
		if ((*timer_iter)->expire > TIMER_LOAD_MS_MAX)
			(*timer_iter)->expire -= TIMER_LOAD_MS_MAX - EXPIRE_TO_MS_ROUND_DOWN(hw_remain + EXPIRE_ROUND_MS_ADJUST) + ms;
		else
			(*timer_iter)->expire = EXPIRE_TO_MS_ROUND_DOWN(hw_remain + EXPIRE_ROUND_MS_ADJUST) - ms;
		
		*timer_iter = timer;		
		hw_timer_load_expire(expire);		
		goto out;
	}
	/* no need to reload hw_timer now, *expire* used for first pending timer's ms_remain */
	if ((*timer_iter)->expire > TIMER_LOAD_MS_MAX)
		expire = (*timer_iter)->expire - TIMER_LOAD_MS_MAX + EXPIRE_TO_MS_ROUND_DOWN(hw_remain + EXPIRE_ROUND_MS_ADJUST); 
	else
		expire = EXPIRE_TO_MS_ROUND_DOWN(hw_remain + EXPIRE_ROUND_MS_ADJUST);

	if (ms < expire ) { /* if timer expires before first pending timer, need to insert front and adjust the timer's expire	*/
		/* adjust pending timer's expire */
		(*timer_iter)->expire = expire - ms;

		/* adjust timer's expire */			
		timer->expire = ms + TIMER_LOAD_MS_MAX - EXPIRE_TO_MS_ROUND_DOWN(hw_remain + EXPIRE_ROUND_MS_ADJUST);		

		/* insert timer */
		timer->next = *timer_iter;
		*timer_iter = timer;
		goto out;
	}
	/* need to insert after the pending timer, so we just skip the pending timer */
	/* adjust *ms* for the pending timer */
	ms -= expire;
	timer_iter = &(*timer_iter)->next;
	/* find the insert position */
	while (*timer_iter && ms >= (*timer_iter)->expire) {
		ms -= (*timer_iter)->expire;
		timer_iter = &(*timer_iter)->next;
	}
	/* insert timer */
	timer->expire = ms;
	timer->next = *timer_iter;
	*timer_iter = timer;
	/* adjust the expire after timer */
	if (timer->next)
		timer->next->expire -= ms;
out:
	unlock_kernel();
	return 0;		
}

/**@brief	delete a timer from timer_list
 * @param[in]	timer	timer to delete
 * @return	if the timer is active.
 * @retval	1	timer is active before.
 * @retval	0	timer is in-active before.
 */	
s8 del_timer(struct timer *timer)
{
	struct timer **timer_iter;
	u16 remain;		/* remain for timer */
	s8 ret = 0;
	assert(timer);
	assert(timer->timer_fn);	

	lock_kernel();
	
	timer_iter = &timer_list;

	/* find in the expired timers */
	while (*timer_iter && (*timer_iter)->expire == 0) {
		if ((*timer_iter) == timer) { /* found */
			/* delete it directly */			
			goto delete_out;
		}
		timer_iter = &(*timer_iter)->next;
	}

	/* find the timer in pending timers */
	if (*timer_iter == timer) {	 /* it's the counting timers */
		if (timer->next) {			
			/* adjust the expire next to timer */			
			hw_timer_get_expire(remain);	
			if (timer->expire > TIMER_LOAD_MS_MAX)
				remain = timer->expire - TIMER_LOAD_MS_MAX + EXPIRE_TO_MS_ROUND_DOWN(remain + EXPIRE_ROUND_MS_ADJUST); 
			else
				remain = EXPIRE_TO_MS_ROUND_DOWN(remain + EXPIRE_ROUND_MS_ADJUST);
			timer->next->expire += remain;
		}
		goto delete_out;
	}
	while ((*timer_iter) != timer && *timer_iter)
		timer_iter = &(*timer_iter)->next;
	
	if (*timer_iter == NULL) /* not found */
		goto out;

	/* adjust the expire next to timer, and delete it */
	if (timer->next)
		timer->next->expire += timer->expire;
			
delete_out:
	ret = 1;
	*timer_iter = timer->next;
	timer->next = NULL;	
out:
	unlock_kernel();
	return ret;
}
s8 mod_timer(struct timer *timer, u16 ms)
{
	assert(timer);
	del_timer(timer);
	return add_timer(timer, ms);
}

s8 timer_init(void)
{
	/* init timer_list, request timer irq, etc... */
	timer_list = NULL;
	timer0_init(TIMER_MODE1);

	return request_irq(1, IRQF_TRIGGER_NONE, timer_interrupt_handler, "timer_int", NULL); 
}
