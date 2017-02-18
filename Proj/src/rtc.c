#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "rtc.h"
#include "i8042.h"
#include "i8254.h"

int isRTCUpdating() {

	unsigned long RegA = 0;

	sys_outb(RTC_ADDR_REG, REGA);
	sys_inb(RTC_DATA_REG, &RegA);

	if ((RegA & RTC_REGA_UIP) != 0)
		return 1;
	else return 0;

}

int isBCD() {

	unsigned long RegB = 0;

	sys_outb(RTC_ADDR_REG, REGB);
	sys_inb(RTC_DATA_REG, &RegB);

	if (!(RegB & RTC_REGB_DM))
		return 1;
	else
		return 0;

}

unsigned long BCDtoBinary(unsigned long* bcd) {
	unsigned long bi;

	bi = (((*bcd) & 0xF0) >> 4) * 10 + ((*bcd) & 0x0F);

	return bi;
}

void getDate(unsigned long *day, unsigned long *month, unsigned long *year) {

	sys_outb(RTC_ADDR_REG, RTC_DAYMONTH);
	sys_inb(RTC_DATA_REG, day);
	sys_outb(RTC_ADDR_REG, RTC_MONTH);
	sys_inb(RTC_DATA_REG, month);
	sys_outb(RTC_ADDR_REG, RTC_YEAR);
	sys_inb(RTC_DATA_REG, year);


	if (isBCD()) {
		(*day) = BCDtoBinary(day);
		(*month) = BCDtoBinary(month);
		(*year) = BCDtoBinary(year);
	}

}

void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds) {

	sys_outb(RTC_ADDR_REG, RTC_HRS);
	sys_inb(RTC_DATA_REG, hour);
	sys_outb(RTC_ADDR_REG, RTC_MIN);
	sys_inb(RTC_DATA_REG, minutes);
	sys_outb(RTC_ADDR_REG, RTC_SEC);
	sys_inb(RTC_DATA_REG, seconds);

	if (isBCD()) {
		(*hour) = BCDtoBinary(hour);
		(*minutes) = BCDtoBinary(minutes);
		(*seconds) = BCDtoBinary(seconds);
	}

}
