#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct message_appointment
{
    float start;
    float end;
    char description[];
} message_appointment;

#endif