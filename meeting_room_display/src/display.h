/*
 * Display specific definitions.
 */

#if defined(CONFIG_DISPLAY)
int display_init(void);

void display_set_status_message(const char* message);
void display_set_status_address(u16_t address);
void display_set_status_heartbeat(u16_t hops);
#else
int display_init(void) { return 0; }

void display_set_status_message(const char* message) {}
void display_set_status_address(u16_t address) {}
void display_set_status_heartbeat(u16_t hops) {}
#endif // 

