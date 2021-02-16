#ifndef _STC89C52_H
#define _STC89C52_H

/* standand library headers */
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <reg52.h>

/* user-defined core headers */
#include "errno.h"
#include "types.h"

#include "memory.h" /* for user-defined malloc/free */
#include "lock.h"
#include "bitops.h"
#include "clock.h"
#include "pin.h"
#include "log.h"
#include "irq.h"
#include "timer.h"
#include "power.h"
#include "led.h" /* for led_debug */

#define MIN(m, n)	((m) < (n) ? (m) : (n))
#define MAX(m, n)	((m) > (n) ? (m) : (n))

#define PIN_NR	32	/* only for IO pins */
#define PIN_GROUP_NR	4

/* sfr for stc89c52 */


#endif