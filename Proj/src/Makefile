# Makefile for Christmas Run (Project)

COMPILER_TYPE= gnu

CC= gcc

PROG= proj
SRCS= proj.c vbe.c video_gr.c timer.c keyboard.c graphics.c xmasrun.c game.c bitmap.c mouse.c stateMachine.c rtc.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -llm -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
