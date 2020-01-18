#ifndef __ARCH_TIME_H__
#define __ARCH_TIME_H__

#include "arch_include.h"

#define ARCH_TIME_INIT_VALUE	{.h = 0, .m = 0, .s = 0, .t = 0}

#define ARCH_TIME_TICK2MS(tick)		((tick)*portTICK_PERIOD_MS)
#define ARCH_TIME_MS2TICK(ms)		((ms)/portTICK_PERIOD_MS)

#define ARCH_TIME_WAIT_FOREVEAR		0xFFFFFFFF

typedef struct _arch_time_t {
	uint8_t h;
	uint8_t m;
	uint8_t s;
	uint16_t t;
}arch_time_t;

void arch_time_delay_ms(uint32_t delay_ms);
uint32_t arch_time_get_ms_now(void);
void arch_time_get_time_now(arch_time_t *time);


#endif
