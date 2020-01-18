#include "arch_net.h"
#include "arch_dbg.h"

#undef	TAG
#define TAG		"arch_net"

static EventGroupHandle_t wifi_event_group;
const int IPV4_GOTIP_BIT = BIT0;
const int IPV6_GOTIP_BIT = BIT1;

#if 0
SYSTEM_EVENT_WIFI_READY = 0,		   /**< ESP32 WiFi ready */
SYSTEM_EVENT_SCAN_DONE, 			   /**< ESP32 finish scanning AP */
SYSTEM_EVENT_STA_START, 			   /**< ESP32 station start */
SYSTEM_EVENT_STA_STOP,				   /**< ESP32 station stop */
SYSTEM_EVENT_STA_CONNECTED, 		   /**< ESP32 station connected to AP */
SYSTEM_EVENT_STA_DISCONNECTED,		   /**< ESP32 station disconnected from AP */
SYSTEM_EVENT_STA_AUTHMODE_CHANGE,	   /**< the auth mode of AP connected by ESP32 station changed */
SYSTEM_EVENT_STA_GOT_IP,			   /**< ESP32 station got IP from connected AP */
SYSTEM_EVENT_STA_LOST_IP,			   /**< ESP32 station lost IP and the IP is reset to 0 */
SYSTEM_EVENT_STA_WPS_ER_SUCCESS,	   /**< ESP32 station wps succeeds in enrollee mode */
SYSTEM_EVENT_STA_WPS_ER_FAILED, 	   /**< ESP32 station wps fails in enrollee mode */
SYSTEM_EVENT_STA_WPS_ER_TIMEOUT,	   /**< ESP32 station wps timeout in enrollee mode */
SYSTEM_EVENT_STA_WPS_ER_PIN,		   /**< ESP32 station wps pin code in enrollee mode */
SYSTEM_EVENT_AP_START,				   /**< ESP32 soft-AP start */
SYSTEM_EVENT_AP_STOP,				   /**< ESP32 soft-AP stop */
SYSTEM_EVENT_AP_STACONNECTED,		   /**< a station connected to ESP32 soft-AP */
SYSTEM_EVENT_AP_STADISCONNECTED,	   /**< a station disconnected from ESP32 soft-AP */
SYSTEM_EVENT_AP_PROBEREQRECVED, 	   /**< Receive probe request packet in soft-AP interface */
SYSTEM_EVENT_GOT_IP6,				   /**< ESP32 station or ap or ethernet interface v6IP addr is preferred */
SYSTEM_EVENT_ETH_START, 			   /**< ESP32 ethernet start */
SYSTEM_EVENT_ETH_STOP,				   /**< ESP32 ethernet stop */
SYSTEM_EVENT_ETH_CONNECTED, 		   /**< ESP32 ethernet phy link up */
SYSTEM_EVENT_ETH_DISCONNECTED,		   /**< ESP32 ethernet phy link down */
SYSTEM_EVENT_ETH_GOT_IP,			   /**< ESP32 ethernet got IP from connected AP */
SYSTEM_EVENT_MAX
#endif

