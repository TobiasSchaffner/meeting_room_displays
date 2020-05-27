#ifndef SERIAL_H
#define SERIAL_H

#if defined(CONFIG_USB_CDC_ACM)
int serial_init(void);

// Callbacks for messages to be defined by user.
void on_serial_message_received(const void* message, u16_t len);
#else
int serial_init(void) { return 0; }
#endif
#endif