#include <minix/drivers.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "pixmap.h"

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv) {
	sef_startup();
	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char **argv) {

	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"init <mode, delay>\"\n"
					"\t service run %s -args \"square <x, y, size, color>\"\n"
					"\t service run %s -args \"line <xi, yi, xf,yf, color>\"\n"
					"\t service run %s -args \"xpm <xi,yi, xpxm[]>\"\n"
					"\t service run %s -args \"move <xi, yi, xpm[], hor, delta, time>\"\n"
					"\t service run %s -args \"controller\"\n", argv[0],
			argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {

	unsigned long mode, delay, xi, yi, size, xf, yf, hor, time, color;
	long delta;
	char *str;

	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("test5: wrong no. of arguments for test_init()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16);
		if ((delay = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;

		printf("test_init(0x%lu , %lu)\n", mode, delay);
		return test_init(mode, delay);

	} else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("test5: wrong no. of arguments for test_square()\n");
			return 1;
		}
		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((size = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[5], 16)) == ULONG_MAX)
			return 1;

		printf("test5::test_square(%lu, %lu, %lu, 0x%X)\n", xi, yi, size,
				color);
		test_square(xi, yi, size, color);
		return 0;

	} else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("test5: wrong no of arguments for test of test_line()\n");
			return 1;
		}

		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((xf = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((yf = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[6], 16)) == ULONG_MAX)
			return 1;

		printf("test_line()\n");
		return test_line(xi, yi, xf, yf, color);
	} else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {
			printf("test5: wrong no of arguments for test_xpm()\n");
			return 1;
		}

		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;

		if ((strncmp(argv[4], "pic1", strlen("pic1")) == 0))
			test_xpm(xi, yi, pic1);
		else if ((strncmp(argv[4], "pic2", strlen("pic2")) == 0))
			test_xpm(xi, yi, pic2);
		else if ((strncmp(argv[4], "pic3", strlen("pic3")) == 0))
			test_xpm(xi, yi, pic3);
		else if ((strncmp(argv[4], "cross", strlen("cross")) == 0))
			test_xpm(xi, yi, cross);
		else if ((strncmp(argv[4], "penguin", strlen("penguin")) == 0))
			test_xpm(xi, yi, penguin);
		else {
			printf("That pic is not available\n", argv[4]);
			return 1;
		}

		printf("test5::test_xpm(%ld)\n");
		return 0;
	} else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 8) {
			printf("test5: wrong no of arguments for test_move()\n");
			return 1;
		}

		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((hor = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((delta = parse_long(argv[6], 10)) == LONG_MAX)
			return 1;
		if ((time = parse_ulong(argv[7], 10)) == ULONG_MAX)
			return 1;

		if ((strncmp(argv[4], "pic1", strlen("pic1")) == 0))
			test_move(xi, yi, pic1, hor, delta, time);
		else if ((strncmp(argv[4], "pic2", strlen("pic2")) == 0))
			test_move(xi, yi, pic2, hor, delta, time);
		else if ((strncmp(argv[4], "pic3", strlen("pic3")) == 0))
			test_move(xi, yi, pic3, hor, delta, time);
		else if ((strncmp(argv[4], "cross", strlen("cross")) == 0))
			test_move(xi, yi, cross, hor, delta, time);
		else if ((strncmp(argv[4], "penguin", strlen("penguin")) == 0))
			test_move(xi, yi, penguin, hor, delta, time);
		else {
			printf("That pic is not available\n", argv[4]);
			return 1;
		}

		printf("test5::test_move(%ld)\n");
		return 0;
	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 3) {
			printf("test5: wrong no of arguments for test_controller()\n");
			return 1;
		}
		printf("test5::test_controller(%ld)\n");
		return test_controller();
	} else {
		printf("test5: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static long parse_long(char *str, int base) {
	char *endptr;
	long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == LONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return LONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_ulong: no digits were found in %s\n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}