static esp_err_t arch_net_event_handler(void *ctx, system_event_t *event)
{
	switch (event->event_id) {

	/* ap envent handle */
	case SYSTEM_EVENT_AP_START :
		LOG_DEBUG_TAG(TAG, "start ap mode");
		break;
	case SYSTEM_EVENT_AP_STOP :
		LOG_DEBUG_TAG(TAG, "stop ap mode");
		break;
	case SYSTEM_EVENT_AP_STACONNECTED :
		LOG_DEBUG_TAG(TAG, "sta: (%02x:%02x:%02x:%02x:%02x:%02x) joined, AID=%d",
			event->event_info.sta_connected.mac[0],
			event->event_info.sta_connected.mac[1],
			event->event_info.sta_connected.mac[2],
			event->event_info.sta_connected.mac[3],
			event->event_info.sta_connected.mac[4],
			event->event_info.sta_connected.mac[5],
			event->event_info.sta_connected.aid);
		break;
	case SYSTEM_EVENT_AP_STADISCONNECTED :
		LOG_DEBUG_TAG(TAG, "sta: (%02x:%02x:%02x:%02x:%02x:%02x) leaved, AID=%d",
			event->event_info.sta_disconnected.mac[0],
			event->event_info.sta_disconnected.mac[1],
			event->event_info.sta_disconnected.mac[2],
			event->event_info.sta_disconnected.mac[3],
			event->event_info.sta_disconnected.mac[4],
			event->event_info.sta_disconnected.mac[5],
			event->event_info.sta_disconnected.aid);
	case SYSTEM_EVENT_AP_PROBEREQRECVED :
		LOG_DEBUG_TAG(TAG, "sta: (%02x:%02x:%02x:%02x:%02x:%02x) leaved, RSSI=%d",
			event->event_info.ap_probereqrecved.mac[0],
			event->event_info.ap_probereqrecved.mac[1],
			event->event_info.ap_probereqrecved.mac[2],
			event->event_info.ap_probereqrecved.mac[3],
			event->event_info.ap_probereqrecved.mac[4],
			event->event_info.ap_probereqrecved.mac[5],
			event->event_info.ap_probereqrecved.rssi);

	case SYSTEM_EVENT_STA_START:
		esp_wifi_connect();
		LOG_DEBUG_TAG(TAG, "start sta mode");
		break;

	case SYSTEM_EVENT_STA_CONNECTED:
		LOG_DEBUG_TAG(TAG, "connected to ap");
#if 0
		/* enable ipv6 */
		tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
#endif
		break;

	case SYSTEM_EVENT_STA_GOT_IP:
		xEventGroupSetBits(wifi_event_group, IPV4_GOTIP_BIT);
		LOG_DEBUG_TAG(TAG, "got ip from ap ip["IPSTR"], mask["IPSTR"], gw["IPSTR"]",
			IP2STR(&event->event_info.got_ip.ip_info.ip),
			IP2STR(&event->event_info.got_ip.ip_info.netmask),
			IP2STR(&event->event_info.got_ip.ip_info.gw));
		break;

	case SYSTEM_EVENT_STA_DISCONNECTED:
		LOG_WARN_TAG(TAG, "disconnected from ssid[%s], reason[%d]",
				   event->event_info.disconnected.ssid,
				   event->event_info.disconnected.reason);
		/* This is a workaround as ESP32 WiFi libs don't currently auto-reassociate. */
		esp_wifi_connect();
		xEventGroupClearBits(wifi_event_group, IPV4_GOTIP_BIT);
		xEventGroupClearBits(wifi_event_group, IPV6_GOTIP_BIT);
		break;
#if 0
    case SYSTEM_EVENT_AP_STA_GOT_IP6:
        xEventGroupSetBits(wifi_event_group, IPV6_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP6");

        char *ip6 = ip6addr_ntoa(&event->event_info.got_ip6.ip6_info.ip);
        ESP_LOGI(TAG, "IPv6: %s", ip6);
#endif
    default:
        break;
    }
    return ESP_OK;
}

void arch_net_config_default(arch_net_init_config_t *net_init_config)
{
	/* config wifi as ap mode for default */
	net_init_config->mode = ARCH_NET_MODE_AP;
	memcpy(net_init_config->wifi_config.ap.ssid, AP_MODE_SSID, strlen(AP_MODE_SSID));
	net_init_config->wifi_config.ap.ssid_len = strlen(AP_MODE_SSID);
	memcpy(net_init_config->wifi_config.ap.password, AP_MODE_PASSWD, strlen(AP_MODE_PASSWD));
	net_init_config->wifi_config.ap.channel = AP_MODE_CHANNEL;
	net_init_config->wifi_config.ap.ssid_hidden = AP_MODE_SSID_HIDDEN;
	net_init_config->wifi_config.ap.max_connection = AP_MODE_MAX_CONNECTIONS;
	net_init_config->wifi_config.ap.beacon_interval = AP_MODE_BEACON_INTERVAL;
}


void arch_net_init(arch_net_init_config_t *arch_net_init_config)
{
	tcpip_adapter_init();
	wifi_event_group = xEventGroupCreate();
	esp_event_loop_init(arch_net_event_handler, NULL);
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);
	esp_wifi_set_storage(WIFI_STORAGE_RAM);

	switch(arch_net_init_config->mode) {
		
	case ARCH_NET_MODE_STA :
		LOG_INFO_TAG(TAG, "wifi set sta mode");
		esp_wifi_set_mode(WIFI_MODE_STA);
		esp_wifi_set_config(ESP_IF_WIFI_STA, &arch_net_init_config->wifi_config);
		LOG_INFO_TAG(TAG, "start to connect ssid[%s]...", arch_net_init_config->wifi_config.sta.ssid);
		esp_wifi_start();
		break;

	case ARCH_NET_MODE_AP :
		LOG_INFO_TAG(TAG, "wifi set ap mode");
		esp_wifi_set_mode(WIFI_MODE_AP);
		esp_wifi_set_config(ESP_IF_WIFI_AP, &arch_net_init_config->wifi_config);
		LOG_INFO_TAG(TAG, "init ap[%s]...", arch_net_init_config->wifi_config.ap.ssid);
		esp_wifi_start();
		break;
	
	case ARCH_NET_MODE_AP_STA :
		break;
	}
}
