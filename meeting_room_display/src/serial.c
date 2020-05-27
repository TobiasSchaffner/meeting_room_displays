#include <stdio.h>
#include <string.h>
#include <device.h>
#include <drivers/uart.h>
#include <zephyr.h>
#include <sys/ring_buffer.h>

#include <usb/usb_device.h>
#include <logging/log.h>
LOG_MODULE_REGISTER(cdc_acm_echo, LOG_LEVEL_INF);

#define RING_BUF_SIZE 1024
u8_t buffer[128];
int recv_len;

static struct k_work message_received_work;

static void message_received(struct k_work *work){
	on_serial_message_received(buffer, recv_len);}

static void interrupt_handler(struct device *dev)
{
	while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
        
		if (uart_irq_rx_ready(dev)) {
			recv_len = uart_fifo_read(dev, buffer, sizeof(buffer));

            k_work_submit(&message_received_work);

			uart_irq_tx_enable(dev);
		}
    
		if (uart_irq_tx_ready(dev)) {
            uart_irq_tx_disable(dev);
            continue;
		}
	}
}

void serial_init(void)
{
	struct device *dev;
	u32_t dtr = 0U;
	int ret;

    k_work_init(&message_received_work, message_received);

	dev = device_get_binding("CDC_ACM_0");
	if (!dev) {
		LOG_ERR("CDC ACM device not found");
		return;
	}

	ret = usb_enable(NULL);
	if (ret != 0) {
		LOG_ERR("Failed to enable USB");
		return;
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
}