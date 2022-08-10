#ifndef __UART_CONTROLLER_H
#define __UART_CONTROLLER_H

struct uart_controller {
    int fd;
};


//int register_uart_controller(const char *file_name, int baudrate);
int uart_loop(void);
int uart_controller_init(const char *file_name, int baudrate);
void uart_controller_exit();

#endif //__UART_CONTROLLER_H
