
#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <power/power.h>
#include <hal/nrf_gpio.h>

#include "power.h"
#include "mesh.h"
//#include "display.h"

#include <logging/log.h>

LOG_MODULE_REGISTER(power, LOG_LEVEL_INF);

bool initialized = false;

static void disable_device(const char* name)
{
	struct device *dev = device_get_binding(name);
	int rc = device_set_power_state(dev, DEVICE_PM_LOW_POWER_STATE, NULL, NULL);

	if (rc) {
		LOG_ERR("Error disabling device %s peripheral (%d)", name, rc);
	}
}

bool sys_pm_policy_low_power_devices(enum power_states state) {
	if (initialized) {
		switch (state) {
		case SYS_POWER_STATE_SLEEP_1:
			LOG_INF("Disabling devices\n");
			disable_device("UART_1");
			return true;
		default:
			LOG_ERR("Unsupported power state %u\n", state);
		}
	}
	return false;
}

void power_suspend(int seconds)
{
	on_power_suspend();
	k_sleep(K_SECONDS(seconds));
	on_power_resume();
}

int power_init(void) {
	initialized = true;
	return 0;
}