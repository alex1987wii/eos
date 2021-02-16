
#include "stc89c52.h"
#include "led.h"
#include "watchdog.h"

static pdata struct timer led_timer1, led_timer2;

void timer_handler1(void *private)
{
	led_toggle(5);
	add_timer(&led_timer1, 800);	
}
void timer_handler2(void *private)
{
	led_toggle(2);
	add_timer(&led_timer2, 500);
}
int main(void)
{
	u8 count = 0;
	clock_init();
	memory_init();
	irq_init();
	pin_init();
	timer_init();
	//uart_init();

	led_init(LED_TYPE_IO);		
	//enable_wdt(WDT_TIMEOUT_2S, 0);
	init_timer(&led_timer1, timer_handler1, NULL);
	init_timer(&led_timer2, timer_handler2, NULL);
	add_timer(&led_timer1, 800);
	add_timer(&led_timer2, 500);
			
	while (1) {
		mdelay(500);
		#if 0
		led_toggle(count & 0x7);
		//if (count < 8)
		//	feed_dog();
		
		++count;
		#endif
	}
	return 0;
}
