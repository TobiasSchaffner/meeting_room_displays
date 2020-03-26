
#include <zephyr/types.h>

extern u16_t addr;
extern u16_t target;

u16_t set_target(void);
void send_button_message(void);
void send_message(const char* message, u16_t len);

void on_message_received(char* message, u16_t len);
void on_heartbeat(u16_t hops);

void mesh_init(void);