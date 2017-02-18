#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8042.h"
#include "i8254.h"
#include "test4.h"
#include "timer.h"
#include "mouse.h"

unsigned char packet[3];

void packet_display() {
	//primeiro packet da overflow e x/y errado e botao
	printf("B1=0x%02x ", packet[0]);
	printf("B2=0x%02x ", packet[1]);
	printf("B3=0x%02x ", packet[2]);
	printf("LB=%d ", ((packet[0] & L_BUTTON) ? 1 : 0));
	printf("MB=%d ", ((packet[0] & M_BUTTON) ? 1 : 0));
	printf("RB=%d ", ((packet[0] & R_BUTTON) ? 1 : 0));
	printf("XOV=%d ", ((packet[0] & X_OVFL) ? 1 : 0));
	printf("YOV=%d ", ((packet[0] & Y_OVFL) ? 1 : 0));

	if (packet[0] & X_SIGN)
		printf("X=-%u ", (packet[1] ^= 0xFF) + 1);
	else
		printf("X=%u ", packet[1]);

	if (packet[0] & Y_SIGN)
		printf("Y=-%u\n", (packet[2] ^= 0xFF) + 1);
	else
		printf("Y=%u\n", packet[2]);

	return;
}

void config_display() {

	printf("\nRemote: ");
	if (packet[0] & BIT(6))
		printf("polled mode.\n");
	else
		printf("stream mode.\n");

	printf("Enable: ");
	if (packet[0] & BIT(5))
		printf("data reporting enable.\n");
	else
		printf("data reporting disable.\n");

	printf("Scaling: ");
	if (packet[0] & BIT(4))
		printf("2:1.\n");
	else
		printf("1:1.\n");

	printf("Resolution: ");
	printf("%d\n", BIT(packet[1]));

	printf("Sample rate: %d.\n", packet[2]);

	return;
}

