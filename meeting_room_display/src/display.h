#ifndef DISPLAY_H
#define DISPLAY_H

#include "message.h"

#if defined(CONFIG_DISPLAY)
int display_init(void);

void display_set_date(const char* message);
void display_sed_room(const char* message);
void display_set_status_message(const char* message);
void display_set_status_address(uint16_t address);
void display_set_status_heartbeat(uint16_t hops);
void display_create_appointment(message_appointment* appointment);
void display_clear_appointments(void);
#else
int display_init(void) { return 0; }

void display_set_date(const char* message) {}
void display_sed_room(const char* message) {}
void display_set_status_message(const char* message) {}
void display_set_status_address(uint16_t address) {}
void display_set_status_heartbeat(uint16_t hops) {}
void display_create_appointment(message_appointment* appointment) {}
void display_clear_appointments(void) {}

#endif // 

#endif