#ifndef __SIOT_INSTANCE_H__
#define __SIOT_INSTANCE_H__

#include "arch_os.h"
#include "tcot.h"
#include "siot_define.h"

typedef void*			siot_handle_t;
typedef struct _siot_instanct_t {
	arch_os_mutex_t mutex;
	arch_os_queue_t queue;

	// TODO : add tcos, tcou
	tcot_t *tcot;
}siot_instance_t;

#define SIOT_INSTANCE_INIT_VALUE		\
{										\
	.mutex = NULL,						\
	.queue = NULL,						\
	.tcot = NULL,						\
}

typedef struct _siot_instance_config_t {
	// TODO : add create configs
} siot_instance_config_t;

int siot_instance_create(siot_handle_t *handle, siot_instance_config_t *cfg);
void siot_instance_destroy(siot_handle_t handle);


#endif /* __SIOT_INSTANCE_H__ */
