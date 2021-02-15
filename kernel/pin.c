
#include "stc89c52.h"
#include <reg52.h>

static code struct pin_desc pin_desc[PIN_NR] = {
	{"P00", PIN_CAP_DEFAULT},
	{"P01", PIN_CAP_DEFAULT},
	{"P02", PIN_CAP_DEFAULT},
	{"P03", PIN_CAP_DEFAULT},
	{"P04", PIN_CAP_DEFAULT},
	{"P05", PIN_CAP_DEFAULT},
	{"P06", PIN_CAP_DEFAULT},
	{"P07", PIN_CAP_DEFAULT},
	{"P10", PIN_CAP_DEFAULT},
	{"P11", PIN_CAP_DEFAULT},
	{"P12", PIN_CAP_DEFAULT},
	{"P13", PIN_CAP_DEFAULT},
	{"P14", PIN_CAP_DEFAULT},
	{"P15", PIN_CAP_DEFAULT},
	{"P16", PIN_CAP_DEFAULT},
	{"P17", PIN_CAP_DEFAULT},
	{"P20", PIN_CAP_DEFAULT},
	{"P21", PIN_CAP_DEFAULT},
	{"P22", PIN_CAP_DEFAULT},
	{"P23", PIN_CAP_DEFAULT},
	{"P24", PIN_CAP_DEFAULT},
	{"P25", PIN_CAP_DEFAULT},
	{"P26", PIN_CAP_DEFAULT},
	{"P27", PIN_CAP_DEFAULT},
	{"P30", PIN_CAP_DEFAULT},
	{"P31", PIN_CAP_DEFAULT},
	{"P32", PIN_CAP_DEFAULT},
	{"P33", PIN_CAP_DEFAULT},
	{"P34", PIN_CAP_DEFAULT},
	{"P35", PIN_CAP_DEFAULT},
	{"P36", PIN_CAP_DEFAULT},
	{"P37", PIN_CAP_DEFAULT},
};

static code const char *group_name[PIN_GROUP_NR] = {
	"P0", "P1", "P2", "P3",
};

static pdata struct pin pins[PIN_NR];

#define set_pin_fn(id, fn)	  /* nothing to do */

s8 request_pin(u8 id, pin_function_t fn, bool share)
{
	u8 req_cap;
	s8 ret = 0;
	assert(id < PIN_NR);
		
	switch (fn) {
		case PIN_FN_PERIPH:
			req_cap = PIN_CAP_PERIPH;
			break;
		case PIN_CAP_INPUT:
			req_cap = PIN_CAP_INPUT;
			break;
		case PIN_CAP_OUTPUT:
			req_cap = PIN_CAP_OUTPUT;
			break;
		default:
			return -EINVAL;
	}
	if (!CAPABLE(pin_desc[id].capable, req_cap))
		return -ECAP;
	
	lock_kernel();
	if (pins[id].refcnt == 0) {
		pins[id].fn = fn;
		pins[id].refcnt = 1;
		pins[id].share = CAPABLE(pin_desc[id].capable, PIN_CAP_SHARE) && share;
		set_pin_fn(id, fn);
	}
	else if (share && pins[id].share){
		pins[id].refcnt++;
	}
	else {
		ret = -EBUSY;
	}
	unlock_kernel();

	return ret;
}

void free_pin(u8 id)
{
	assert(id < PIN_NR);
	lock_kernel();
	if (pins[id].refcnt)
		pins[id].refcnt--;
	unlock_kernel();
}

void set_pin(u8 id)
{
	u8 group, index;
	assert(id < PIN_NR);
	group = PIN_ID_TO_GROUP(id);
	index = PIN_ID_TO_INDEX(id);

	switch (group) {
		case PIN_GROUP0:
			SET_BIT(P0, index);
			break;
		case PIN_GROUP1:
			SET_BIT(P1, index);
			break;
		case PIN_GROUP2:
			SET_BIT(P2, index);
			break;
		case PIN_GROUP3:
			SET_BIT(P3, index);
			break;
	}
}

void clear_pin(u8 id)
{
	u8 group, index;
	assert(id < PIN_NR);
	group = PIN_ID_TO_GROUP(id);
	index = PIN_ID_TO_INDEX(id);

	switch (group) {
		case PIN_GROUP0:
			CLEAR_BIT(P0, index);
			break;
		case PIN_GROUP1:
			CLEAR_BIT(P1, index);
			break;
		case PIN_GROUP2:
			CLEAR_BIT(P2, index);
			break;
		case PIN_GROUP3:
			CLEAR_BIT(P3, index);
			break;
	}
}

void toggle_pin(u8 id)
{
	u8 group, index;
	assert(id < PIN_NR);
	group = PIN_ID_TO_GROUP(id);
	index = PIN_ID_TO_INDEX(id);

	switch (group) {
		case PIN_GROUP0:
			TOGGLE_BIT(P0, index);
			break;
		case PIN_GROUP1:
			TOGGLE_BIT(P1, index);
			break;
		case PIN_GROUP2:
			TOGGLE_BIT(P2, index);
			break;
		case PIN_GROUP3:
			TOGGLE_BIT(P3, index);
			break;
	}
}
u8 read_pin(u8 id)
{
	u8 group, index;
	assert(id < PIN_NR);
	group = PIN_ID_TO_GROUP(id);
	index = PIN_ID_TO_INDEX(id);

	switch (group) {
		case PIN_GROUP0:
			return TEST_BIT(P0, index);
			
		case PIN_GROUP1:
			return TEST_BIT(P1, index);
			
		case PIN_GROUP2:
			return TEST_BIT(P2, index);
			
		case PIN_GROUP3:
			return TEST_BIT(P3, index);			
	}
	return 0; /* never be here, just shut-up compiler */
}

const char *get_pin_name(u8 id)
{
	return pin_desc[id].name;
}

s8 request_pin_group(u8 group_id, pin_function_t fn)
{
	u8 index;
	s8 ret = 0;
	assert(group_id < PIN_GROUP_NR);
	for (index = 0; index < 8; ++index) {
		ret = request_pin(TO_PIN_ID(group_id, index), fn, 0);
		if (ret < 0)
			goto fail;
	}
	return 0;
fail:
	for (; index != 0; --index) {
		free_pin(TO_PIN_ID(group_id, index - 1));
	} 
	return ret;
}

void free_pin_group(u8 group_id)
{
	u8 index;	
	assert(group_id < PIN_GROUP_NR);

	for (index = 0; index < 8; ++index)
		free_pin(TO_PIN_ID(group_id, index));	
}

void set_pin_group(u8 group_id, u8 value)
{
	assert(group_id < PIN_GROUP_NR);
	switch (group_id) {
		case PIN_GROUP0:
			P0 = value;
			break;
		case PIN_GROUP1:
			P1 = value;
			break;
		case PIN_GROUP2:
			P2 = value;
			break;
		case PIN_GROUP3:
			P3 = value;
			break;
	}
}

u8 read_pin_group(u8 group_id)
{
	assert(group_id < PIN_GROUP_NR);
	switch (group_id) {
		case PIN_GROUP0:
			return P0;
			
		case PIN_GROUP1:
			return P1;
			
		case PIN_GROUP2:
			return P2;

		case PIN_GROUP3:
			return P3;
	}
	return 0;
}

void pin_init(void)
{
	u8 i;
	for (i = 0; i < PIN_NR; ++i) {
		pins[i].fn = PIN_FN_NONE;
		pins[i].refcnt = 0;
	}
}

