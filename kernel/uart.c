
#include "stc89c52.h"
#include "uart.h"
#include "hw_timer.h"

static bit uart_rx_busy, uart_tx_busy;

#define wait_for_rx_ready()		do { while (uart_rx_busy) ;} while (0)
#define wait_for_tx_ready()		do { while (uart_tx_busy) ;} while (0)

static irq_return_t uart_irq_handler(void *private)
{
	if (RI) {
		REN = 0;  /* disable rx, let user to enable it again as needed, make SBUF(rx) saved. */
		uart_rx_busy = 0;
		RI = 0;
	} else if (TI) {
		uart_tx_busy = 0;
		TI = 0;
	} else {
		return IRQ_NONE;
	}
	return IRQ_HANDLED;	
}

void uart_send_char(u8 ch)
{
	wait_for_tx_ready();
	SBUF = ch;
}

char putchar (char ch)
{
	uart_send_char(ch);
	return ch;
}

u8 uart_recv_char(void)
{
	REN = 1; /* enable rx */
	wait_for_rx_ready();
	return SBUF;
}

char getchar (void)
{
	return uart_recv_char();
}

/* use uart MODE1 to init */
s8 uart_init(void) /* baudrate: 4800 */
{
	s8 ret;
	ret = timer1_init(TIMER_MODE2); /* init 8 bits auto load timer */
	if (ret)
		return ret;
			
	SM0 = 0;
	SM1 = 1;
	SET_BIT(PCON, 7);

	uart_rx_busy = 0;
	uart_tx_busy = 0;
	ret = request_irq(UART_IRQ, IRQ_NONE, uart_irq_handler, "uart_irq", NULL);
	if (ret)
		return ret;

	TH1 = TL1 = (256 - 13);
	enable_timer1();
	return ret;		
}