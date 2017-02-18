#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

unsigned long counter = 0;
static int hook_id = 20;


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

