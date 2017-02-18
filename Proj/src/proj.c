#include <minix/drivers.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "i8254.h"
#include "i8042.h"
#include "stateMachine.h"
#include "game.h"
#include "graphics.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "video_gr.h"
#include "timer.h"

int main() {
	srand(time(NULL));
	sef_startup();

	vg_init(VBE_MODE);
	Game* game = (Game*) startGame();
	States* gameStates = (States*) newGameStates();

	defaultPlayer(game);
	positionMouseInit(game); //inicializa as coordenadas do rato

	interruptHandler(game, gameStates);

	leaveGame(game, gameStates);
	vg_exit();
	return 0;
}
