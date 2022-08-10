#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#include <stdint.h>
#include <sys/queue.h>
#include <stdlib.h>

#include "greybus-testbench.h"

struct connection {
    TAILQ_ENTRY(connection) cnode;
    uint16_t cport_id;
    uint16_t hd_cport_id;
    int protocol;

    struct interface *intf;
};

struct interface {
    TAILQ_ENTRY(interface) intf_node;

    uint8_t intf_id;
    uint8_t feature;

    uint32_t vendor_id;
    uint32_t product_id;
    uint64_t serial_id;

    void *manifest;
    size_t manifest_size;
//    unsigned long manifest_fname_hash;

    struct connection *control_conn;
    struct svc *svc;

    TAILQ_HEAD(chead, connection) connections;
};

struct svc {
    struct interface *intf;

    TAILQ_HEAD(intf_head, interface) interfaces;
};

void cport_pack(struct gb_operation_msg_hdr *header, uint16_t cport_id);
void cport_clear(struct gb_operation_msg_hdr *header);
uint16_t cport_unpack(struct gb_operation_msg_hdr *header);
struct interface *interface_get_by_id(struct svc *svc, uint8_t intf_id);
struct interface *allocate_interface(struct svc *svc, uint8_t intf_id);
void interface_free(struct svc *svc, struct interface *intf);
struct connection *_get_connection(uint16_t cport_id);
struct connection *allocate_connection(struct interface *intf, uint16_t cport_id, uint16_t hd_cport_id);
void free_connection(struct connection *conn);
void set_connection_protocol(struct connection *conn, uint16_t cport_id);

int cport_get_protocol(struct interface *intf, uint16_t cport_id);

#endif //__CONTROLLER_H
