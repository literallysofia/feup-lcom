#include "stateMachine.h"

States* newGameStates() {
	States* gameStates = (States*) malloc(sizeof(States));

	gameStates->gameState = MAIN_MENU;
	gameStates->playerState = RUN;

	return gameStates;
}

void check_menu(event_game evt, States* gameStates) {

	switch (gameStates->gameState) {

	case MAIN_MENU:
		if (evt == PLAY)
			gameStates->gameState = GAME;
		else if (evt == END)
			gameStates->gameState = COMP;
		else if (evt == OPTIONS)
			gameStates->gameState = MENU_OPTIONS;

	case GAME:
		if (evt == COLLISION)
			gameStates->gameState = GAMEOVER;
		else if (evt == ESC)
			gameStates->gameState = GAMEOVER;

	case GAMEOVER:
		if (evt == PLAYAGAIN)
			gameStates->gameState = GAME;
		else if (evt == HOME)
			gameStates->gameState = MAIN_MENU;

	case MENU_OPTIONS:
		if (evt == HOME)
			gameStates->gameState = MAIN_MENU;

		break;
	default:
		break;
	}

	return;
}

void check_move(event_t evt, States* gameStates) {

	switch (gameStates->playerState) {

	case RUN:
		if (evt == UP)
			gameStates->playerState = JUMP;
		else if (evt == LOW)
			gameStates->playerState = DOWN;

	case JUMP:
		if (evt == BACK)
			gameStates->playerState = RUN;

	case DOWN:
		if (evt == BACK)
			gameStates->playerState = RUN;

		break;
	default:
		break;
	}

	return;
}

void deleteGameStates(States* gameStates) {
	free(gameStates);
	return;
}
