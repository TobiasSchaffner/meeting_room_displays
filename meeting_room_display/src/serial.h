#ifndef SERIAL_H
#define SERIAL_H

#include "message.h"

#if defined(CONFIG_USB_CDC_ACM)

int serial_init(void);
void serial_message_send(u32_t message_type, u16_t src_address, const void* payload, u16_t len);

// Callbacks for messages to be defined by user.
void on_serial_message_received(u32_t message_type, u16_t dst_address, const void* payload, u16_t len);
#else
int serial_init(void) { return 0; }
void serial_message_send(u32_t message_type, u16_t src_address, const void* payload, u16_t len) {}
#endif
#endif
