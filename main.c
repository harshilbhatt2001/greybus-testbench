#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "debug.h"
#include "controllers/uart_controller.h"
#include "greybus-testbench.h"

static void help(void) {
    printf("Greybus host testbench\n"
           "\t-h: Print help\n"
           "uart options:\n"
           "-p uart_device: set the uart device\n"
           "-b baud rate: set the uart baud rate\n"
    );
}

static void cleanup(void) {
    pr_debug("cleaning up...\n");
    uart_controller_exit();
    svc_exit();
}

int main(int argc, char *argv[]) {
    int ret;
    int c;
    int baudrate = 115200;
    const char *uart = NULL;

    while ((c = getopt(argc, argv, "p:b:")) != -1) {
        switch (c) {
            case 'p':
                uart = optarg;
                break;
            case 'b':
                if (sscanf(optarg, "%u", &baudrate) != 1) {
                    help();
                    return -EINVAL;
                }
                break;
            default:
                help();
                return -EINVAL;
        }
    }

    ret = uart_controller_init(uart, baudrate);
    if (ret) {
        return ret;
    }

    ret = svc_init();
    if (ret < 0) {
        pr_error("Failed to init svc\n");
        goto out_cleanup;
    }

    ret = uart_loop();

    out_cleanup:
    cleanup();

    return 0;
}