int test_packet(unsigned short cnt) {
	int ipc_status;
	message msg;
	int r;
	int mouse_irq_set = mouse_subscribe_int();
	int pCounter = 0;
	unsigned long pByte;
	unsigned short first = 0;

	if (mouse_irq_set == -1) {
		printf("ERROR: mouse_subscribe_int failed.\n");
		return -1;
	}

	if (write_Controller(MOUSE_WRITE) != 0) {
		printf("ERROR: write_Controller failed.\n");
		return -1;
	}

	if (mouse_cmd(MOUSE_EN_DATA) != 0) {
		printf("ERROR: mouse_cmd failed.\n");
		return -1;
	}
	mouse_Read();

	while (cnt > 0) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */

					sys_inb(OUT_BUF, &pByte);
					packet[pCounter] = pByte;

					if ((packet[0] & BIT(3)) == 0) {
						pCounter = 0;
					} else {
						if ((pCounter == 2)) {
							if (first != 0) {
								packet_display();
								cnt--;
								pCounter = 0;
							} else {
								first = 1;
								pCounter = 0;
							}
						} else
							pCounter++;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	printf("Done!\n");

	if (mouse_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}
	return 0;
}

int test_async(unsigned short idle_time) {
	int ipc_status;
	message msg;
	int r;
	int mouse_irq_set = mouse_subscribe_int();
	int pCounter = 0;
	unsigned long pByte;
	unsigned short first = 0;
	unsigned long counter = 0;
	int timer_irq_set = timer_subscribe_int();

	if (timer_irq_set == -1) {
		printf("ERROR: timer_subscribe_int failed.\n");
		return -1;
	}

	if (mouse_irq_set == -1) {
		printf("ERROR: mouse_subscribe_int failed.\n");
		return -1;
	}

	if (write_Controller(MOUSE_WRITE) != 0) {
		printf("ERROR: write_Controller failed.\n");
		return -1;
	}

	if (mouse_cmd(MOUSE_EN_DATA) != 0) {
		printf("ERROR: mouse_cmd failed.\n");
		return -1;
	}

	while (counter < idle_time * 60) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */

			if (msg.NOTIFY_ARG & timer_irq_set)
				counter++;

			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */

					sys_inb(OUT_BUF, &pByte);
					packet[pCounter] = pByte;
					counter = 0;

					if ((packet[0] & BIT(3)) == 0) {
						pCounter = 0;
					} else {
						if ((pCounter == 2)) {
							if (first != 0) {
								packet_display();
								pCounter = 0;
							} else {
								first = 1;
								pCounter = 0;
							}
						} else
							pCounter++;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	printf("Done!\n");

	if (mouse_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	if (timer_unsubscribe_int() == -1) {
		printf("ERROR: timer_unsubscribe_int failed.\n");
		return -1;
	}

	return 0;
}

int test_config(void) {
	int ipc_status;
	message msg;
	int r;
	int mouse_irq_set = mouse_subscribe_int();
	int pCounter = 0;
	unsigned long pByte;
	unsigned short first = 0;

	if (mouse_irq_set == -1) {
		printf("ERROR: mouse_subscribe_int failed.\n");
		return -1;
	}

	if (write_Controller(MOUSE_WRITE) != 0) {
		printf("ERROR: write_Controller failed.\n");
		return -1;
	}

	if (mouse_cmd(MOUSE_STATUS_REQ) != 0) {
		printf("ERROR: mouse_cmd failed.\n");
		return -1;
	}

	unsigned int i = 0;
	for (i; i < 3; i++) {
		packet[i] = mouse_Read();
	}

	packet_display();
	config_display();
	mouse_Read();

	if (mouse_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}
	return 0;
}

typedef enum {
	INIT, DRAW, COMP
} state_t;
typedef enum {
	RDOWN, RUP, MOVE
} event_t;
static state_t st = INIT; // initial state; keep state
int total = 0;

void check_line(event_t evt) {
	switch (st) {
	case INIT:
		if (evt == RDOWN)
			st = DRAW;
		break;
	case DRAW:
		if (evt == MOVE)
			st = COMP;
		else if (evt == RUP) {
			st = INIT;
			total = 0;
		}
		break;
	default:
		break;
	}
}

int test_gesture(short length) {
	int ipc_status;
	message msg;
	int r;
	int mouse_irq_set = mouse_subscribe_int();
	int pCounter = 0;
	unsigned long pByte;
	unsigned short first = 0;

	if (mouse_irq_set == -1) {
		printf("ERROR: mouse_subscribe_int failed.\n");
		return -1;
	}

	if (write_Controller(MOUSE_WRITE) != 0) {
		printf("ERROR: write_Controller failed.\n");
		return -1;
	}

	if (mouse_cmd(MOUSE_EN_DATA) != 0) {
		printf("ERROR: mouse_cmd failed.\n");
		return -1;
	}

	mouse_Read();

	/*if (sys_outb(STAT_REG, MOUSE_WRITE) != OK)
	 printf("ERROR: on writing to STATUS_REG.\n");
	 if (sys_outb(OUT_BUF, MOUSE_EN_DATA) != OK)
	 printf("ERROR: on writing to OUT_BUF.\n");*/

	while (st != COMP) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */

					sys_inb(OUT_BUF, &pByte);
					packet[pCounter] = pByte;

					if ((packet[0] & BIT(3)) == 0) {
						pCounter = 0;
					} else {
						if ((pCounter == 2)) {
							if (first != 0) {
								packet_display();

								if ((packet[0] & R_BUTTON) == 0) { //se largar right button
									check_line(RUP);
									total = 0;
								} else {
									check_line(RDOWN);

									if ((length > 0 && (packet[0] & X_SIGN) == 0)
											|| (length < 0
													&& (packet[0] & X_SIGN)
															== X_SIGN)) {

										if ((packet[0] & Y_SIGN) == Y_SIGN
												&& length < 0)
											total -= ((packet[2] ^= 0xFF) + 1);
										else if ((packet[0] & Y_SIGN) == 0
												&& length > 0)
											total += packet[2];

										if (length > 0) {
											if ((total >= length))
												check_line(MOVE);
										} else if ((total <= length)) {
											check_line(MOVE);
										}
									}
								}

								pCounter = 0;
							} else {
								first = 1;
								pCounter = 0;
							}
						} else
							pCounter++;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	printf("Done!\n");

	if (mouse_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	if (sys_outb(STAT_REG, MOUSE_WRITE) != OK)
		printf("ERROR: on writing to STATUS_REG.\n");
	if (sys_outb(IN_BUF, MOUSE_DIS_DATA) != OK)
		printf("ERROR: on writing to IN_BUF.\n");

	return 0;
}
