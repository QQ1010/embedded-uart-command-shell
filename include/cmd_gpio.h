#ifndef CMD_GPIO_H
#define CMD_GPIO_H

#include "hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

void cmd_gpio_init(const hal_gpio_driver_t *driver);
void cmd_gpio_register_all(void);


#ifdef __cplusplus
}
#endif

#endif