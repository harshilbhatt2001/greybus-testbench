#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdint.h>
#include <stdio.h>

/* debug/info/error macros */
#define pr_debug(fmt, ...)						\
        do { fprintf(stdout, "[D] greybus-testbench: " fmt,  	\
				  ##__VA_ARGS__); fflush(stdout); } while (0)
#define pr_info(fmt, ...)						\
        do { fprintf(stdout, "[I] greybus-testbench: " fmt, ##__VA_ARGS__); fflush(stdout); } while (0)
#define pr_error(fmt, ...)						\
        do { fprintf(stderr, "[E] greybus-testbench: " fmt, ##__VA_ARGS__); fflush(stderr); } while (0)

static inline void pr_dump(void *data, size_t size)
{
    uint8_t *buf = data;
    int i;

    fprintf(stdout, "[R] greybus-testbench: DUMP ->");
    for (i = 0; i < size; i++)
        fprintf(stdout, " %02hhx", buf[i]);
    fprintf(stdout, "\n");
    fflush(stdout);
}

#endif //__DEBUG_H
