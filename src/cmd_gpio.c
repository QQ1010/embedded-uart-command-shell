#include "cmd_gpio.h"
#include "hal_gpio.h"
#include "driver/gpio.h"
#include "shell.h"
#include "uart_io.h"
#include <string.h>
#include <stdlib.h> 

static hal_gpio_dev_t gpio_dev;

void cmd_gpio_init(const hal_gpio_driver_t *driver) {
    hal_gpio_init(&gpio_dev, driver);
}

int cmd_gpio(int argc, char *argv[]) {
    if(argc < 2) {
        uart_io_printf("Usage:\r\ngpio mode <pin> <in|out>\r\ngpio set  <pin> <0|1>\r\ngpio get  <pin>\r\n");
        return -1;
    }

    if(strcmp(argv[1], "mode") == 0) {
        if(argc < 4) {
            uart_io_printf("gpio mode <pin> <in|out>\r\n");
            return -1;
        }

        int pin = atoi(argv[2]);
        if(strcmp(argv[3], "out") == 0) {
            hal_gpio_set_mode(&gpio_dev, pin, HAL_GPIO_MODE_OUTPUT);
        }
        else if(strcmp(argv[3], "in") == 0) {
            hal_gpio_set_mode(&gpio_dev, pin, HAL_GPIO_MODE_INPUT);
        }
        return 0;
    }
    else if(strcmp(argv[1], "set") == 0) {
        if(argc < 4) {
            uart_io_printf("gpio set  <pin> <0|1>\r\n");
            return -1;
        }

        int pin = atoi(argv[2]);
        int value = atoi(argv[3]);
        hal_gpio_write(&gpio_dev, pin, value);
        return 0;
    }
    else if(strcmp(argv[1], "get") == 0) {
        if(argc < 3) {
            uart_io_printf("gpio get  <pin>\r\n");
            return -1;
        }

        int pin = atoi(argv[2]);
        int val = hal_gpio_read(&gpio_dev, pin);
        uart_io_printf("pin %d = %d\r\n", pin, val);
        return 0;
    }
    return -1;
}

void cmd_gpio_register_all(void) {
    static const shell_cmd_t cmds[] = {
        {"gpio", "Control GPIO pins", cmd_gpio},
    };
    for(int i = 0 ; i < (int)(sizeof(cmds)/sizeof(cmds[0])); i++) {
        shell_register(&cmds[i]);
    }
}