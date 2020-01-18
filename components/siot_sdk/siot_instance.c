#include "siot_instance.h"
#include "arch_dbg.h"
#include "siot_config.h"
#include "tcot.h"

#undef	TAG
#define TAG		"siot"

int siot_instance_create(siot_handle_t *handle, siot_instance_config_t *cfg)
{
	LOG_INFO_TAG(TAG, "siot instance create...");
	
	int ret = SIOT_OK;
	if(NULL != *handle) {
		ret = SIOT_OK;
		goto success_exit;
	}

	*handle = (siot_handle_t)malloc(sizeof(siot_instance_t));
	siot_instance_t *siot = (siot_instance_t *)(*handle);
	//*siot = SIOT_INSTANCE_INIT_VALUE;

#if 0
	// TODO : set config parameters
	if(NULL == cfg) {
		LOG_WARN_TAG(TAG, "siot instance create failed");
		ret = SIOT_ERROR;
		goto failure_exit;
	}
#endif

#if SIOT_CONFIG_TCOT
	siot->tcot = tcot_init(NULL);
#endif

#ifdef SIOT_CONFIG_TCOU

#endif
	

success_exit :
	return ret;

#if 0
failure_exit :
	if(NULL != handle) {
		free(handle);
	}
	return ret;
#endif
}
void siot_instance_destroy(siot_handle_t handle)
{
	LOG_WARN_TAG(TAG, "siot instance destroy...");
	if(NULL != handle) {
		free(handle);
	}
}

