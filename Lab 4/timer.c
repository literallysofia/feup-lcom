#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

static int counter = 0;
int hook_id = 20;

int timer_set_square(unsigned long timer, unsigned long freq) {
	//calcular o divisor
	//calcular atraves do divisor o MSB e o LSB
	//ler a configuracao do controlo
	//manipular informacao dos 4 ultimos bits
	//escrever o LSB e depois MSB no timer que escolhemos

	if (freq < 19) {
		printf("ERROR: frequency should be higher than 18.\n");
		return 1;
	} else {
		unsigned long div, msb, lsb;
		unsigned char conf;

		div = TIMER_FREQ / freq;

		if (timer_get_conf(timer, &conf) != 0) {
			printf("ERROR: timer_get_conf failed.\n");
			return 1;
		}

		lsb = div;
		msb = div >> 8;

		switch (timer) {
		case 0:
			if (sys_outb(TIMER_CTRL, (TIMER_0 | TIMER_LSB_MSB | (conf << 4)))
					!= OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			if (sys_outb(TIMER_0, lsb) != OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			if (sys_outb(TIMER_0, msb) != OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			break;
		case 1:
			if (sys_outb(TIMER_CTRL, (TIMER_1 | TIMER_LSB_MSB | (conf << 4)))
					!= OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			if (sys_outb(TIMER_1, lsb) != OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			if (sys_outb(TIMER_1, msb) != OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			break;
		case 2:
			if (sys_outb(TIMER_CTRL, (TIMER_2 | TIMER_LSB_MSB | (conf << 4)))
					!= OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			if (sys_outb(TIMER_2, lsb) != OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			if (sys_outb(TIMER_2, msb) != OK) {
				printf("ERROR: sys_outb failed.\n");
				return 1;
			}
			break;
		default:
			return 1;
			break;
		}
		return 0;
	}
}

int timer_subscribe_int(void) {
	int hook_id_temp = hook_id;

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK) {
		printf("ERROR: sys_irqsetpolicy failed.\n");
		return -1;
	}

	if (sys_irqenable(&hook_id) != OK) {
		printf("ERROR: sys_irqenable failed.\n");
		return -1;
	}

	return BIT(hook_id_temp);
}

int timer_unsubscribe_int() {
	if (sys_irqdisable(&hook_id) != OK) {

		printf("ERROR: sys_irqdisable of timer_unsubscribe_int failed.\n");
		return -1;
	}
	if (sys_irqrmpolicy(&hook_id) != OK) {

		printf("ERROR: sys_irqrmpolicy of timer_unsubscribe_int failed.\n");
		return -1;
	}
	return 0;
}

void timer_int_handler() {
	counter++;
	return;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {

	unsigned long rb_Command = TIMER_RB_CMD | TIMER_RB_COUNT_
			| TIMER_RB_SEL(timer);

	unsigned long temp;

	if (sys_outb(TIMER_CTRL, rb_Command) == OK) {
		switch (timer) {
		case 0:
			if (sys_inb(TIMER_0, &temp) != OK) {
				printf("ERROR: sys_inb failed.\n");
				return 1;
			}
			break;
		case 1:
			if (sys_inb(TIMER_1, &temp) != OK) {
				printf("ERROR: sys_inb failed.\n");
				return 1;
			}
			break;
		case 2:
			if (sys_inb(TIMER_2, &temp) != OK) {
				printf("ERROR: sys_inb failed.\n");
				return 1;
			}
			break;
		default:
			printf("ERROR: sys_outb failed.\n");
			return 1;
			break;
		}
		*st = (unsigned char) temp;
		return 0;
	}

	return 1;
}

int timer_display_conf(unsigned char conf) {

	//STATUS
	printf("Timer Status Byte: ");
	printf("0x%x", conf);
	printf("\n");

	//SELECT COUNTER
	printf("Select Counter: ");
	if (conf & TIMER_SEL1 & TIMER_SEL2)
		return 1;
	else {
		if (!(conf & TIMER_SEL1 & TIMER_SEL2))
			printf("0.\n");
		else {
			if (conf & TIMER_SEL1)
				printf("1.\n");
			else
				printf("2.\n");
		}
	}

	//OUTPUT
	printf("Output: ");
	if (conf & TIMER_SEL2)
		printf("[1].\n");
	else
		printf("[0].\n");

	//NULL COUNT
	printf("Null Count: ");
	if (conf & TIMER_SEL1)
		printf("[1].\n");
	else
		printf("[0].\n");

	//TYPE OF ACCESS
	printf("Type of Access: ");
	if (conf & TIMER_LSB_MSB)
		printf("LSB followed by MSB -[11].\n");
	else {
		if (conf & TIMER_LSB)
			printf("LSB -[01].\n");
		else {
			if (conf & TIMER_MSB)
				printf("MSB -[10].\n");
			else
				printf("unknown.\n");
		}
	}

	//OPERATING MODE
	printf("Operating Mode: ");
	if (conf & TIMER_SQR_WAVE)
		printf("square wave generator (mode 3) -[X11].\n");
	else {
		if (conf & TIMER_RATE_GEN)
			printf("rate generator (mode 2) -[X10].\n");
		else
			printf("could be 0, 1, 4 or 5.\n");
	}

	//COUNTING MODE
	printf("Counting Mode: ");
	if (conf & BIT(0))
		printf("BCD -[1].\n");
	else
		printf("binary -[0].\n");

	return 0;
}

int timer_test_square(unsigned long freq) {

	if (timer_set_square(0, freq) == 0)
		return 0;
	else {
		printf("ERROR: timer_set_square failed.\n");
		return 1;
	}
}

int timer_test_int(unsigned long time) {

	int ipc_status;
	message msg;
	int r;
	int irq_set;

	if (timer_set_square(0, 60) != 0) {
		printf("ERROR: timer_set_square failed.\n");
		return -1;
	}

	if ((irq_set = timer_subscribe_int()) == -1) {
		printf("ERROR: timer_subscribe_int failed.\n");
		return -1;
	}

	while (counter < time * 60) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					timer_int_handler();
					break;
					default:
					break;
				}
			}
		} else {
		}
	}

	counter = 0;
	if (timer_unsubscribe_int() == -1) {
		printf("ERROR: timer_unsubscribe_int failed.\n");
		return -1;
	}
	return 0;
}

int timer_test_config(unsigned long timer) {

	if ((timer <= 2) && (timer >= 0)) {

		unsigned char st;

		if (timer_get_conf(timer, &st) == 0) {
			if (timer_display_conf(st) == 0)
				return 0;
			else {
				printf("ERROR: timer_display_conf failed.\n");
				return 1;
			}
		} else {
			printf("ERROR: timer_get_conf failed.\n");
			return 1;
		}

	} else {
		printf("ERROR: timer out of range.\n");
		return 1;
	}
}
