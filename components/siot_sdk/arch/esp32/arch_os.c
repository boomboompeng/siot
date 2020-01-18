#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "arch_time.h"
#include "arch_os.h"
#include "arch_dbg.h"

#undef	TAG
#define	TAG		"arch_os"

int arch_os_create_mutex(arch_os_mutex_t *pmutex)
{
	*pmutex = xSemaphoreCreateMutex();
	if(NULL == *pmutex) {
		return SIOT_OK;
	}
	else {
		*pmutex = NULL;
		return SIOT_ERROR;
	}
}

void arch_os_delete_mutex(arch_os_mutex_t mutex)
{
	vSemaphoreDelete(mutex);
}

void arch_os_get_mutex(arch_os_mutex_t mutex, uint32_t wait_ms)
{
	xSemaphoreTake(mutex, ARCH_TIME_MS2TICK(wait_ms));
}

void arch_os_put_mutex(arch_os_mutex_t mutex)
{
	xSemaphoreGive(mutex);
}

int arch_os_create_queue(arch_os_queue_t *pqueue, uint32_t queue_len, uint32_t item_size)
{
	*pqueue  = xQueueCreate(queue_len, item_size);
	if(*pqueue) {
		return SIOT_OK;
	}
	else {
		return SIOT_ERROR;
	}
}

int arch_os_queue_send_msg(arch_os_queue_t queue, const void *msg, uint32_t wait_time_ms)
{
	int ret = xQueueSendToBack(queue, msg, ARCH_TIME_MS2TICK(wait_time_ms));
	return ret == pdTRUE ? SIOT_OK : SIOT_ERROR;
}

int arch_os_queue_recv_msg(arch_os_queue_t queue, void *msg, uint32_t wait_time_ms)
{
	int ret = xQueueReceive(queue, msg, ARCH_TIME_MS2TICK(wait_time_ms));
	return ret == pdTRUE ? SIOT_OK : SIOT_ERROR;
}

void arch_os_delete_queue(arch_os_queue_t queue)
{
	if(queue) {
		vQueueDelete(queue);
	}
}

int arch_os_thread_create(arch_os_thread_t* pthread, const char* name, void *function, uint32_t stack_size, void* arg, int priority )
{
	if( pdPASS == xTaskCreate( (pdTASK_CODE)function, (const char* const)name, (unsigned short) (stack_size/sizeof( portSTACK_TYPE )),
			arg, priority, pthread ) ){
		LOG_DEBUG_TAG(TAG, "create handle = %x, name = %s, prio = %d", (uint32_t)(*pthread), name, priority);
		return SIOT_OK;
	}

	LOG_ERROR_TAG(TAG, "create failed, name = %s, prio = %d", name, priority);

	return SIOT_ERROR;
}

#if 0
int arch_os_create_thread(void *pthread, void *function, const char *name, uint32_t stack_size, void *arg, int priority)
{
	int ret = xTaskCreate((TaskFunction_t)function,
							(const char *)name,
							(unsigned short)(stack_size / sizeof(portSTACK_TYPE)),
							arg,
							priority,
							pthread);

	if(pdPASS == ret) {
		LOG_DEBUG_TAG(TAG, "task[%s] create success", name);
	}
	else {
		LOG_DEBUG_TAG(TAG, "task[%s] create failed", name);
	}

	return (ret == pdPASS) ? SIOT_OK : SIOT_ERROR;
}
#endif
