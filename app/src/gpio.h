#ifndef GPIO_H
#define GPIO_H

#if !defined(CONFIG_USB_CDC_ACM)
int gpio_init(void);

// Callbacks for buttons to be defined by user.
void on_button_press(uint32_t button);
#else
int gpio_init(void) { return 0; }
#endif
#endif