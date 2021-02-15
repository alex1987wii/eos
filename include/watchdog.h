#ifndef _EOS_WATCHDOG_H
#define _EOS_WATCHDOG_H

enum {
	WDT_TIMEOUT_65MS = 0,
	WDT_TIMEOUT_131MS,
	WDT_TIMEOUT_262MS,
	WDT_TIMEOUT_524MS,
	WDT_TIMEOUT_1S,
	WDT_TIMEOUT_2S,
	WDT_TIMEOUT_4S,
	WDT_TIMEOUT_8S,
};

sfr WDT_CONTR	= 0xE1;
	
void enable_wdt(u8 timeout, u8 idle_count);
void feed_dog(void);

#endif