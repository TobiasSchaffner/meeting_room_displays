
#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <power/power.h>
#include <hal/nrf_gpio.h>

#include "power.h"

bool initialized = false;

static void disable_device(const char* name)
{
	struct device *dev = device_get_binding(name);
	int rc = device_set_power_state(dev, DEVICE_PM_OFF_STATE, NULL, NULL);

	if (rc) {
		printk("Error disabling device %s peripheral (%d)\n", name, rc);
	}
}

static void enable_device(const char* name)
{
	struct device *dev = device_get_binding(name);
	int rc = device_set_power_state(dev, DEVICE_PM_ACTIVE_STATE, NULL, NULL);

	if (rc) {
		printk("Error enabling device %s peripheral (%d)\n", name, rc);
	}
}

/*
 * Application defined function for doing any target specific operations
 * for low power entry.
 */
void sys_pm_notify_power_state_entry(enum power_states state)
{
	if (initialized) {
		switch (state) {
		case SYS_POWER_STATE_SLEEP_1:
			printk("Disabling devices\n");
			//disable_device("UART_O");
			disable_device("UART_1");
			k_cpu_idle();
			break;
		default:
			printk("Unsupported power state %u\n", state);
			break;
		}
	}
}

/*
 * Application defined function for doing any target specific operations
 * for low power exit.
 */
void sys_pm_notify_power_state_exit(enum power_states state)
{
	if (initialized) {
		switch (state) {
		case SYS_POWER_STATE_SLEEP_1:
			//enable_device("UART_O");
			enable_device("UART_1");
			printk("Enabled devices\n");
			break;

		default:
			printk("Unsupported power state %u\n", state);
			break;
		}
	}
}

int power_init(void) {
	initialized = true;
	return 0;
}