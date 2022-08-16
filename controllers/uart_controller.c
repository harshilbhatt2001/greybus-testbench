#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <poll.h>

#include <debug.h>
#include <greybus_protocols.h>
#include <greybus-testbench.h>
#include "uart_controller.h"

static speed_t to_speed_t(int baudrate) {

#define _case(x) case x: return B ## x
    switch(baudrate) {
        _case(0);
        _case(50);
        _case(75);
        _case(110);
        _case(134);
        _case(150);
        _case(200);
        _case(300);
        _case(600);
        _case(1200);
        _case(1800);
        _case(2400);
        _case(4800);
        _case(9600);
        _case(19200);
        _case(38400);
        _case(57600);
        _case(115200);
        _case(230400);
        _case(460800);
        _case(500000);
        _case(576000);
        _case(921600);
        _case(1000000);
        _case(1152000);
        _case(1500000);
        _case(2000000);
        _case(2500000);
        _case(3000000);
        _case(3500000);
        _case(4000000);
        default:
            pr_error("%d is not a recognized baud rate\n", baudrate);
            return -1;
    }
#undef _case
}

struct uart_controller *uart_ctrl;

int register_uart_controller(const char *file_name, int baudrate) {
    int ret;
    struct termios tio;

    speed_t speed = to_speed_t(baudrate);
    if ((speed_t) - 1 == speed ) {
        return -EINVAL;
    }

    uart_ctrl = malloc(sizeof(*uart_ctrl));
    if (uart_ctrl == NULL) {
        return -ENOMEM;
    }

    uart_ctrl->fd = open(file_name, O_RDWR | O_NOCTTY);
    if (uart_ctrl->fd < 0) {
        ret = uart_ctrl->fd;
        free(uart_ctrl);
        return ret;
    }

    cfmakeraw(&tio);
    cfsetspeed(&tio, speed);
    tio.c_cc[VMIN] = 1; // 1 character minimum
    tio.c_cc[VTIME] = 1; // 100ms timeout

    ret = tcsetattr(uart_ctrl->fd, TCSANOW, &tio);
    if (ret < 0) {
        close(uart_ctrl->fd);
        free(uart_ctrl);
        return ret;
    }

    pr_debug("Registered uart controller at %s baudrate %d\n", file_name, baudrate);

    return 0;
}

void uart_init_gb(void) {

}

int uart_controller_init(const char *file_name, int baudrate) {
    register_uart_controller(file_name, baudrate);

    uart_init_gb();
    return 0;
}

void uart_controller_exit() {
    close(uart_ctrl->fd);
    free(uart_ctrl);
}

#if 0
static int uart_write(struct connection *conn, void *data, size_t len) {
    int ret;
    size_t remaining, offset, written;

    struct uart_controller *ctrl = conn->intf2->ctrl->priv;

    cport_pack(data, conn->cport2_id);

    for(remaining = len, offset = 0, written = 0; remaining; remaining -= written, offset += written, written = 0) {
        ret = write(ctrl->fd, &((uint8_t *)data)[offset], remaining);
        if (ret < 0) {
            pr_error("%s(): write failed: %d\n", __func__, ret);
            return ret;
        }
        written = ret;
    }
    ret = len;
    return ret;
}
#endif

static int _uart_read(struct uart_controller *ctrl, void *data, size_t len) {
    int ret;
    size_t remaining, offset, recvd;

    if (!len) {
        return 0;
    }

    for (remaining = len, offset = 0, recvd = 0; remaining; remaining -= recvd, offset += recvd, recvd = 0) {
        ret = read(ctrl->fd, &((uint8_t *)data)[offset], remaining);
        if (ret < 0) {
            if (EAGAIN == errno) {
                continue;
            }
            pr_error("%s(): read failed: %d\n", __func__, ret);
            return ret;
        }
        recvd = ret;
    }
    return 0;
}

static int uart_read(void *data) {
    int ret;
    uint8_t *p_data = data;
    size_t msg_size, payload_size;

    ret = _uart_read(uart_ctrl, p_data, sizeof(struct gb_operation_msg_hdr));
    if (ret) {
        pr_error("%s(): Failed to get header\n", __func__);
        return ret;
    }

    msg_size = gb_operation_msg_size(p_data);
    payload_size = msg_size - sizeof(struct gb_operation_msg_hdr);
    p_data += sizeof(struct gb_operation_msg_hdr);

    ret = _uart_read(uart_ctrl, p_data, payload_size);
    if (ret) {
        pr_error("%s(): Failed to get payload\n", __func__);
        return ret;
    }

//    *cport_id = cport_unpack(data);

    return msg_size;
}

int uart_loop(void) {
    struct pollfd ep_poll[1];
    int ret;
    void *data = NULL;

    do {
        ep_poll[0].fd = uart_ctrl->fd;
        ep_poll->events = POLLIN | POLLHUP;

        ret = poll(ep_poll, 1, -1);
        if (ret < 0) {
            pr_error("%s(): poll", __func__);
            break;
        }

        if (ep_poll[0].revents & POLLIN) {
            ret = uart_read(data);
            if (ret < 0) {
                if (errno == EAGAIN) continue;
                goto done;
            }
        }

    } while (1);

    return 0;

done:
    return ret;
}
