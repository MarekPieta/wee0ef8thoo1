/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <usb/usb_device.h>
#include <power/reboot.h>
#include <random/rand32.h>
#include <drivers/gpio.h>

#define LED0	DT_GPIO_LABEL(DT_ALIAS(led0), gpios)
#define PIN	DT_GPIO_PIN(DT_ALIAS(led0), gpios)
#define FLAGS	DT_GPIO_FLAGS(DT_ALIAS(led0), gpios)

void assert_post_action(const char *file, unsigned int line)
{
	__asm volatile ("bkpt #0\n");
	sys_reboot(SYS_REBOOT_WARM);
}

static void sleep_and_reboot(void)
{
	uint32_t sleep_time_max = 15000;
	uint32_t sleep_time = 250 + sys_rand32_get() % sleep_time_max;

	k_sleep(K_MSEC(sleep_time));
	sys_reboot(SYS_REBOOT_WARM);
}

void main(void)
{
	const struct device *led_dev;

	led_dev = device_get_binding(LED0);
	if (gpio_pin_configure(led_dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS)) {
		return;
	}

	if (usb_enable(NULL)) {
		return;
	}

	while (1) {
		gpio_pin_set(led_dev, PIN, 1);
		sleep_and_reboot();
		gpio_pin_set(led_dev, PIN, 0);
	}
}
