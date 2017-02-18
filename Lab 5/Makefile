# Makefile for lab5

COMPILER_TYPE= gnu

CC= gcc

PROG= lab5
SRCS= lab5.c test5.c vbe.c video_gr.c timer.c keyboard.c read_xpm.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -llm -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
