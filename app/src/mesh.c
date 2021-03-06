#include <stdio.h>
#include <settings/settings.h>
#include "mesh.h"
#include "message.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(ble, LOG_LEVEL_INF);

#define MOD_LF 0x0000

#define PUBLISHER_ADDR  0x000f
#define MESH_MESSAGE_TYPE(type)			BT_MESH_MODEL_OP_3(type, BT_COMP_ID_LF)

uint16_t mesh_addr = MESH_NODE_ADDR;
uint16_t mesh_target = MESH_GROUP_ADDR;

static const uint8_t net_key[16] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};
static const uint8_t dev_key[16] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};
static const uint8_t app_key[16] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
};

static const uint16_t net_idx;
static const uint16_t app_idx;
static const uint32_t iv_index;
static uint8_t flags;

static void heartbeat(uint8_t hops, uint16_t feat)
{
    on_mesh_heartbeat(hops);
}

static struct bt_mesh_cfg_srv cfg_srv = {
	.relay = BT_MESH_RELAY_ENABLED,
	.beacon = BT_MESH_BEACON_ENABLED,
	.frnd = BT_MESH_FRIEND_NOT_SUPPORTED,
	.default_ttl = 7,

	/* 3 transmissions with 20ms interval */
	.net_transmit = BT_MESH_TRANSMIT(2, 20),
	.relay_retransmit = BT_MESH_TRANSMIT(3, 20),

	.hb_sub.func = heartbeat,
};

static struct bt_mesh_cfg_cli cfg_cli = {
};

static void attention_on(struct bt_mesh_model *model)
{
	LOG_INF("Attention on.");
}

static void attention_off(struct bt_mesh_model *model)
{
	LOG_INF("Attention off.");
}

static const struct bt_mesh_health_srv_cb health_srv_cb = {
	.attn_on = attention_on,
	.attn_off = attention_off,
};

static struct bt_mesh_health_srv health_srv = {
	.cb = &health_srv_cb,
};

BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);

static struct bt_mesh_model root_models[] = {
	BT_MESH_MODEL_CFG_SRV(&cfg_srv),
	BT_MESH_MODEL_CFG_CLI(&cfg_cli),
	BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
};

static void message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf, uint32_t opcode) {
	// Check if we received our own message
	if (ctx->addr == bt_mesh_model_elem(model)->addr) {
		LOG_DBG("Ignored message as 0x%04x is our own address.\n", bt_mesh_model_elem(model)->addr);
		return;
	}
    on_mesh_message_received(opcode, ctx->addr, buf->data, buf->len);
}

static void ok_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_OK);}
static void day_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_DAY);}
static void room_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_ROOM);}
static void appointment_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_APPOINTMENT);}
static void clear_appointment_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_APPOINTMENT_CLEAR);}
static void clear_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_APPOINTMENTS_CLEAR);}
static void suspend_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_SUSPEND);}
static void sync_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_SYNC);}
static void string_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_STRING);}
static void button_message_received(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf) {
	message_received(model, ctx, buf, MESSAGE_BUTTON);}

static const struct bt_mesh_model_op vnd_ops[] = {
	{ MESH_MESSAGE_TYPE(MESSAGE_OK), 0, ok_message_received },
	{ MESH_MESSAGE_TYPE(MESSAGE_DAY), 0, day_message_received },
	{ MESH_MESSAGE_TYPE(MESSAGE_ROOM), 0, room_message_received },
	{ MESH_MESSAGE_TYPE(MESSAGE_APPOINTMENT), 0, appointment_message_received },
	{ MESH_MESSAGE_TYPE(MESSAGE_APPOINTMENT_CLEAR), 0, clear_appointment_message_received },
	{ MESH_MESSAGE_TYPE(MESSAGE_APPOINTMENTS_CLEAR), 0, clear_message_received },
	{ MESH_MESSAGE_TYPE(MESSAGE_SUSPEND), 0, suspend_message_received },
	{ MESH_MESSAGE_TYPE(MESSAGE_SYNC), 0, sync_message_received },

	{ MESH_MESSAGE_TYPE(MESSAGE_STRING), 0, string_message_received },
	{ MESH_MESSAGE_TYPE(MESSAGE_BUTTON), 0, button_message_received },

	BT_MESH_MODEL_OP_END,
};

static struct bt_mesh_model vnd_models[] = {
	BT_MESH_MODEL_VND(BT_COMP_ID_LF, MOD_LF, vnd_ops, NULL, NULL),
};

