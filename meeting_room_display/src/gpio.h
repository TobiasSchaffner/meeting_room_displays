/* 
 * Configuration of gpios, buttons and leds
 */

int gpio_init(void);

// Callbacks for buttons to be defined by user.
void on_button_1_press(void);
void on_button_2_press(void);
void on_button_3_press(void);
void on_button_4_press(void);