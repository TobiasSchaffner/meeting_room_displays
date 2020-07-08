
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
	int rc = device_set_power_state(dev, DEVICE_PM_LOW_POWER_STATE, NULL, NULL);

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

bool sys_pm_policy_low_power_devices(enum power_states state) {
	if (initialized) {
		switch (state) {
		case SYS_POWER_STATE_SLEEP_1:
			printk("Disabling devices\n");
			disable_device("UART_1");
			return true;
		default:
			printk("Unsupported power state %u\n", state);
		}
	}
	return false;
}

int power_init(void) {
	initialized = true;
	return 0;
}