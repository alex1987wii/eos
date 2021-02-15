#ifndef _EOS_LOG_H
#define _EOS_LOG_H

#include <stdarg.h>
#include <stdio.h>
#include "types.h"

typedef enum {
	LOG_EMG = 0,
	LOG_ERROR,
	LOG_WARN,
	LOG_NOTICE,
	LOG_INFO,
	LOG_DEBUG,
}log_level_t;

#define LOG_LEVEL_DEFAULT	LOG_NOTICE

/* use printf to log for now */
#define log_emg		printf
#define log_error	printf
#define log_warn	printf
#define log_notice	printf
#define log_info	printf
#define log_debug	printf

#if 0
void log_init(void);
s8 log(log_level_t level, const char *fmt, ...);
#endif

#endif