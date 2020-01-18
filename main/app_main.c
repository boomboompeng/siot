#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nvs_flash.h"

#include "arch_dbg.h"
#include "arch_time.h"
#include "arch_net.h"
#include "tcot.h"
#include "siot_instance.h"
#include "arch_chip.h"

#undef	TAG
#define TAG		"app_main"

static siot_handle_t	siot;

void app_main()
{
	arch_chip_init();
	arch_dbg_init();

	/* init esp32 wifi */
	arch_net_init_config_t net_init_config = ARCH_NET_INIT_CONFIG_VALUE;
	arch_net_config_default(&net_init_config);
	arch_net_init(&net_init_config);

	while(1) {
		LOG_INFO_TAG(TAG, "loop...");
		arch_time_delay_ms(1000);
	}

#if 0
	/* init siot instance */
	siot_instance_create(&siot, NULL);

	while(1) {
		siot_instance_t * siot_instance = (siot_instance_t *)siot;
		tcot_t *tcot = siot_instance->tcot;
		tcot_set_mode(tcot, TCOT_CRTL_OPEN, NULL, 0);
		arch_time_delay_ms(1000);
	}
#endif
}
