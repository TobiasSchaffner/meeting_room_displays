#include <zephyr/types.h>
#include <bluetooth/mesh.h>
#include <bluetooth/bluetooth.h>
#include "message.h"

#ifndef MESH_H
#define MESH_H

#if !defined(MESH_NODE_ADDR)
#define MESH_NODE_ADDR 0x0b0c
#endif

#define MESH_GROUP_ADDR 0xc000

int mesh_init(void);

void mesh_increment_target_address(void);
void mesh_message_send(u32_t message_type, u16_t dst_address, const void* payload, u16_t len);

// Callbacks to be defined by main
void on_mesh_message_received(u32_t message_type, u16_t src_address, const void* payload, u16_t len);
void on_mesh_heartbeat(u16_t hops);

#endif
