#ifndef _EOS_UART_H
#define _EOS_UART_H

#include "pin.h"
#include "types.h"
#include "hw_timer.h"

#define UART_TX_PIN		PIN_P30
#define UART_RX_PIN		PIN_P31
#define UART_IRQ		4
	
typedef enum {
	B1200 = 0,
	B2400,
	B4800,
	B9600,
	B14400,
	B19200,
	B28800,
	B38400,
	B57600,
	B115200,
	B230400,
	B460800,
}uart_baudrate_t;
		
s8 uart_init(void);

void uart_send_char(u8 ch);
u8 uart_recv_char(void);

#endif
