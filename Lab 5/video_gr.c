#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "pixmap.h" // defines  pic1, pic2, etc
#include "read_xpm.h"
#include "vbe.h"
#include "timer.h"
#include "keyboard.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vg_init(unsigned short mode) {
	vbe_mode_info_t vbe_mode;

	vbe_get_mode_info(mode, &vbe_mode);

	v_res = vbe_mode.YResolution;
	h_res = vbe_mode.XResolution;
	bits_per_pixel = vbe_mode.BitsPerPixel;

	unsigned int vram_size; /* VRAM's size, but you can use
	 the frame-buffer size, instead */

	struct reg86u reg;
	reg.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
	reg.u.b.intno = 0x10;
	if (sys_int86(&reg) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	vram_size = h_res * v_res * (bits_per_pixel / 8);

	int r;
	struct mem_range mr;

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vbe_mode.PhysBasePtr; //depois trocar por generico (endereco base)
	mr.mr_limit = mr.mr_base + vram_size;
	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
	if (video_mem == MAP_FAILED)
		panic("couldn't map video memory");

	return (void*) vbe_mode.PhysBasePtr;
}

int set_pixel(unsigned short x, unsigned short y, unsigned long color) {

	if ((x < 0 || x >= h_res) || (y < 0 || y >= v_res)) // input check
		return -1;

	char *ptr_VM = video_mem;

	//contas
	ptr_VM += (x + h_res * y) * (bits_per_pixel / 8);

	*ptr_VM = color;
	return 0;

}

int draw_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	printf("x: %d , y: %d.\n", x, y);
	int i, j;

	for (i = 0; i < size; i++) {
		printf("I: %d\n", i);
		for (j = 0; j < size; j++) {
			set_pixel(x + i, y + j, color);
		}
	}

	return 0;
}

int draw_line(int xi, int yi, int xf, int yf, int color) {
	//algoritmo usado: Digital Differential Analyzer
	//baseado no codigo: https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm

	int difx,dify;
	float xinc, yinc;

	//calculo das diferenças
	difx = xf - xi;
	dify = yf - yi;

	//numero de iteraçoes
	int iters;

	if (abs(difx) > abs(dify)) //se x >
		iters = abs(difx);
	else
		iters = abs(dify);

	//incremento por iteraçao
	xinc = (float) difx / iters;
	yinc = (float) dify / iters;

	//adicionar ao ponto o incremento
	int i;
	for (i = 0; i < iters; i++) {
		xi = xi + xinc;
		yi = yi + yinc;

		//arredondamentos
		if (xi > 0)
			(long)(xi + 0.5);
		else
			(long)(xi - 0.5);

		if (yi > 0)
			(long)(yi + 0.5);
		else
			(long)(yi - 0.5);

		//pintar o pixel
		set_pixel(xi, yi, color);
	}

	return 0;

}

//desenha frame
int draw_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	int wd, hg;
	char *sprite = read_xpm(xpm, &wd, &hg);

	unsigned int i, j;

	for (i = 0; i < hg; i++) {
		for (j = 0; j < wd; j++) {
			if (*(sprite + i * wd + j) != 0)
				set_pixel(xi + j, yi + i, *(sprite + i * wd + j));
		}
	}

	free(sprite);

	return 0;
}

//apaga frame
int erase_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	int wd, hg;
	char *sprite = read_xpm(xpm, &wd, &hg);

	unsigned int i, j;

	for (i = 0; i < hg; i++) {
		for (j = 0; j < wd; j++) {
			set_pixel(xi + j, yi + i, 0);
		}
	}

	free(sprite);

	return 0;
}

int draw_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {

	int ipc_status;
	message msg;
	int r;
	unsigned long scan;
	int irq_set_kbd = kbd_subscribe_int();
	int irq_set_timer = timer_subscribe_int();

	float interPixel = (float) time * 60 / abs(delta); //interrupçoes por pixel
	float interTotal = time * 60; //interrupçoes totais do programa
	float interSum = 0; //soma de interrupçoes

	if (irq_set_timer == -1) {
		printf("ERROR: timer_subscribe_int failed.\n");
		return -1;
	}

	if (irq_set_kbd == -1) {
		printf("ERROR: kbd_subscribe_int failed.\n");
		return -1;
	}

	while (scan != ESC_BREAK && interSum < interTotal) {

		unsigned short xiTemp = xi;
		unsigned short yiTemp = yi;

		if (delta < 0) {
			if (hor == 1)
				xi--;
			else if (hor == 0)
				yi--;
		} else {
			if (hor == 1)
				xi++;
			else if (hor == 0)
				yi++;
		}

		int interCounter = 0; //contador de interrupçoes

		while (interCounter < interPixel) {

			if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}

			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */

					if (msg.NOTIFY_ARG & irq_set_timer)
						interCounter++;

					if (msg.NOTIFY_ARG & irq_set_kbd) /* subscribed interrupt */
						scan = readData();

					break;
				default:
					break; /* no other notifications expected: do*/
				}
			}
		}

		// apaga frame anterior
		erase_xpm(xiTemp, yiTemp, xpm);

		// desenha nova frame
		draw_xpm(xi, yi, xpm);

		interSum += interPixel;

	}

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