static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, root_models, vnd_models),
};

static const struct bt_mesh_comp comp = {
	.cid = BT_COMP_ID_LF,
	.elem = elements,
	.elem_count = ARRAY_SIZE(elements),
};

void mesh_message_send(uint32_t message_type, uint16_t address, const void* payload, uint16_t len) {
	NET_BUF_SIMPLE_DEFINE(msg, 3 + len + 4);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx = net_idx,
		.app_idx = app_idx,
		.addr = address,
		.send_ttl = BT_MESH_TTL_DEFAULT,
	};
	
	bt_mesh_model_msg_init(&msg, MESH_MESSAGE_TYPE(message_type));

	if (len != 0 && payload != NULL)
		net_buf_simple_add_mem(&msg, payload, len);

	if (bt_mesh_model_send(&vnd_models[0], &ctx, &msg, NULL, NULL) == 0) {
		LOG_DBG("Sending Mesh Message Type: %d Address: %d!", message_type, address);
	} else {
		LOG_ERR("Sending Mesh Failed!");
	}
}

static void configure(void)
{
	LOG_INF("Configuring...\n");

	/* Add Application Key */
	bt_mesh_cfg_app_key_add(net_idx, mesh_addr, net_idx, app_idx, app_key, NULL);

	/* Bind to vendor model */
	bt_mesh_cfg_mod_app_bind_vnd(net_idx, mesh_addr, mesh_addr, app_idx,
				     MOD_LF, BT_COMP_ID_LF, NULL);

	/* Bind to Health model */
	bt_mesh_cfg_mod_app_bind(net_idx, mesh_addr, mesh_addr, app_idx,
				 BT_MESH_MODEL_ID_HEALTH_SRV, NULL);

	/* Add model subscription */
	bt_mesh_cfg_mod_sub_add_vnd(net_idx, mesh_addr, mesh_addr, MESH_GROUP_ADDR,
				    MOD_LF, BT_COMP_ID_LF, NULL);

#if MESH_NODE_ADDR == PUBLISHER_ADDR
	{
		struct bt_mesh_cfg_hb_pub pub = {
			.dst = MESH_GROUP_ADDR,
			.count = 0xff,
			.period = 0x05,
			.ttl = 0x07,
			.feat = 0,
			.net_idx = net_idx,
		};

		bt_mesh_cfg_hb_pub_set(net_idx, mesh_addr, &pub, NULL);
		LOG_INF("Publishing heartbeat messages");
	}
#endif
	printk("Configuration complete.\n");
}

static const uint8_t dev_uuid[16] = { 0xdd, 0xdd };

static const struct bt_mesh_prov prov = {
	.uuid = dev_uuid,
};

static void bt_ready(int err)
{
	if (err) {
		LOG_ERR("Bluetooth init failed. (err %d)", err);
		return;
	}

	LOG_INF("Bluetooth initialized.");

	err = bt_mesh_init(&prov, &comp);
	if (err) {
		LOG_ERR("Initializing mesh failed. (err %d)", err);
		return;
	}

	LOG_INF("Mesh initialized.");

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		LOG_INF("Loading stored settings");
		settings_load();
	}

	err = bt_mesh_provision(net_key, net_idx, flags, iv_index, mesh_addr,
				dev_key);

	if (err == -EALREADY) {
		LOG_INF("Using stored settings.");
	} else if (err) {
		LOG_ERR("Provisioning failed. (err %d)", err);
		return;
	} else {
		LOG_INF("Provisioning completed.");
		configure();
	}

#if MESH_NODE_ADDR != PUBLISHER_ADDR
	/* Heartbeat subcscription is a temporary state (due to there
	 * not being an "indefinite" value for the period, so it never
	 * gets stored persistently. Therefore, we always have to configure
	 * it explicitly.
	 */
	{
		struct bt_mesh_cfg_hb_sub sub = {
			.src = PUBLISHER_ADDR,
			.dst = MESH_GROUP_ADDR,
			.period = 0x10,
		};

		bt_mesh_cfg_hb_sub_set(net_idx, mesh_addr, &sub, NULL);
		LOG_INF("Subscribing to heartbeat messages.");
	}
#endif
}

int mesh_suspend(void)
{
	return bt_mesh_suspend();
}

int mesh_resume(void)
{
	return bt_mesh_resume();
}

int mesh_init(void) {
    int err;

    /* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
	}

	return err;
}