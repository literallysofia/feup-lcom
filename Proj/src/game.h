#ifndef GAME_H
#define GAME_H

/** @defgroup Game Game
* @{
*/

#include "stateMachine.h"
#include "bitmap.h"

typedef struct {
	int irq_set_kbd; ///< specifies the keyboard irq
	int irq_set_timer; ///< specifies the timer irq
	int irq_set_mouse; ///< specifies the mouse irq

	unsigned char packet[3]; ///< specifies the mouse packtes of the last interrupt

	int intcounter; ///< number of interrupts counter
	unsigned long scan; ///< scan code of the keyboard of the last interrupt

	//images

	/** @name  All the bitmaps*/
	/**@{
	*
	*/
	Bitmap* start;
	Bitmap* gameover;
	Bitmap* options;
	Bitmap* background;

	Bitmap* deer;
	Bitmap* deer2;
	Bitmap* deerdown;
	Bitmap* santa;
	Bitmap* santa2;
	Bitmap* santadown;
	Bitmap* snowman;
	Bitmap* snowman2;
	Bitmap* snowmandown;

	Bitmap* gift;
	Bitmap* candy;
	Bitmap* snowball;
	Bitmap* trees;
	Bitmap* mouse;
	Bitmap* clouds;
	Bitmap* plaque;

	//numbers
	Bitmap* zero;
	Bitmap* one;
	Bitmap* two;
	Bitmap* three;
	Bitmap* four;
	Bitmap* five;
	Bitmap* six;
	Bitmap* seven;
	Bitmap* eight;
	Bitmap* nine;

	Bitmap* score;
	Bitmap* until;

	/** @} end of All the bitmaps */

	//date
	unsigned long* day; ///< specifies the day that was got from the rtc
	unsigned long* month; ///< specifies the month that was got from the rtc
	unsigned long* year; ///< specifies the year that was got from the rtc

	//hour
	unsigned long* hour; ///< specifies the hour that was got from the rtc
	unsigned long* minutes; ///< specifies the minutes that was got from the rtc
	unsigned long* seconds;///< specifies the seconds that was got from the rtc

} Game;


/**
	* @brief Subscribes timer, mouse and keyboard. Loads all bitmaps to the right member in game struct. Alocates space for the date and hour.
	* @return pointer to the struct game
	*/
Game* startGame();

/**
	* @brief Handles the interrups from timer, keyboard and mouse.
	* @param game pointer to an object of game struct
	* @param gameStates the struct with the current states in the two state machines
	*/
void interruptHandler(Game* game, States* gameStates);

/**
	* @brief Deletes all bitmaps
	* @param game pointer to an object of game struct
	*/
void deleteBitmaps(Game* game);

/**
	* @brief unsubscribes the timer, keyboard and mouse, calls deleteBitmaps and frees all the allocated memory.
	* @param game pointer to an object of game struct
	* @param gameStates the struct with the current states in the two state machines
	*/
void leaveGame(Game* game, States* gameStates);

/** @} end of Game */

#endif /* GAME_H */
