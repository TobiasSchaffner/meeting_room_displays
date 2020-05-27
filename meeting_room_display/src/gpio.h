#ifndef GPIO_H
#define GPIO_H

#if defined(BOARD_NRF52840_PCA10056)
int gpio_init(void);

// Callbacks for buttons to be defined by user.
void on_button_press(u32_t button);
#else
int gpio_init(void) { return 0; }
#endif
#endif