#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_GPIO_MODE_INPUT  0
#define HAL_GPIO_MODE_OUTPUT 1

// driver
typedef struct {
    int (*set_mode)(int pin, int mode);
    int (*write)(int pin, int value);
    int (*read)(int pin);
} hal_gpio_driver_t;

// device instance
typedef struct {
    const hal_gpio_driver_t *driver;
    const char *name;
} hal_gpio_dev_t;


void hal_gpio_init(hal_gpio_dev_t *dev, const hal_gpio_driver_t *driver);
int hal_gpio_set_mode(hal_gpio_dev_t *dev, int pin, int mode);
int hal_gpio_write(hal_gpio_dev_t *dev, int pin, int value);
int hal_gpio_read(hal_gpio_dev_t *dev, int pin);
const hal_gpio_driver_t *hal_gpio_get_esp32_driver(void);

#ifdef __cplusplus
}
#endif

#endif