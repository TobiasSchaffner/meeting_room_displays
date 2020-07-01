#include <stdio.h>
#include <string.h>
#include <device.h>
#include <drivers/uart.h>
#include <zephyr.h>
#include <sys/ring_buffer.h>

#include <usb/usb_device.h>
#include <logging/log.h>

#include "serial.h"

LOG_MODULE_REGISTER(cdc_acm_echo, LOG_LEVEL_INF);

#define BUF_SIZE 128
#define MESSAGE_SERIAL_BEGIN			"###"
#define MESSAGE_MIN_LEN				    8

char buffer[BUF_SIZE];
u16_t bytes_read = 0;

struct device *dev;

typedef struct serial_message
{
    char begin[3];
    u8_t length;
    u16_t address;
    u16_t type;
    u8_t payload[];
} serial_message;

static void serial_message_received(u32_t message_type, u16_t dst_address, const void* payload, u16_t len) {
	printk("Received Serial Message Type %d: Address: %d Len: %d\n", message_type, dst_address, len);
	on_serial_message_received(message_type, dst_address, payload, len);
}

static void interrupt_handler(struct device *dev)
{
	while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {

		if (uart_irq_rx_ready(dev)) {
			char *begin;

			bytes_read += uart_fifo_read(dev, &buffer[bytes_read], BUF_SIZE - bytes_read);
			begin = strstr(buffer, MESSAGE_SERIAL_BEGIN);

			printk("Recieved\n");
			if (begin == NULL) bytes_read = 0;
			else {
				int offset = begin - buffer;

				printk("Marker\n");
				if (bytes_read - offset >= MESSAGE_MIN_LEN) {
					serial_message* message = (serial_message*) begin;

					if (bytes_read - (begin - buffer) >= message->length) {
						serial_message_received(message->type, message->address, message->payload, message->length);
						bytes_read = 0;
					}
				}
			}
			uart_irq_tx_enable(dev);
		}

		if (uart_irq_tx_ready(dev)) {
            uart_irq_tx_disable(dev);
            continue;
		}
	}
}

void serial_message_send(u32_t message_type, u16_t address, const void* payload, u16_t len)
{
	u32_t buf_size = sizeof(serial_message) + len;
	u8_t buf[buf_size];

	serial_message* message = (serial_message*) &buf;
	memcpy(buf, MESSAGE_SERIAL_BEGIN, 3);
	message->type = message_type;
	message->address = address;
	message->length = len;
	memcpy(&buf[sizeof(serial_message)], payload, len);

	k_str_out(buf, buf_size);
}

int serial_init(void)
{
	u32_t dtr = 0U;
	int ret;

	printk("Enabling USB");

	k_sleep(K_MSEC(2000));

	ret = usb_enable(NULL);
	if (ret != 0) {
		LOG_ERR("Failed to enable USB");
		return 2;
	}

	dev = device_get_binding("CDC_ACM_0");
	if (!dev) {
		LOG_ERR("CDC ACM device not found");
		return 1;
	}

	LOG_INF("Wait for DTR");

	while (true) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		if (dtr) {
			break;
		} else {
			k_sleep(K_MSEC(100));
		}
	}

	LOG_INF("DTR set");

	uart_irq_callback_set(dev, interrupt_handler);
	uart_irq_rx_enable(dev);

	return 0;
}
