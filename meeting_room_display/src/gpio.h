/* 
 * Configuration of gpios, buttons and leds
 */

int gpio_init(void);

// Callbacks for buttons to be defined by user.
void on_button_press(u32_t button);