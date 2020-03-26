
#include <zephyr/types.h>

extern u16_t addr;
extern u16_t target;

void mesh_init(void);

u16_t mesh_set_target_address(void);
void mesh_send_button_message(void);
void mesh_send_message(const char* message, u16_t len);

// Callbacks to be defined by main
void on_message_received(char* message, u16_t len);
void on_heartbeat(u16_t hops);