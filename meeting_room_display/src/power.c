
#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <power/power.h>
#include <hal/nrf_gpio.h>

struct device *uart0;
struct device *uart1;

void sys_pm_notify_power_state_entry(enum power_states state)
{
    printk("Entering state: %d\n", state);
}

void power_hibernate(int seconds)
{
	// Disable UART
	device_pm_disable(uart0);
	device_pm_disable(uart1);

	// Force Sleep
	k_sleep(K_SECONDS(seconds));

	device_pm_enable(uart0);
	device_pm_enable(uart1);
}

int power_init()
{
    uart0 = device_get_binding("UART_0");
    uart1 = device_get_binding("UART_1");
    return 0;
}