#ifndef UART_IO_H
#define UART_IO_H

#ifdef __cplusplus
extern "C" {
#endif

// Initialization
void uart_io_init(int baud_rate);

// Read char , if timeout return -1
int uart_io_getchar(int timeout_ms);

// Write char
void uart_io_putchar(char c);

// Write string
void uart_io_puts(const char *str);

// Printf
void uart_io_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif