/*
 * Display specific definitions.
 */

#if defined(CONFIG_DISPLAY)
void display_init(void);

void display_set_top_label(char* message);
void display_set_center_label(char* message);
void display_set_bottom_label(char* message);
#else
void display_init(void) {}

void display_set_top_label(char* message) {}
void display_set_center_label(char* message) {}
void display_set_bottom_label(char* message) {}
#endif // 

