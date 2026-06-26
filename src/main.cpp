#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" {
    #include "uart_io.h"
    #include "shell.h"
}

void setup() {
  uart_io_init(115200);
  shell_init();
  uart_io_puts("\r\n=== RTOS Shell ===\r\n");

  xTaskCreate(shell_task, "shell", 4096, NULL, 2, NULL);
}

void loop() {
    vTaskDelay(portMAX_DELAY);
}
