#include "timer.h"
#include "i8254.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv) {
	sef_startup();
	sys_enable_iop(SELF);

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char **argv) {
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"scan <0 for c, 1 for assembly>\"\n"
			"\t service run %s -args \"leds <x>\"\n"
			"\t service run %s -args \"timed_scan <decimal no. - time>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {
	unsigned long ass, size, time;

	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
		if (argc != 3) {
			printf("kbd: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}

		if ((ass = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if (ass != 0 && ass != 1) {
			printf("you can only choose 1 or 0\n");
			return 1;
		}
		printf("kbd_test_scan(%lu)\n", ass);
		kbd_test_scan(ass);
		return 0;

	} else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {
		if (argc < 3) {
			printf("kbd: wrong no. of arguments for kbd_test_leds()\n");
			return 1;
		}

		unsigned short leds[argc -2];
		int i;
		for (i=0; i < argc - 2; i++){
			leds[i] = parse_ulong(argv[i+2], 10);
		}

		printf("timer::kbd_test_leds(%lu)\n", argc - 2);
		kbd_test_leds(argc - 2, leds);
		return 0;

	} else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
		if (argc != 3) {
			printf("kbd: wrong no of arguments for timer_test_timed_scan()\n");
			return 1;
		}
		time = parse_ulong(argv[2], 10); /* Parses string to unsigned long */
		if (time == ULONG_MAX)
			return 1;
		printf("kbd::kbd_test_timed_scan(%lu)\n", time);
		kbd_test_timed_scan(time);
		return 0;
	} else {
		printf("kbd: %s - no valid function!\n", argv[1]);
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
		printf("kbd: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
