/*
 * Display specific definitions.
 */

#if defined(CONFIG_DISPLAY)
void display_init(void);

void display_set_status_message_label(char* message);
void display_set_status_address_label(char* message);
void display_set_status_heartbeat_label(char* message);
#else
void display_init(void) {}

void display_set_status_message_label(char* message) {}
void display_set_status_address_label(char* message) {}
void display_set_status_heartbeat_label(char* message) {}
#endif // 

