#include <minix/sysutil.h>
#include <minix/drivers.h>

#include "mouse.h"
#include "i8042.h"
#include "i8254.h"

int mouse_hookid = 15;

int mouse_subscribe_int() {
	int mouse_hookid_temp = mouse_hookid;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookid)
			!= OK) {
		printf("ERROR: sys_irqsetpolicy failed.\n");
		return -1;
	}

	if (sys_irqenable(&mouse_hookid) != OK) {
		printf("ERROR: sys_irqenable failed.\n");
		return -1;
	}

	return BIT(mouse_hookid_temp);
}

int mouse_unsubscribe_int() {
	if (sys_irqdisable(&mouse_hookid) != OK) {

		printf("ERROR: sys_irqdisable of kbd_unsubscribe_int failed.\n");
		return -1;
	}
	if (sys_irqrmpolicy(&mouse_hookid) != OK) {
		printf("ERROR: sys_irqrmpolicy of kbd_unsubscribe_int failed.\n");
		return -1;
	}
	return 0;
}

void delay() {
	tickdelay(micros_to_ticks(WAIT_KBC));
}

int mouse_Read() {
	unsigned long stat;
	unsigned long data;
	int numTries = TRIES;

	while (numTries > 0) {
		sys_inb(KBC_CMD_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ((stat & (PAR_ERR | TO_ERR)) == 0) {
				delay();
				return data;
			} else
				return -1;
		}
		numTries--;
		delay();
	}
}

int mouse_Write(unsigned long cmd, unsigned long port) {
	unsigned long stat, data;
	int numTries = TRIES;

	while (numTries > 0) {
		sys_inb(KBC_CMD_REG, &stat); /* assuming it returns OK */

		//Discard output buffer
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
		}

		/* loop while 8042 input buffer is not empty */
		if ((stat & IBF) == 0) {
			sys_outb(port, cmd); /* no args command */
			delay();
			return 0;
		}
		numTries--;
		delay();
	}
}

int write_Controller(unsigned long cmd) {
	if (mouse_Write(cmd, KBC_CMD_REG) != 0)
		return -1;
	return 0;
}

int mouse_cmd(unsigned long cmd) {
	unsigned long data;

	do {
		if (mouse_Write(cmd, OUT_BUF) != 0)
			return -1;

		data = mouse_Read();

	} while (data == RESEND);

	if (data == ACK)
		return 0;
	else
		return -1;
}
