#include <zephyr/types.h>
#include <bluetooth/mesh.h>
#include <bluetooth/bluetooth.h>

#if !defined(MESH_NODE_ADDR)
#define MESH_NODE_ADDR 0x0b0c
#endif

#define MESH_GROUP_ADDR 0xc000

#define MESH_MESSAGE_DAY     				BT_MESH_MODEL_OP_3(0x00, BT_COMP_ID_LF)
#define MESH_MESSAGE_ROOM     				BT_MESH_MODEL_OP_3(0x01, BT_COMP_ID_LF)
#define MESH_MESSAGE_APPOINTMENT     		BT_MESH_MODEL_OP_3(0x02, BT_COMP_ID_LF)
#define MESH_MESSAGE_APPOINTMENTS_CLEAR  	BT_MESH_MODEL_OP_3(0x03, BT_COMP_ID_LF)

#define MESH_MESSAGE_STRING 				BT_MESH_MODEL_OP_3(0x0e, BT_COMP_ID_LF)
#define MESH_MESSAGE_BUTTON 				BT_MESH_MODEL_OP_3(0x0f, BT_COMP_ID_LF)

typedef struct message_appointment
{
    float start;
    float end;
    char description[];
} message_appointment;

int mesh_init(void);

void mesh_increment_target_address(void);
void mesh_send_message(u32_t message_type, u16_t address, const void* message, u16_t len);

// Callbacks to be defined by main
void on_message_received(u32_t message_type, u16_t address, const void* message, u16_t len);
void on_heartbeat(u16_t hops);