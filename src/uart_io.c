#include "uart_io.h"
#include "driver/uart.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Initialization
void uart_io_init(int baud_rate) {
    if(uart_is_driver_installed(UART_NUM_0))
        return;
    
    // Configure UART communication parameters.
    // Uses the specified baud rate, 8-N-1 format (8 data bits, no parity,
    // 1 stop bit), and disables RTS/CTS hardware flow control.
    uart_config_t cfg = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM_0, &cfg);
    uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);
}

// Read char , if timeout return -1
int uart_io_getchar(int timeout_ms) {
    uint8_t c;
    int len = uart_read_bytes(UART_NUM_0, &c, 1, pdMS_TO_TICKS(timeout_ms));
    return len > 0 ? (int)c : -1;
}

// Write char
void uart_io_putchar(char c) {
    uart_write_bytes(UART_NUM_0, &c, 1);
}

// Write string
void uart_io_puts(const char *str) {
    uart_write_bytes(UART_NUM_0, str, strlen(str));
}

// Printf
void uart_io_printf(const char *fmt, ...) {
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    uart_io_puts(buf);
}