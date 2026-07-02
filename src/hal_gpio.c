#include "hal_gpio.h"
#include "driver/gpio.h"

static int esp32_gpio_set_mode(int pin, int mode) {
    gpio_mode_t dir = (mode == HAL_GPIO_MODE_OUTPUT) 
                           ? GPIO_MODE_OUTPUT 
                           : GPIO_MODE_INPUT;
    return gpio_set_direction((gpio_num_t)pin, dir);
}

static int esp32_gpio_write(int pin, int value) {
    return gpio_set_level((gpio_num_t)pin, (uint32_t)value);
}

static int esp32_gpio_read(int pin) {
    return gpio_get_level((gpio_num_t)pin);
}

static const hal_gpio_driver_t esp32_gpio_driver = {
    .set_mode = esp32_gpio_set_mode,
    .write    = esp32_gpio_write,
    .read     = esp32_gpio_read,
};

void hal_gpio_init(hal_gpio_dev_t *dev, const hal_gpio_driver_t *driver) {
    dev->driver = driver;
}

const hal_gpio_driver_t *hal_gpio_get_esp32_driver(void) {
    return &esp32_gpio_driver;
}

int hal_gpio_write(hal_gpio_dev_t *dev, int pin, int value) {
    if(dev->driver == NULL) {
        return -1;
    }
    if(dev->driver->write == NULL) {
        return -1;
    }
    return dev->driver->write(pin, value);
}

int hal_gpio_set_mode(hal_gpio_dev_t *dev, int pin, int mode) {
    if(dev->driver == NULL) {
        return -1;
    }
    if(dev->driver->set_mode == NULL) {
        return -1;
    }
    return dev->driver->set_mode(pin, mode);
}
int hal_gpio_read(hal_gpio_dev_t *dev, int pin) {
    if(dev->driver == NULL) {
        return -1;
    }
    if(dev->driver->read == NULL) {
        return -1;
    }
    return dev->driver->read(pin);
}