#ifndef MESSAGE_H
#define MESSAGE_H

#define MESSAGE_OK				        0x00
#define MESSAGE_DAY				        0x01
#define MESSAGE_ROOM				    0x02
#define MESSAGE_APPOINTMENT			    0x03
#define MESSAGE_APPOINTMENT_CLEAR		0x04
#define MESSAGE_APPOINTMENTS_CLEAR		0x05
#define MESSAGE_SUSPEND		            0x06
#define MESSAGE_SYNC		            0x07

#define MESSAGE_STRING				    0x0e
#define MESSAGE_BUTTON				    0x0f

typedef struct message_appointment
{
    int slot;
    float start;
    float end;
    char description[];
} message_appointment;

#endif
