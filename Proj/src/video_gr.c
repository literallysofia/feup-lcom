#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "video_gr.h"
#include "vbe.h"

/* Private global variables */

static char *video_mem; /* Process address to which VRAM is mapped */
static char *double_buffer;

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned int vram_size; /* VRAM's size, but you can use
	 the frame-buffer size, instead */

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
	printf("%d", v_res);
	h_res = vbe_mode.XResolution;
	bits_per_pixel = vbe_mode.BitsPerPixel;

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

	double_buffer = (char *) malloc(vram_size);

	return (void*) vbe_mode.PhysBasePtr;
}

void DBtoVM() {
	memcpy(video_mem, double_buffer, vram_size);
}

void callBitmap(Bitmap* bmp, int x, int y) {
	drawBitmap(double_buffer, bmp, x, y);
}
