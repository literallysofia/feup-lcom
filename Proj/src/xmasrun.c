#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "xmasrun.h"
#include "keyboard.h"
#include "graphics.h"
#include "i8254.h"
#include "i8042.h"

void kbdManager(Game* game, States* gameStates) {

	if (gameStates->gameState == MAIN_MENU)
		if (game->scan == ESC_BREAK)
			check_menu(END, gameStates);

	if (gameStates->gameState == GAME) {

		if (game->scan == ESC_BREAK)
			check_menu(ESC, gameStates);

		if (gameStates->playerState == RUN) {

			if (game->scan == SPACE_MAKE)
				check_move(UP, gameStates);

			if (game->scan == W_MAKE)
				check_move(UP, gameStates);

			if (game->scan == S_MAKE)
				check_move(LOW, gameStates);

		}

		if (gameStates->playerState == DOWN) {
			if (game->scan == S_BREAK)
				check_move(BACK, gameStates);
		}

	}

	return;
}

void timeManager(Game* game, States* gameStates) {

	updateDateHour(game);
	countDown(game);
	countDownDraw(game);

	if (gameStates->gameState == MAIN_MENU) {
		positionInit(game);
		playAgain(game);
		updateFrame(game, gameStates);
	}

	else if (gameStates->gameState == GAME) {

		if (gameStates->playerState == JUMP) {
			if (jump(game) == 0)
				check_move(BACK, gameStates);
		}

		if (gameStates->playerState == DOWN) {
			movedown(game);
		}

		obj();

		if (game->intcounter % 10 == 0)
			scoreCounter(game);

		updateFrame(game, gameStates);

		if (collision(gameStates) == -1)
			check_menu(COLLISION, gameStates);

	}

	else if (gameStates->gameState == GAMEOVER) {
		updateFrame(game, gameStates);

		if (gameStates->playerState != RUN)
			check_move(BACK, gameStates);

	} else if (gameStates->gameState == MENU_OPTIONS) {
		updateFrame(game, gameStates);
	}

	DBtoVM();

	return;
}

void mouseManager(Game* game, States* gameStates) {

	if (gameStates->gameState == MAIN_MENU) {

		drawMouse(game);

		if (game->packet[0] & L_BUTTON) {
			if (clicksMainMenu() == 1)
				check_menu(PLAY, gameStates);

			if (clicksMainMenu() == 2)
				check_menu(END, gameStates);

			if (clicksMainMenu() == 3)
				check_menu(OPTIONS, gameStates);
		}

	} else if (gameStates->gameState == GAME) {

		if (gameStates->playerState == RUN) {
			if (game->packet[0] & L_BUTTON)
				check_move(UP, gameStates);

			if (game->packet[0] & R_BUTTON)
				check_move(LOW, gameStates);
		}

		if (gameStates->playerState == DOWN) {
			if (!(game->packet[0] & R_BUTTON))
				check_move(BACK, gameStates);
		}

	}

	else if (gameStates->gameState == GAMEOVER) {
		drawMouse(game);

		if (game->packet[0] & L_BUTTON) {
			if (clicksGameOver() == 1)
				check_menu(HOME, gameStates);

			else if (clicksGameOver() == 2) {
				check_menu(PLAYAGAIN, gameStates);
				positionInit(game);
				playAgain(game);
			}
		}

	}

	else if (gameStates->gameState == MENU_OPTIONS) {
		drawMouse(game);

		if (game->packet[0] & L_BUTTON) {
			if (clicksOptionsMenu() == 1) {
				drawPlaque(1);
				changePlayer(game, 1);
			}

			if (clicksOptionsMenu() == 2) {
				drawPlaque(2);
				changePlayer(game, 2);
			}

			if (clicksOptionsMenu() == 3) {
				drawPlaque(3);
				changePlayer(game, 3);
			}

			if (clicksOptionsMenu() == 4) {
				check_menu(HOME, gameStates);
			}

		}

	}

	return;
}
