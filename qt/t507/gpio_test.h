#ifndef __GPIO_TEST_H_
#define __GPIO_TEST_H_


#define SYSFS_GPIO_DIR "/sys/class/gpio"

#define MAX_BUF 256

enum PIN_DIRECTION{
    INPUT_PIN=0,
    OUTPUT_PIN=1
};

enum PIN_VALUE{
    LOW=0,
    HIGH=1
};

int calc_port_num(char port, int num);
int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_state(unsigned int gpio, char *state);
int gpio_set_value(unsigned int gpio, int value);
int gpio_get_value(unsigned int gpio);


#endif
