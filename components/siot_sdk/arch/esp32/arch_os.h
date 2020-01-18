#ifndef __ARCH_OS_H__
#define __ARCH_OS_H__

#include "arch_include.h"

typedef void*		arch_os_mutex_t;
typedef void*		arch_os_queue_t;
typedef void*		arch_os_thread_t;
//typedef void *		arch_os_function_return_t;


void arch_os_reboot_async(uint32_t delay_reboot);
void arch_os_reboot(void);

int arch_os_create_mutex(arch_os_mutex_t *pmutex);
void arch_os_get_mutex(arch_os_mutex_t mutex, uint32_t wait_ms);
void arch_os_put_mutex(arch_os_mutex_t mutex);
void arch_os_delete_mutex(arch_os_mutex_t mutex);

int arch_os_create_queue(arch_os_queue_t *pqueue, uint32_t queue_len, uint32_t item_size);
int arch_os_queue_send_msg(arch_os_queue_t queue, const void *msg, uint32_t wait_time_ms);
int arch_os_queue_recv_msg(arch_os_queue_t queue, void *msg, uint32_t wait_time_ms);
void arch_os_delete_queue(arch_os_queue_t queue);

int arch_os_thread_create(arch_os_thread_t* pthread, const char* name, void *function, uint32_t stack_size, void* arg, int priority );


#endif
