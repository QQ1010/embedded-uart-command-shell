#include "cmd_system.h"
#include "shell.h"
#include "uart_io.h"

#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

static int cmd_help(int argc, char *argv[]) {
    (void)argc; (void)argv;
    int count;
    const shell_cmd_t *list = shell_get_cmds(&count);
    for(int i = 0 ; i < count; i++) {
        uart_io_printf("%s  %s\r\n", list[i].name, list[i].desc);
    }
    return 0;
}

static int cmd_free(int argc, char *argv[]) {
    (void)argc; (void)argv;
    uint32_t free_heap = esp_get_free_heap_size();
    uint32_t min_heap = esp_get_minimum_free_heap_size();
    uart_io_printf("Heap free: %u bytes\r\n", free_heap);
    uart_io_printf("Min free: %u bytes (all-time low)\r\n", min_heap);
    return 0;
}

static int cmd_uptime(int argc, char *argv[]) {
    (void)argc; (void)argv;
    uint32_t ms = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
    uint32_t sec = ms / 1000;
    uint32_t min = sec / 60;
    uint32_t hr = min / 60;
    uart_io_printf("Uptime: %uh %um %us\r\n", hr, min % 60, sec % 60);
    return 0;
}

static int cmd_ps(int argc, char *argv[]) {
    (void)argc; (void)argv;
    UBaseType_t count = uxTaskGetNumberOfTasks();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    const char *name = pcTaskGetName(handle);
    uart_io_printf("Tasks: %u\r\n", count);
    uart_io_printf("Current: %s\r\n", name);
    return 0;
}

static int cmd_reboot(int argc, char *argv[]) {
    (void)argc; (void)argv;
    uart_io_puts("Rebooting...\r\n");
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_restart();
    return 0;
}

void cmd_system_register_all(void) {
    static const shell_cmd_t cmds[] = {
        {"help", "Show available commands", cmd_help},
        {"free", "Show heap usage", cmd_free},
        {"uptime", "Show system uptime", cmd_uptime},
        {"ps", "List background processes", cmd_ps},
        {"reboot", "Restart", cmd_reboot},
    };
    for(int i = 0 ; i < (int)(sizeof(cmds)/sizeof(cmds[0])); i++) {
        shell_register(&cmds[i]);
    }
}