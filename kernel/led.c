
#include "stc89c52.h"
#include "led.h"

static code pin_t led_io_pins[LED_IO_NR] = {
	PIN_P20, PIN_P21, PIN_P22, PIN_P23,
	PIN_P24, PIN_P25, PIN_P26, PIN_P27,	
};

static code struct hc595_led hc595_led_desc = {
	PIN_P35, PIN_P36, PIN_P34
};

s8 led_on(led_id_t id)
{
	assert(id < LED_IO_NR);
	clear_pin(led_io_pins[id]);
	return 0;
}

s8 led_off(led_id_t id)
{
	assert(id < LED_IO_NR);
	set_pin(led_io_pins[id]);
	return 0;
}

s8 led_toggle(led_id_t id)
{
	assert(id < LED_IO_NR);
	toggle_pin(led_io_pins[id]);
	return 0;
}

s8 led_init(led_type_t type)
{
	s8 ret = 0;
	u8 i = 0;
	assert(type < LED_TYPE_NR);

	switch (type) {
		case LED_TYPE_IO:
			/* request pins */
			for (; i < LED_IO_NR; ++i) {
				ret = request_pin(led_io_pins[i], PIN_FN_OUTPUT, 0);
				if (ret < 0)
					goto io_pin_request_fail;
			}
			/* set default state for led */
			for (; i < LED_IO_NR; ++i)
				led_off(i);
			break;

		case LED_TYPE_HC595:
			/* request pins */
			for (; i < LED_HC595_PINS_NR; ++i) {
				ret = request_pin(hc595_led_desc.RCLK, PIN_FN_OUTPUT, 0);
				if (ret < 0)
					goto hc595_RCLK_request_fail;

				ret = request_pin(hc595_led_desc.SRCLK, PIN_FN_OUTPUT, 0);
				if (ret < 0)
					goto hc595_SRCLK_request_fail;

				ret = request_pin(hc595_led_desc.SER, PIN_FN_OUTPUT, 0);
				if (ret < 0)
					goto hc595_SER_request_fail;
			}
			/* set default state for led */
			/* TODO */
#if 0
			for (; i < LED_IO_NR; ++i)
				led_off(i);
#endif
			break;	
	}
	return ret;
io_pin_request_fail:
	for (; i != 0; --i) {
		free_pin(led_io_pins[i]);
	}
	return ret;
hc595_SER_request_fail:
	free_pin(hc595_led_desc.SRCLK);
hc595_SRCLK_request_fail:
	free_pin(hc595_led_desc.RCLK);
hc595_RCLK_request_fail:
	return ret;
}

void led_exit(led_type_t type)
{
	u8 i = 0;
	assert(type < LED_TYPE_NR);
	switch (type) {
		case LED_TYPE_IO:
			/* free pins */
			for (; i < LED_IO_NR; ++i)
				free_pin(led_io_pins[i]);		
			break;

		case LED_TYPE_HC595:
			/* free pins */
			free_pin(hc595_led_desc.RCLK);
			free_pin(hc595_led_desc.SRCLK);
			free_pin(hc595_led_desc.SER);
			break;
	}
}