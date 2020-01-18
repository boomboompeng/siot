#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "arch_time.h"
#include "arch_os.h"
#include "arch_dbg.h"

log_level_t g_log_level = LOG_LEVEL_INFO;

static arch_os_mutex_t arch_dbg_print_mutex = NULL;

void arch_printf_begin(const char* ansi_color)
{
	arch_time_t time = ARCH_TIME_INIT_VALUE;
	arch_time_get_time_now(&time);

	if(arch_dbg_print_mutex) {
		arch_os_get_mutex(arch_dbg_print_mutex, ARCH_TIME_WAIT_FOREVEAR);
	}
	
    if(ansi_color) {
    	arch_printf("%s[%02u:%02u:%02u.%03u] ", ansi_color, time.h, time.m, time.s, time.t);
    }
    else {
    	arch_printf("[%02u:%02u:%02u.%03u] ", time.h, time.m, time.s, time.t);
    }
}

void arch_printf_end(const char* ansi_color)
{
	if(ansi_color)
		arch_printf("%s\r\n", ansi_color);
	else
		arch_printf("\r\n");

	if(arch_dbg_print_mutex) {
		arch_os_put_mutex(arch_dbg_print_mutex);
	}
}

void arch_dbg_init(void)
{
	uint32_t 	ret = arch_os_create_mutex(&arch_dbg_print_mutex);
	if(SIOT_OK == ret) {
		return;
	}
	else {
		arch_dbg_print_mutex = NULL;
	}
}

void arch_dump_hex(const void *data, int len,const char *tips)
{
    int i;
	char dump_buf[128]={0};
	int  dump_size=0;
	const unsigned char *data_p = data;

	arch_printf_begin(LOG_ANSI_COLOR_GRE);

    if (tips) {
    	arch_printf("%s: size=%d\r\n",tips, len);
    }

    for (i = 0; i < len; i++) {
		if (0 == ((i+1) % 16)) {
			dump_size += sprintf(dump_buf+dump_size, "0x%02x,\r\n", data_p[i]);
			dump_buf[dump_size] = '\0';
			arch_printf("%s",dump_buf);
			dump_size=0;
			continue;
		}
		else{
			dump_size += sprintf(dump_buf+dump_size, "0x%02x,", data_p[i]);
		}
	}

    if(dump_size)
    	arch_printf("%s",dump_buf);

    arch_printf_end(LOG_ANSI_COLOR_OFF);

	return;
}

