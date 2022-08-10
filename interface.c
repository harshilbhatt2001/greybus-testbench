#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/queue.h>

#include "debug.h"
#include "greybus-testbench.h"
#include "controller.h"

struct interface *interface_get_by_id(struct svc *svc, uint8_t intf_id) {
    struct interface *intf;

    TAILQ_FOREACH(intf, &svc->interfaces, intf_node) {
        if (intf->intf_id == intf_id) {
            return intf;
        }
    }

    return NULL;
}

void interface_free(struct svc *svc, struct interface *intf) {
    struct connection *conn;

    pr_debug("%s(): free interface %u\n", __func__, intf->intf_id);
    TAILQ_FOREACH(conn, &intf->connections, cnode) {
        free(conn);
    }

    TAILQ_REMOVE(&svc->interfaces, intf, intf_node);
    free(intf->manifest);
    free(intf);
}

struct interface *allocate_interface(struct svc *svc, uint8_t intf_id) {
    pr_debug("%s(): Allocating interface %d\n", __func__, intf_id);
    struct interface *intf;

    intf = interface_get_by_id(svc, intf_id);
    if (intf) {
        pr_error("%s(): allocated an existing interface %u\n", __func__, intf_id);
        return intf;
    }

    intf = calloc(1, sizeof(*intf));
    if (!intf) {
        pr_error("%s(): cannot allocate interface %d\n", __func__, intf_id);
        return NULL;
    }

    TAILQ_INIT(&intf->connections);
    intf->intf_id = intf_id;

    intf->svc = svc;

    TAILQ_INSERT_TAIL(&svc->interfaces, intf, intf_node);

    return intf;
}