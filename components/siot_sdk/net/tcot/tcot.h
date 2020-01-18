#ifndef __TCT_H__
#define __TCT_H__

#include "arch_include.h"
#include "arch_os.h"
#include "siot_define.h"

typedef enum {
	TCOT_STATE_IDLE = 0,
	TCOT_STATE_OPENING,
	TCOT_STATE_TOUCHING,
	TCOT_STATE_CLOSING,
	TCOT_STATE_DEAD
}tcot_state_t;

typedef enum {
	TCOT_CRTL_OPEN = 0,
	TCOT_CRTL_READ,
	TCOT_CRTL_WRITE,
	TCOT_CRTL_CLOSE
} tcot_ctrl_msg_type_t;

typedef struct _tcot_ctrl_msg_t {
	tcot_ctrl_msg_type_t type;
	void *arg;
	uint32_t arg_len;
} tcot_ctrl_msg_t;

#define TCOT_CTRL_MSG_INIT_VALUE	{ .type = TCOT_CRTL_CLOSE, .arg = NULL, .arg_len = 0 }

typedef struct _tcp_port_t {
	uint8_t port;
	uint32_t addr;
} tcp_port_t;

#define TCP_PORT_INIT_VALUE		\
{								\
	.port = 0,					\
	.addr = 0					\
}

typedef struct _tcot_t {

	struct tcp_t {
		int fd;
		tcp_port_t port;
		uint32_t read_timeout_ms;
		uint32_t write_timeout_ms;
		arch_os_mutex_t rw_mutex;
	}tcp;
	
	arch_os_thread_t pthread;
	arch_os_queue_t queue;
	tcot_state_t state;
	unsigned char *up_buf;
	unsigned char *down_buf;
} tcot_t;

typedef struct _tcot_config_t {
	int fd;
	uint8_t port;
	uint32_t addr;
	uint32_t read_timeout_ms;
	uint32_t write_timeout_ms;
	tcot_state_t state;
	unsigned char *up_buf;
	unsigned char *down_buf;
} tcot_config_t;

#define TCP_TIMEOUT_MS			(100)
#define TCP_INIT_VALUE					\
{										\
	.fd = -1,							\
	.port = TCP_PORT_INIT_VALUE,		\
	.read_timeout_ms = TCP_TIMEOUT_MS,	\
	.write_timeout_ms = TCP_TIMEOUT_MS,	\
	.rw_mutex = NULL					\
}

#define TCOT_INIT_VALUE			\
{								\
	.tcp = TCP_INIT_VALUE,		\
	.queue = NULL,				\
	.state = TCOT_STATE_IDLE,	\
	.up_buf = NULL,				\
	.down_buf = NULL			\
}

#define TCOT_THREAD_STACK_SIZE		(4096)
#define TCOT_THREAD_PRIORITY		(-1)
#define TCOT_QUEUE_LEN				(5)
#define TCOT_CTRL_MSG_TIMEOUT_MS	(100)


int tcot_get_ctrl_msg(tcot_t *tcot, tcot_ctrl_msg_t *ctrl_msg, uint32_t wait_time_ms);
int tcot_set_ctrl_msg(tcot_t *tcot, tcot_ctrl_msg_t *ctrl_msg, uint32_t wait_time_ms);
void tcot_free_ctrl_msg_arg(tcot_ctrl_msg_t *ctrl_msg);

tcot_t* tcot_init(tcot_config_t *tcot_config);
int tcot_set_mode(tcot_t *tcot, tcot_ctrl_msg_type_t type, void *arg, uint32_t arg_len);


#endif
