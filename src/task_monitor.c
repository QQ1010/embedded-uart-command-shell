#include "task_monitor.h"
#include "uart_io.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#define LED_PIN      2        // 板子上的 LED
#define HEAP_MIN     10000    // heap 低於 10KB 就警告

static void monitor_task(void *param) {
    // 1. 先把 LED_PIN 設成 output（gpio_set_direction）
    // 2. 進無窮迴圈：
    //    - toggle LED（記一個 int state，每次 1 變 0、0 變 1）
    //    - gpio_set_level(LED_PIN, state)
    //    - 檢查 free heap，低於門檻就 uart_io_printf 印警告
    //    - vTaskDelay(pdMS_TO_TICKS(500))  ← 等 500ms
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    int state = 0;
    while(1) {
        state = !state;
        gpio_set_level(LED_PIN, state);
        uint32_t free = esp_get_free_heap_size();
        if(free < HEAP_MIN) {
            uart_io_printf("[WARN] low heap: %u\r\n", free);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void task_monitor_start(void) {
    xTaskCreate(monitor_task, "monitor", 2048, NULL, 1, NULL);
}