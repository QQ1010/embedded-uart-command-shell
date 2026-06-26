#include <Arduino.h>
extern "C" {
    #include "uart_io.h"
}

void setup() {
  uart_io_init(115200);
  uart_io_puts("UART IO test\r\n");
  uart_io_printf("Number: %d\r\n", 42);
}

void loop() {

}
