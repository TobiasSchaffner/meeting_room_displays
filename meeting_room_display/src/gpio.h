#ifndef GPIO_H
#define GPIO_H

int gpio_init(void);

// Callbacks for buttons to be defined by user.
void on_button_press(u32_t button);

#endif