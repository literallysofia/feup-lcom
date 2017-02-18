#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"
#include "timer.h"

int kbd_test_scan(unsigned short ass) {

	int ipc_status;
	message msg;
	int r;
	int irq_set = kbd_subscribe_int();
	unsigned long scan;
	int a = 0;

	if (irq_set == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	while (scan != ESC_BREAK) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					if (ass == 1) {
						scan = kbd_ass();
					} else if (ass == 0) {
						scan = readData();
					}

					if (a) {
						a = 0;
						if (scan & BIT(7))
							printf("BREAK CODE: 0xE0 0x%02x\n", scan);
						else
							printf("\nMAKE CODE: 0xE0 0x%02x\n", scan);
					} else {

						if (scan == TWO_BYTES)
							a = 1;
						else if (scan & BIT(7))
							printf("BREAK CODE: 0x%02x\n", scan);
						else
							printf("\nMAKE CODE: 0x%02x\n", scan);
					}
				}
				break;
			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	printf("Finished.\n");
	if (kbd_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *leds) {
	unsigned short led;

	int kbd_irq_set = kbd_subscribe_int();

	if (kbd_irq_set == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	unsigned int i;
	for (i = 0; i < n; i++) {

		timer_test_int(1);

		if (kbd_cmd(LEDS_CMD) == -1) {
			printf("ERROR: kbd_cmd failed.\n");
			return -1;
		}

		led ^= BIT(leds[i]);

		if (kbd_cmd(led) == -1) {
			printf("ERROR: kbd_cmd failed.\n");
			return -1;
		}

		switch (leds[i]) {
		case 0:
			printf("ScrollLock changed.\n");
			break;
		case 1:
			printf("NumLock changed.\n");
			break;
		case 2:
			printf("CapsLock changed.\n", leds[i]);
			break;
		default:
			break;
		}
	}

	if (kbd_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}
	return 0;
}

int kbd_test_timed_scan(unsigned short n) {

	int ipc_status;
	message msg;
	int r;
	int kbd_irq_set = kbd_subscribe_int();
	unsigned long scan;
	int a = 0;

	unsigned long counter = 0;
	int timer_irq_set = timer_subscribe_int();

//se der erro
	if (kbd_irq_set == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	if (timer_irq_set == -1) {
		printf("ERROR: timer_subscribe_int failed.\n");
		return -1;
	}

//funcao
	while (counter < n * 60) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & timer_irq_set) /* subscribed interrupt */
					counter++;

				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */

					scan = readData();

					if (a) {
						a = 0;
						if (scan & BIT(7))
							printf("BREAK CODE: 0xE0 0x%02x\n", scan);
						else
							printf("\nMAKE CODE: 0xE0 0x%02x\n", scan);
					} else {

						if (scan == TWO_BYTES)
							a = 1;
						else if (scan & BIT(7))
							printf("BREAK CODE: 0x%02x\n", scan);
						else
							printf("\nMAKE CODE: 0x%02x\n", scan);
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}

			if (scan == ESC_BREAK) {

				printf("\nESC: Finished.\n");
				if (kbd_unsubscribe_int() == -1) {
					return 1;
				}
				if (timer_unsubscribe_int() == -1) {
					return 1;
				}
				return 0;
			}
		}
	}
	printf("\nTime out! Finished.\n");
	if (kbd_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	if (timer_unsubscribe_int() == -1) {
		printf("ERROR: timer_unsubscribe_int failed.\n");
		return -1;
	}

	return 0;
}
