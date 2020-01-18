#ifndef __ARCH_NET_H__
#define __ARCH_NET_H__

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "esp_wifi.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "esp_wifi.h"


#include "arch_include.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ap mode configs macro define */
#define AP_MODE_SSID					"esp32"
#define AP_MODE_PASSWD					"esp32pwd"
#define AP_MODE_HOST_NAME				"esp32"
#define AP_MODE_BANDWIDTH				1
#define AP_MODE_CHANNEL					1
#define AP_MODE_SSID_HIDDEN				0
#define AP_MODE_IP						"10.10.0.1"
#define AP_MODE_GATEWAY					"10.10.0.1"
#define AP_MODE_NETMASK					"255.255.255.0"
#define AP_MODE_MAX_CONNECTIONS			4
#define AP_MODE_BEACON_INTERVAL			100

/* sta mode configs macro define */
#define STA_MODE_AP_SSID				"esp32"
#define STA_MODE_AP_PASSWD				"esp32pwd"
#define STA_MODE_POWER_SAVE 			0

typedef enum {
	ARCH_NET_MODE_AP = 0,
	ARCH_NET_MODE_STA = 1,
	ARCH_NET_MODE_AP_STA = 2,
}arch_net_mode;

typedef struct _arch_net_init_config_t {
	arch_net_mode mode;
	wifi_config_t wifi_config;
}arch_net_init_config_t;

#define ARCH_NET_INIT_CONFIG_VALUE				\
{												\
	.mode = ARCH_NET_MODE_STA					\
}

void arch_net_init(arch_net_init_config_t *net_init_config);
void arch_net_config_default(arch_net_init_config_t *net_init_config);

#ifdef __cplusplus
}
#endif

#endif /* __ARCH_NET_H__ */
