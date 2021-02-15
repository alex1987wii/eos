#ifndef _EOS_LED_H
#define _EOS_LED_H

#include "types.h"
#include "pin.h"

typedef enum {
	LED_D1 = 0,
	LED_D2,
	LED_D3,
	LED_D4,
	LED_D5,
	LED_D6,
	LED_D7,
	LED_D8,
	LED_D9,
	LED_D10,
	LED_D11,
	LED_D12,
	LED_D13,
	LED_D14,
	LED_D15,
	LED_D16,
}led_id_t;

#define LED_IO_NR	8
#define LED_HC595_NR	8
#define LED_HC595_PINS_NR	3

typedef enum {
	LED_TYPE_IO = 0,
	LED_TYPE_HC595,	
	LED_TYPE_NR,
}led_type_t;

struct hc595_led {
	pin_t RCLK; 	/* pin id for RCLK */
	pin_t SRCLK;	/* pin id for SRCLK */
	pin_t SER;		/* pin id for SER */
};

s8 led_on(led_id_t id);
s8 led_off(led_id_t id);
s8 led_toggle(led_id_t id);
s8 led_blink(led_id_t id); /* TODO */
s8 led_init(led_type_t type);
void led_exit(led_type_t type);

#endif
