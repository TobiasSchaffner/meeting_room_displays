#ifndef DISPLAY_H
#define DISPLAY_H

#include "message.h"

#if defined(CONFIG_DISPLAY)
int display_init(void);

void display_set_status_message(const char* message);
void display_set_status_address(u16_t address);
void display_set_status_heartbeat(u16_t hops);
void display_create_appointment(message_appointment* appointment);
void display_clear_appointments(void);
#else
int display_init(void) { return 0; }

void display_set_status_message(const char* message) {}
void display_set_status_address(u16_t address) {}
void display_set_status_heartbeat(u16_t hops) {}
void display_create_appointment(message_appointment* appointment) {}
void display_clear_appointments(void) {}
#endif // 

#endif