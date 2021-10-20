/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <power/reboot.h>
#include <drivers/gpio.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define LED0	"GPIO_0"
#define PIN	5

const struct device *led_dev;

void assert_post_action(const char *file, unsigned int line)
{
	static struct k_spinlock lock;

	k_spinlock_key_t key = k_spin_lock(&lock);
	while(1){
	}
	k_spin_unlock(&lock, key);
}

void main(void)
{
	led_dev = device_get_binding(LED0);

	if (gpio_pin_configure(led_dev, PIN, GPIO_OUTPUT)) {
		return;
	}

	if (gpio_pin_set_raw(led_dev, PIN, 1)) {
		return;
	}

	k_sleep(K_MSEC(1));

	if (gpio_pin_set_raw(led_dev, PIN, 0)) {
		return;
	}

	k_sleep(K_MSEC(1));

	if (gpio_pin_set_raw(led_dev, PIN, 1)) {
		return;
	}
	sys_reboot(SYS_REBOOT_WARM);
}
