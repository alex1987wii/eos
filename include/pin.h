#ifndef _EOS_PIN_H
#define _EOS_PIN_H

#include "types.h"
#include "bitops.h"

typedef enum {
	PIN_P00 = 0,
	PIN_P01,
	PIN_P02,
	PIN_P03,
	PIN_P04,
	PIN_P05,
	PIN_P06,
	PIN_P07,
	PIN_P10,
	PIN_P11,
	PIN_P12,
	PIN_P13,
	PIN_P14,
	PIN_P15,
	PIN_P16,
	PIN_P17,
	PIN_P20,
	PIN_P21,
	PIN_P22,
	PIN_P23,
	PIN_P24,
	PIN_P25,
	PIN_P26,
	PIN_P27,
	PIN_P30,
	PIN_P31,
	PIN_P32,
	PIN_P33,
	PIN_P34,
	PIN_P35,
	PIN_P36,
	PIN_P37,
}pin_t;	/* pin id as a pin object */

enum {
	PIN_GROUP0 = 0,
	PIN_GROUP1,
	PIN_GROUP2,
	PIN_GROUP3,
};

#define PIN_ID_TO_GROUP(id)		((id) / 8)
#define PIN_ID_TO_INDEX(id)		((id) % 8)
#define TO_PIN_ID(group, index)	((group) * 8 + (index))

typedef enum {
	PIN_FN_NONE = 0,
	PIN_FN_PERIPH,
	PIN_FN_INPUT,
	PIN_FN_OUTPUT,
}pin_function_t;

#define PIN_CAP_PERIPH	BIT(0)	 /* capable to periph function */
#define PIN_CAP_INPUT	BIT(1)	 /* capable to input function */
#define PIN_CAP_OUTPUT	BIT(2)	 /* capable to output function */
#define PIN_CAP_SHARE	BIT(3)	 /* capable to share pin */
#define PIN_CAP_MUX		BIT(4)	 /* capable to mux pin dynamically */
#define PIN_CAP_FULL	((PIN_CAP_PERIPH) | (PIN_CAP_INPUT) | (PIN_CAP_OUTPUT) | (PIN_CAP_SHARE) | (PIN_CAP_MUX))
#define PIN_CAP_DEFAULT	((PIN_CAP_PERIPH) | (PIN_CAP_INPUT) | (PIN_CAP_OUTPUT) | (PIN_CAP_MUX))

struct pin_desc {
	const char *name;	
	u8 capable;
};

struct pin {	
	u8 refcnt:4;
	u8 fn:2;
	u8 share:1;	
};


s8 request_pin(u8 id, pin_function_t fn, bool share);
void free_pin(u8 id);
void set_pin(u8 id);
void clear_pin(u8 id);
void toggle_pin(u8 id);
u8 read_pin(u8 id);

const char *get_pin_name(u8 id);

s8 request_pin_group(u8 group_id, pin_function_t fn); 
void free_pin_group(u8 group_id);
void set_pin_group(u8 group_id, u8 value);
u8 read_pin_group(u8 group_id);

void pin_init(void);

#endif
