#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "arch_time.h"

/*=================== static functions =================*/
static uint32_t arch_time_get_tick_count(void);
static void arch_time_ms2time(uint32_t ms, arch_time_t *time);

static uint32_t arch_time_get_tick_count(void)
{
	return xTaskGetTickCount();
}

static void arch_time_ms2time(uint32_t ms, arch_time_t *time)
{
	time->t = ms%1000;
	time->s = ms/1000%60;
	time->m = ms/1000/60%60;
	time->h = ms/1000/60/60%24;
}

/*==================== public functions ==================*/
void arch_time_delay_ms(uint32_t delay_ms)
{
	vTaskDelay(ARCH_TIME_MS2TICK(delay_ms));
}


uint32_t arch_time_get_ms_now(void)
{
	uint32_t ms = ARCH_TIME_TICK2MS(arch_time_get_tick_count());
	return ms;
}

void arch_time_get_time_now(arch_time_t *time)
{
	uint32_t ms = arch_time_get_ms_now();
	arch_time_ms2time(ms, time);
}

