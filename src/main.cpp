#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" {
    #include "uart_io.h"
    #include "shell.h"
    #include "cmd_system.h"
    #include "cmd_gpio.h"
    #include "hal_gpio.h"
}

void setup() {
  uart_io_init(115200);
  shell_init();
  cmd_system_register_all();
  cmd_gpio_init(hal_gpio_get_esp32_driver());
  cmd_gpio_register_all();
  uart_io_puts("\r\n=== RTOS Shell ===\r\n");

  xTaskCreate(shell_task, "shell", 4096, NULL, 2, NULL);

}

void loop() {
    vTaskDelay(portMAX_DELAY);
}
