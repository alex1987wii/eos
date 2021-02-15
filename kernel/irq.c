
#include "stc89c52.h"
#include "irq.h"


#define IRQ_NR	5

static data struct irq irqs[IRQ_NR];
static code u8 irq_priority[IRQ_NR] = {
	0, 1, 0, 1, 0,
};

static void set_irq_trigger(u8 irq, u8 flags)
{
	u8 value = 1; /* default to IRQF_TRIGGER_FALLING */
	assert(irq < IRQ_NR);

	if (flags & IRQF_TRIGGER_LOW)
		value = 0;
	
	switch (irq) {
		case 0:
			IT0 = value;
			break;		
		case 2:
			IT1 = value;
			break;
#ifdef CONFIG_EXT52
		case 6: /* EXT INT 2 */
			IT2 = value;
			break;
		case 7:	/* EXT INT 3 */
			IT3 = value;
			break;
#endif
	}	
}

static void set_irq_priority(u8 irq, u8 priority)
{
	assert(irq < IRQ_NR);
	
	switch (irq) {
		case 0:
			PX0 = priority & 1;
#ifdef CONFIG_EXT52
			PX0H = (priority >> 1) & 1;
#endif
			break;

		case 1:
			PT0 = priority & 1;
#ifdef CONFIG_EXT52
			PT0H = (priority >> 1) & 1;
#endif
			break;

		case 2:
			PX1 = priority & 1;
#ifdef CONFIG_EXT52
			PX1H = (priority >> 1) & 1;
#endif
			break;
		case 3:
			PT1 = priority & 1;
#ifdef CONFIG_EXT52
			PT1H = (priority >> 1) & 1;
#endif
			break;

		case 4:
			PS = priority & 1;
#ifdef CONFIG_EXT52
			PSH = (priority >> 1) & 1;
#endif
			break;
		
		case 5:
			PT2 = priority & 1;
#ifdef CONFIG_EXT52
			PT2H = (priority >> 1) & 1;
#endif
			break;
	}	
}

void enable_irq(u8 irq)
{
	assert(irq < IRQ_NR);
	switch (irq) {
		case 0:
			EX0 = 1;
			break;
		case 1:
			ET0 = 1;
			break;
		case 2:
			EX1 = 1;
			break;
		case 3:
			ET1 = 1;
			break;
		case 4:
			ES = 1;
			break;
	}
}

void disable_irq(u8 irq)
{
	assert(irq < IRQ_NR);
	switch (irq) {
		case 0:
			EX0 = 0;
			break;
		case 1:
			ET0 = 0;
			break;
		case 2:
			EX1 = 0;
			break;
		case 3:
			ET1 = 0;
			break;
		case 4:
			ES = 0;
			break;
	}
}

void irq0_handler(void) interrupt 0
{
	struct irq_desc *idesc = irqs[0].irq_chain;
	while (idesc) {
		if (idesc->handler(idesc->private) == IRQ_HANDLED)
			break;
		idesc = idesc->next;		
	}
}

void irq1_handler(void) interrupt 1
{
	struct irq_desc *idesc = irqs[1].irq_chain;
	while (idesc) {
		if (idesc->handler(idesc->private) == IRQ_HANDLED)
			break;
		idesc = idesc->next;		
	}
}

void irq2_handler(void) interrupt 2
{
	struct irq_desc *idesc = irqs[2].irq_chain;
	while (idesc) {
		if (idesc->handler(idesc->private) == IRQ_HANDLED)
			break;
		idesc = idesc->next;		
	}
}

void irq3_handler(void) interrupt 3
{
	struct irq_desc *idesc = irqs[3].irq_chain;
	while (idesc) {
		if (idesc->handler(idesc->private) == IRQ_HANDLED)
			break;
		idesc = idesc->next;		
	}
}

void irq4_handler(void) interrupt 4
{
	struct irq_desc *idesc = irqs[4].irq_chain;
	while (idesc) {
		if (idesc->handler(idesc->private) == IRQ_HANDLED)
			break;
		idesc = idesc->next;		
	}
}

s8 request_irq(u8 irq, u8 flags, irq_handler_t handler, const char *name, void *private)
{
	struct irq_desc *idesc;
	s8 ret = 0;
	assert(irq < IRQ_NR);
	assert(name != NULL);

	lock_kernel();
	
	if (!(irqs[irq].irq_chain) || ((flags & irqs[irq].flags & IRQF_SHARE) && ((flags & IRQF_TRIGGER_MASK) == (irqs[irq].flags & IRQF_TRIGGER_MASK)))) {
		idesc = malloc(sizeof(struct irq_desc));
		if (!idesc) {
			ret = -ENOMEM;
			goto out_nomem;
		}		
		idesc->name = name;
		idesc->handler = handler;
		idesc->private = private;

		/* add new irq_desc node to head */
		if (!(irqs[irq].irq_chain)) {			
			idesc->next = NULL;
			irqs[irq].irq_chain = idesc;
			irqs[irq].flags = flags;
			
			/* set irq trigger type */
			set_irq_trigger(irq, flags);

			/* enable irq */
			enable_irq(irq);	
		}
		else {
			idesc->next = irqs[irq].irq_chain;
			irqs[irq].irq_chain = idesc;			
		}		
	}	
	else
		ret = -EBUSY;

	unlock_kernel();
out_nomem:
	return ret;	 	
}

void free_irq(u8 irq, const char *name)
{
	struct irq_desc *idesc;
	struct irq_desc **pidesc;
	s8 ret = 0;
	assert(irq < IRQ_NR);
	assert(irq != NULL);
	
	irq_disable();
	pidesc = &irqs[irq].irq_chain;
	while (*pidesc && strcmp((*pidesc)->name, name))
		pidesc = &(*pidesc)->next;
	
	idesc = *pidesc;
	if (idesc) { /* found */
		*pidesc = idesc->next;
		free(idesc);

		if (*pidesc == NULL)
			disable_irq(irq);		
	}
	irq_enable();
}

void irq_init(void)
{
	u8 i;
	for (i = 0; i < IRQ_NR; ++i) {
		set_irq_priority(i, irq_priority[i]); 
	}
	irq_enable();
}