#include "i8042.h"

.text
.global _kbd_ass

_kbd_ass:

	xor %eax,%eax
CICLO:
	inb $STAT_REG, %al
	test $OUT_BUF_FULL, %al
	jz CICLO
	inb $OUT_BUF, %al
ret
