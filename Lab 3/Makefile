# Makefile for lab2

COMPILER_TYPE= gnu

CC= gcc

PROG= lab3
SRCS= lab3.c timer.c test3.c keyboard.c kbd_ass.S

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
