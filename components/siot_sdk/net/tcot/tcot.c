#include "tcot.h"
#include "arch_dbg.h"
#include "arch_time.h"
#include <string.h>

#undef TAG
#define TAG		"tcot"

/*============ static functions ====================*/

static int tcp_open(tcot_t *tcot);
static int tcp_write_timeout(tcot_t *tcot);
static int tcp_read_timeout(tcot_t *tcot);
static int tcp_close(tcot_t *tcot);

static void tcot_idle(tcot_t *tcot);
static void tcot_opening(tcot_t *tcot);
static void tcot_touching(tcot_t *tcot);
static void tcot_closing(tcot_t *tcot);

static void tcot_user_config(tcot_t *tcot, tcot_config_t *tcot_config);

void tcot_thread(void *arg);



static void tcot_idle(tcot_t *tcot)
{
	LOG_INFO_TAG(TAG, "tcot is idle...");

	tcot_ctrl_msg_t ctrl_msg = TCOT_CTRL_MSG_INIT_VALUE;
	if(SIOT_OK == tcot_get_ctrl_msg(tcot, &ctrl_msg, ARCH_TIME_WAIT_FOREVEAR)) {
		
		switch(ctrl_msg.type) {
		case TCOT_CRTL_OPEN :
			tcot->state = TCOT_STATE_OPENING;
			break;
		case TCOT_CRTL_CLOSE :
			tcot->state = TCOT_STATE_CLOSING;
			break;
		default :
			LOG_INFO_TAG(TAG, "invalid tcot ctrl msg[%u]", ctrl_msg.type);
			tcot->state = TCOT_STATE_IDLE;
		}

		tcot_free_ctrl_msg_arg(&ctrl_msg);
	}
}

static void tcot_opening(tcot_t *tcot)
{
	LOG_INFO_TAG(TAG, "tcot is opening...");
	tcot->state = TCOT_STATE_TOUCHING;
}

static void tcot_touching(tcot_t *tcot)
{
	LOG_INFO_TAG(TAG, "tcot is touching...");
	tcot_ctrl_msg_t ctrl_msg = TCOT_CTRL_MSG_INIT_VALUE;
	if(SIOT_OK == tcot_get_ctrl_msg(tcot, &ctrl_msg, ARCH_TIME_WAIT_FOREVEAR)) {
		
		switch(ctrl_msg.type) {
		case TCOT_CRTL_WRITE :
			break;
		case TCOT_CRTL_READ :
			break;
		default :
			break;
		}

		tcot_free_ctrl_msg_arg(&ctrl_msg);
	}
}
static void tcot_closing(tcot_t *tcot)
{
	LOG_INFO_TAG(TAG, "tcot is closing...");
}

static void tcot_user_config(tcot_t *tcot, tcot_config_t *tcot_config)
{
	if(NULL == tcot_config || NULL == tcot) {
		return;
	}

	tcot->state = tcot_config->state;
	tcot->tcp.port.port = tcot_config->port;
	tcot->tcp.port.addr = tcot_config->addr;
	tcot->tcp.read_timeout_ms = tcot_config->read_timeout_ms;
	tcot->tcp.write_timeout_ms = tcot_config->write_timeout_ms;
	tcot->up_buf = tcot_config->up_buf;
	tcot->down_buf = tcot_config->down_buf;
}



void tcot_thread(void *arg)
{
	tcot_t *tcot = (tcot_t *)arg;
	
	while(TCOT_STATE_DEAD != tcot->state) {
		switch(tcot->state) {
		case TCOT_STATE_IDLE :
			tcot_idle(tcot);
			break;
		case TCOT_STATE_OPENING :
			tcot_opening(tcot);
			break;
		case TCOT_STATE_TOUCHING :
			tcot_touching(tcot);
			break;
		case TCOT_STATE_CLOSING :
			tcot_closing(tcot);
			break;
		case TCOT_STATE_DEAD :
			goto thread_exit;
			break;
		default :
			tcot->state = TCOT_STATE_IDLE;
			break;
		}
		
	}

thread_exit :
	LOG_INFO_TAG(TAG, "tcot thread exit...");
}

/*============ public functions ====================*/

int tcot_get_ctrl_msg(tcot_t *tcot, tcot_ctrl_msg_t *ctrl_msg, uint32_t wait_time_ms)
{
	return arch_os_queue_recv_msg(tcot->queue, (void *)ctrl_msg, wait_time_ms);
}

int tcot_set_ctrl_msg(tcot_t *tcot, tcot_ctrl_msg_t *ctrl_msg, uint32_t wait_time_ms)
{
	LOG_INFO_TAG(TAG, "set ctrl msg[%u]", ctrl_msg->type);
	
	return arch_os_queue_send_msg(tcot->queue, (const void *)ctrl_msg, wait_time_ms);
}

void tcot_free_ctrl_msg_arg(tcot_ctrl_msg_t *ctrl_msg)
{
	if(NULL != ctrl_msg->arg) {
		free(ctrl_msg->arg);
		ctrl_msg->arg = NULL;
	}

	ctrl_msg->arg_len = 0;
}

int tcot_set_mode(tcot_t *tcot, tcot_ctrl_msg_type_t type, void *arg, uint32_t arg_len)
{
	int ret = SIOT_OK;
	tcot_ctrl_msg_t ctrl_msg = TCOT_CTRL_MSG_INIT_VALUE;

	ctrl_msg.type = type;

	if(NULL != arg) {
		ctrl_msg.arg = malloc(arg_len);
		if(NULL == ctrl_msg.arg) {
			goto safe_exit;
		}
		memcpy(ctrl_msg.arg, arg, arg_len);
		ctrl_msg.arg_len = arg_len;
	}

	ret = tcot_set_ctrl_msg(tcot, &ctrl_msg, TCOT_CTRL_MSG_TIMEOUT_MS);

safe_exit :
	if(NULL != ctrl_msg.arg) {
		free(ctrl_msg.arg);
	}

	return ret;
}


tcot_t* tcot_init(tcot_config_t *tcot_config)
{
	tcot_t *tcot = (tcot_t *)malloc(sizeof(tcot_t));
	if(NULL == tcot) {
		LOG_INFO_TAG(TAG, "tcot init failed");
		goto error_exit;
	}

	memset(tcot, 0, sizeof(tcot_t));

	(*tcot).state = TCOT_STATE_IDLE;
	tcot_user_config(tcot, tcot_config);
	arch_os_create_mutex(&tcot->tcp.rw_mutex);
	arch_os_create_queue(&tcot->queue, TCOT_QUEUE_LEN, sizeof(tcot_ctrl_msg_t));

	arch_os_thread_create(&tcot->pthread, "tcot_thread", tcot_thread, TCOT_THREAD_STACK_SIZE, (void *)tcot, TCOT_THREAD_PRIORITY);

	LOG_INFO_TAG(TAG, "tcot init success");
	return tcot;

error_exit :
	if(tcot) {
		free(tcot);
	}

	return NULL;
}
