#ifndef _EOS_IRQ_H
#define _EOS_IRQ_H

#include <intrins.h>
#include <reg52.h>
#include "types.h"

#define IRQF_TRIGGER_NONE	0
#define IRQF_TRIGGER_RISING	1
#define IRQF_TRIGGER_FALLING	2
#define IRQF_TRIGGER_HIGH	4
#define IRQF_TRIGGER_LOW	8
#define IRQF_TRIGGER_MASK	(IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_TRIGGER_HIGH | IRQF_TRIGGER_LOW)

#define IRQF_SHARE		0x80

#define IRQ_NONE		0
#define IRQ_HANDLED		1

#define irq_enable()	( EA = 1 )
#define irq_disable()	( EA = 0 )

typedef u8 irq_return_t;
typedef irq_return_t (*irq_handler_t)(void *arg);

struct irq_desc {	
	const char *name;	
	struct irq_desc *next;	
	irq_handler_t handler;
	void *private; /* argument for irq_handler */	 
};

struct irq {
	u8 flags;
	struct irq_desc *irq_chain;
};

extern bit in_irq;

#define in_interrupt()	in_irq

void irq_init(void);
void enable_irq(u8 irq);
void disable_irq(u8 irq);

s8 request_irq(u8 irq, u8 flags, irq_handler_t handler, const char *name, void *private);
void free_irq(u8 irq, const char *name);

#endif
