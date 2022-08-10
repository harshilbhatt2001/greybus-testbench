#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/queue.h>

#include "debug.h"
#include "greybus-testbench.h"
#include "greybus_protocols.h"
#include "controller.h"
#include "controllers/uart_controller.h"

#define ES1_MSG_SIZE    (2 * 1024)

static char cport_rbuf[ES1_MSG_SIZE];
static char cport_tbuf[ES1_MSG_SIZE];

extern struct svc *svc;
extern struct uart_controller *uart_ctrl;

void cport_pack(struct gb_operation_msg_hdr *header, uint16_t cport_id) {
    header->pad[0] = cport_id;
}

void cport_clear(struct gb_operation_msg_hdr *header) {
    header->pad[0] = 0;
}

uint16_t cport_unpack(struct gb_operation_msg_hdr *header) {
    return (uint16_t)header->pad[0];
}

struct connection *_get_connection(uint16_t cport_id)
{
    struct connection *connection;
    struct interface *intf;

    TAILQ_FOREACH(intf, &svc->interfaces, intf_node)
        TAILQ_FOREACH(connection, &intf->connections, cnode)
            if (connection->hd_cport_id == cport_id)
                return connection;

    return NULL;
}

void set_connection_protocol(struct connection *conn, uint16_t cport_id) {
    int protocol_id;

    protocol_id = cport_get_protocol(conn->intf, cport_id);
    if (protocol_id < 0) {
        pr_error("%s(): fail to get protocol to cport_id: %u\n", __func__, cport_id);
    }
    pr_debug("%s(): set connection protocol %d\n", __func__, protocol_id);
}

struct connection *allocate_connection(struct interface *intf, uint16_t cport_id, uint16_t hd_cport_id) {
    pr_debug("%s(): Allocating connection on cport %d\n", __func__, cport_id);
    struct connection *conn;
    conn = calloc(1, sizeof(*conn));
    if (!conn) {
        return NULL;
    }

    conn->cport_id = cport_id;
    conn->hd_cport_id = hd_cport_id;

    TAILQ_INSERT_TAIL(&intf->connections, conn, cnode);

    if (cport_id == GB_CONTROL_CPORT_ID) {
        intf->control_conn = conn;
    }
    conn->intf = intf;
    return conn;
}

void free_connection(struct connection *conn) {
    struct interface *intf = conn->intf;

    TAILQ_REMOVE(&intf->connections, conn, cnode);
    free(conn);
}

static void get_protocol_operation(uint16_t cport_id, char **protocol, char **operation, uint8_t type) {
    struct connection *conn;
    conn = _get_connection(cport_id);
    if (!conn) {
        *protocol = "N/A";
        *operation = "N/A";
        return;
    }

    switch (conn->protocol) {
        case GREYBUS_PROTOCOL_CONTROL:
            *protocol = "CONTROL";
            *operation = control_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_SVC:
            *protocol = "SVC";
            *operation = svc_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_GPIO:
            *protocol = "GPIO";
//            *operation = gpio_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_I2C:
            *protocol = "I2C";
//            *operation = i2c_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_UART:
            *protocol = "UART";
//            *operation = uart_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_LOOPBACK:
            *protocol = "LOOPBACK";
//            *operation = loopback_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_PWM:
            *protocol = "PWM";
//            *operation = pwm_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_SDIO:
            *protocol = "SDIO";
//            *operation = sdio_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_SPI:
            *protocol = "SPI";
//            *operation = spi_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_LIGHTS:
            *protocol = "LIGHTS";
//            *operation = lights_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_POWER_SUPPLY:
            *protocol = "POWER_SUPPLY";
//            *operation = power_supply_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_BOOTROM:
            *protocol = "BOOTROM";
//            *operation = bootrom_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_FW_MANAGEMENT:
            *protocol = "fw-mgmt";
//            *operation = fw_mgmt_get_operation(type);
            break;
        case GREYBUS_PROTOCOL_FW_DOWNLOAD:
            *protocol = "fw-download";
//            *operation = fw_download_get_operation(type);
            break;
        default:
            *protocol = "(Unknown protocol)";
            *operation = "(Unknown operation)";
            break;
    }

}

static int send_msg_to_ap(uint16_t hd_cport_id, struct op_msg *msg, uint16_t msg_size,
                          uint16_t operation_id, uint8_t type, uint8_t result) {
    struct gb_operation_msg_hdr *header = &msg->header;
    char *protocol, *operation;
    ssize_t nbytes;

    header->size = htole16(msg_size);
    header->operation_id = operation_id;
    header->type = type;
    header->result = result;

    cport_pack(header, hd_cport_id);
    get_protocol_operation(hd_cport_id, &protocol, &operation, type & ~OP_RESPONSE);
    if (type & OP_RESPONSE)
        pr_debug("Module -> AP CPort %hu %s %s response\n",
                    hd_cport_id, protocol, operation);
    else
        pr_debug("Module -> AP CPort %hu %s %s request\n",
                    hd_cport_id, protocol, operation);

    /* Send the response to the AP */
    pr_dump(msg, msg_size);

    nbytes = write(uart_ctrl->fd, msg, msg_size);
    if (nbytes < 0)
        return nbytes;

    return 0;
}

int send_response(uint16_t hd_cport_id, struct op_msg *msg, uint16_t msg_size,
                  uint16_t operation_id, uint8_t type, uint8_t result) {
    return send_msg_to_ap(hd_cport_id, msg, msg_size, operation_id, type | OP_RESPONSE, result);
}

int send_request(uint16_t hd_cport_id, struct op_msg *msg, uint16_t msg_size,
                  uint16_t operation_id, uint8_t type) {
    return send_msg_to_ap(hd_cport_id, msg, msg_size, operation_id, type, 0);
}