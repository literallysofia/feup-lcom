#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "video_gr.h"
#include "keyboard.h"
#include "i8254.h"
#include "i8042.h"
#include "timer.h"

void *test_init(unsigned short mode, unsigned short delay) {
	/* - Entrar em modo grafico
	 * Implementar vg_init em video_gr.c
	 * aguardar (60 x numero segundos interrupcoes
	 * chamar vg_exit() */

	void *init = vg_init(mode); //virtual address
	timer_test_int(delay);
	vg_exit();
	printf("physical address: %x", init);
	return init;
}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	/* Chamar vg_init() (mapeando mem. grafica)
	 * chamar em ciclo funcao "que pinta pixel"
	 * esperar tecla esc
	 * vg_exit()
	 * set_pixel (..., int x, int y, int color)
	 * Duvida: necessario implementar vbe_get_mode_info() ? */

	int ipc_status;
	message msg;
	int r;
	int irq_set = kbd_subscribe_int();
	unsigned long scan;

	if (irq_set == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	vg_init(VBE_MODE);

	draw_square(x, y, size, color);

	while (scan != ESC_BREAK) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					scan = readData();

				}
				break;
			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	if (kbd_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	vg_exit();
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {
	/* vg_init()
	 * desenhar reta usando set_pixel()
	 * esperar por esc
	 * vg_exit() */

	int ipc_status;
	message msg;
	int r;
	int irq_set = kbd_subscribe_int();
	unsigned long scan;

	if (irq_set == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	vg_init(VBE_MODE);

	draw_line(xi, yi, xf, yf, color);

	while (scan != ESC_BREAK) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					scan = readData();

				}
				break;
			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	if (kbd_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	vg_exit();
	return 0;

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	int ipc_status;
	message msg;
	int r;
	int irq_set = kbd_subscribe_int();
	unsigned long scan;

	if (irq_set == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	vg_init(VBE_MODE);

	draw_xpm(xi, yi, xpm);

	while (scan != ESC_BREAK) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					scan = readData();

				}
				break;
			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	if (kbd_unsubscribe_int() == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	vg_exit();
	return 0;
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {

	vg_init(VBE_MODE);

	draw_xpm(xi, yi, xpm);

	draw_move(xi, yi, xpm, hor, delta, time);

	vg_exit();
	return 0;
}

int test_controller() {

	/* To be completed */

}

