#ifndef __ARCH_DBG_H__
#define __ARCH_DBG_H__

#include <stdio.h>
#include <stdlib.h>

#define LOG_LEVEL_VERBOSE						0
#define LOG_LEVEL_INFO							1
#define LOG_LEVEL_WARNING						2
#define LOG_LEVEL_ERROR							3
#define LOG_LEVEL_FATAL							4
#define LOG_LEVEL_OFF							5

typedef int log_level_t;

#if MIIO_LOG_ANSI_COLOR_ENABLE
#define LOG_ANSI_COLOR_RED						"\033[0;31m"
#define LOG_ANSI_COLOR_GRE						"\033[0;32m"
#define LOG_ANSI_COLOR_YEL						"\033[0;33m"
#define LOG_ANSI_COLOR_OFF						"\033[0m"
#else
#define LOG_ANSI_COLOR_RED						NULL
#define LOG_ANSI_COLOR_GRE						NULL
#define LOG_ANSI_COLOR_YEL						NULL
#define LOG_ANSI_COLOR_OFF						NULL
#endif

#define arch_printf								printf
void arch_printf_begin(const char* ansi_color);
void arch_printf_end(const char* ansi_color);
void arch_dbg_init(void);
void arch_dump_hex(const void *data, int len,const char *tips);

#define LOG_PRINT(_fmt_, ...)					arch_printf(_fmt_"\r\n", ##__VA_ARGS__)

#ifndef MIIO_LOG_LEVEL
#define MIIO_LOG_LEVEL							LOG_LEVEL_VERBOSE
#endif
#define LOG_LEVEL 								MIIO_LOG_LEVEL

#if MIIO_LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_INFO){\
			arch_printf_begin(LOG_ANSI_COLOR_GRE);\
			arch_printf("[I] "_fmt_, ##__VA_ARGS__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)

#define LOG_INFO_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_INFO){\
			arch_printf_begin(LOG_ANSI_COLOR_GRE);\
			arch_printf("[I] %s: "_fmt_, TAG, ##__VA_ARGS__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)
#else

#define LOG_INFO(_fmt_, ...) \
	do{}while(0)

#define LOG_INFO_TAG(TAG, _fmt_, ...) \
	do{}while(0)

#endif

#if MIIO_LOG_LEVEL <= LOG_LEVEL_WARNING
#define LOG_WARN(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_WARNING){\
			arch_printf_begin(LOG_ANSI_COLOR_YEL);\
			arch_printf("[W] "_fmt_" (%s,%d)", ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)
#define LOG_WARN_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_WARNING){\
			arch_printf_begin(LOG_ANSI_COLOR_YEL);\
			arch_printf("[W] %s: "_fmt_" (%s,%d)", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)
#else
#define LOG_WARN(_fmt_, ...) \
	do{}while(0)
#define LOG_WARN_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if MIIO_LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_ERROR){\
			arch_printf_begin(LOG_ANSI_COLOR_RED);\
			arch_printf("[E] "_fmt_" (%s,%d)", ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)
#define LOG_ERROR_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_ERROR){\
			arch_printf_begin(LOG_ANSI_COLOR_RED);\
			arch_printf("[E] %s: "_fmt_" (%s,%d)", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)
#else
#define LOG_ERROR(_fmt_, ...) \
	do{}while(0)
#define LOG_ERROR_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if MIIO_LOG_LEVEL <= LOG_LEVEL_FATAL
#define LOG_FATAL(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_FATAL){\
			arch_printf_begin(LOG_ANSI_COLOR_RED);\
			arch_printf("[F] "_fmt_" (%s,%d)", ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)
#define LOG_FATAL_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_FATAL){\
			arch_printf_begin(LOG_ANSI_COLOR_RED);\
			arch_printf("[F] %s: "_fmt_" (%s,%d)", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)

#else
#define LOG_FATAL(_fmt_, ...) \
	do{}while(0)
#define LOG_FATAL_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if MIIO_LOG_LEVEL <= LOG_LEVEL_VERBOSE
#define LOG_DEBUG(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_VERBOSE){\
			arch_printf_begin(LOG_ANSI_COLOR_GRE);\
			arch_printf("[D] "_fmt_, ##__VA_ARGS__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)
#define LOG_DEBUG_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_VERBOSE){\
			arch_printf_begin(LOG_ANSI_COLOR_GRE);\
			arch_printf("[D] %s: "_fmt_, TAG, ##__VA_ARGS__);\
			arch_printf_end(LOG_ANSI_COLOR_OFF);\
		}\
	}while(0)
#else
#define LOG_DEBUG(_fmt_, ...) \
	do{}while(0)
#define LOG_DEBUG_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

// ---------------- Assertion -----------------

#define BREAKPOINT() 						while(1){}
#define ARCH_ASSERT(info, assertion)		do{if(!(assertion)){arch_printf("[ASSERT] %s.\r\n");} }while(0)


#endif  /* __ARCH_DBG_H__ */
