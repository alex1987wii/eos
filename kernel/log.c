
#include "log.h"

#if 0
static s8 emmit_char(u8 ch)
{
	return uart_send(ch);
}

s8 log(log_level_t level, const char *fmt, ...)
{
	s8 ret = 0;
	va_list ap;
	va_start(ap, fmt);
	//ret = vsprintf
	va_end(ap);
	return ret;
}
#endif