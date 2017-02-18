#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"

static int kbd_hookid = 10;

int kbd_subscribe_int(void) {

	int kbd_hookid_temp = kbd_hookid;

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hookid)
			!= OK) {
		printf("ERROR: sys_irqsetpolicy failed.\n");
		return -1;
	}

	if (sys_irqenable(&kbd_hookid) != OK) {
		printf("ERROR: sys_irqenable failed.\n");
		return -1;
	}

	return BIT(kbd_hookid_temp);
}

int kbd_unsubscribe_int() {
	if (sys_irqdisable(&kbd_hookid) != OK) {

		printf("ERROR: sys_irqdisable of kbd_unsubscribe_int failed.\n");
		return -1;
	}
	if (sys_irqrmpolicy(&kbd_hookid) != OK) {
		printf("ERROR: sys_irqrmpolicy of kbd_unsubscribe_int failed.\n");
		return -1;
	}
	return 0;
}

void delay() {
	tickdelay(micros_to_ticks(WAIT_KBC));
}

int readData() {

	unsigned long status;
	unsigned long data;

	while (1) {
		sys_inb(STAT_REG, &status); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if (status & OBF) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ((status & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		delay();
	}
}


void kbd_discard() {
	unsigned long stat, data;
	int numTries = TRIES;

	while (numTries > 0) {

		sys_inb(KBC_CMD_REG, &stat); /* assuming it returns OK */

		if (stat & OBF)
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */

		delay(1);
		numTries--;

	}
}
