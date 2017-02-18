#ifndef GRAPHICS_H
#define GRAPHICS_H

/** @defgroup Graphics Graphics
 * @{
 */

#include "stateMachine.h"
#include "game.h"
#include "bitmap.h"

typedef struct {
	Bitmap* bmp1; ///< bitmap of first position of the character
	Bitmap* bmp2; ///< bitmap of second position of the character
	int x; ///< specifies x of the character when is up (upper left corner)
	int y; ///< specifies y of the character when is up (upper left corner)
	int width;  ///< specifies width in pixels
	int height; ///< specifies height in pixels
} Character;

typedef struct {
	Bitmap* bmp; ///< bitmap of the character down
	int x; ///< specifies x of the character when is down (upper left corner)
	int y; ///< specifies y of the character when is down (upper left corner)
	int width; ///< specifies width in pixels
	int height; ///< specifies height in pixels
} DownCharacter;

typedef struct {
	int x; ///< specifies x of the candy cane (upper left corner)
	int y; ///< specifies y of the candy cane (upper left corner)
	int width; ///< specifies width in pixels
	int height; ///< specifies height in pixels
} Cane;

typedef struct {
	int x; ///< specifies x of the gift (upper left corner)
	int y; ///< specifies x of the gift (upper left corner)
	int width; ///< specifies width in pixels
	int height;  ///< specifies height in pixels
} Gift;

typedef struct {
	int x; ///< specifies x of the snow ball (upper left corner)
	int y; ///< specifies y of the snow ball (upper left corner)
	int width; ///< specifies width in pixels
	int height; ///< specifies height in pixels
} Snow;

typedef struct {
	int x; ///< specifies x of the trees (upper left corner)
	int y; ///< specifies y of the trees (upper left corner)
	int width; ///< specifies width in pixels
	int height;  ///< specifies height in pixels
} Tree;

typedef struct {
	int x; ///< specifies x of the clouds (upper left corner)
	int y; ///< specifies y of the clouds (upper left corner)
	int width; ///< specifies width in pixels
	int height; ///< specifies height in pixels
} Clouds;

typedef struct {
	int x; ///< specifies x of the plaque (upper left corner)
	int y; ///< specifies y of the plaque (upper left corner)
} Plaque;

typedef struct {
	int x; ///< specifies x of the plaque (upper left corner)
	int y; ///< specifies y of the plaque (upper left corner)
	int width; ///< specifies width in pixels
	int height; ///< specifies height in pixels
} Mouse;

typedef struct {
	Bitmap* pos1; ///< number bitmap of the first digit of the score counting from the right
	Bitmap* pos2; ///< number bitmap of the second digit of the score counting from the right
	Bitmap* pos3; ///< number bitmap of the third digit of the score counting from the right
	Bitmap* pos4; ///< number bitmap of the fourth digit of the score counting from the right
	Bitmap* pos5; ///< number bitmap of the fifth digit of the score counting from the right
	int value; ///< score value
} Score;

typedef struct {
	Bitmap* pos1S; ///< number bitmap of the first digit of the seconds counting from the right
	Bitmap* pos2S; ///< number bitmap of the second digit of the seconds counting from the right
	Bitmap* pos1M; ///< number bitmap of the first digit of the minutes counting from the right
	Bitmap* pos2M; ///< number bitmap of the second digit of the minutes counting from the right
	Bitmap* pos1H; ///< number bitmap of the first digit of the hours counting from the right
	Bitmap* pos2H; ///< number bitmap of the second digit of the hours counting from the right
	Bitmap* pos1D; ///< number bitmap of the first digit of the days counting from the right
	Bitmap* pos2D; ///< number bitmap of the second digit of the days counting from the right
	Bitmap* pos3D; ///< number bitmap of the third digit of the days counting from the right

	int seconds; ///< seconds value
	int minutes; ///< minutes value
	int hours; ///< hours value
	int days; ///< days value

} Countdown;

static Character player; ///< represents the character when it's up
static DownCharacter dplayer; ///< represents the character when it's down
static Gift present; ///< represents the gift obstacle
static Cane candyCane; ///< represents the candy cane obstacle
static Tree pine; ///< represents the pines obstacle
static Snow ball; ///< represents the snow ball obstacle
static Clouds clouds; ///< represents the first set of clouds
static Clouds clouds2; ///< represents the second set of clouds
static Plaque plaque; ///< represents the plaque in choose your character menu
static Score gameScore; ///< represents the score of the game
static Countdown countdown; ///< represents countdown until christmas

static Mouse mouse; ///< represents the mouse cursor

/**
 * @brief Initializes the position of the mouse cursor
 * @param game pointer to an object of game struct
 */
void positionMouseInit(Game* game);

/**
 * @brief Initializes the positions of the gift, the candy cane, the pines, the snowball, the first set of clouds, the second set of clouds and the game score
 * @param game pointer to an object of game struct
 */
void positionInit(Game* game);

/**
 * @brief Resets the choose your character plaque back to the default character. Resets the distances (jump and run distance) back to the default.
 * @param game pointer to an object of game struct
 */
void playAgain(Game* game);

/**
 * @brief Updates every bitmap to the next positions
 * @param game pointer to an object of game struct
 * @param gameStates the struct with the current states in the two state machines
 */
void updateFrame(Game* game, States* gameStates);

/**
 * @brief Resets the plaque and the character to the default
 * @param game pointer to an object of game struct
 */
void defaultPlayer(Game* game);

/**
 * @brief Updates the character depending on the player option. If option is 1, the character would be the deer, if 2 the santa claus, if 3 the snowman.
 * @param game pointer to an object of game struct
 * @param option int of the character option.
 */
void changePlayer(Game* game, int option);

/**
 * @brief Updates the plaque position depending on the player option. If option is 1, the plaque will move to the deer, if 2 the santa claus, if 3 the snowman.
 * @param option int of the character option.
 */
void drawPlaque(int option);

/**
 * @brief Converts the number passed as argument to the right bitmap number
 * @param game pointer to an object of game struct
 * @param number int meant to be converted
 * @return bitmap of the number
 */
Bitmap* numberToBitmap(Game* game, int number);

/**
 * @brief Updates score bitmaps with its value
 * @param game pointer to an object of game struct
 */
void scoreCounter(Game* game);

/**
 * @brief Updates the character position when it jumps
 */
int jump();

/**
 * @brief Updates the character position when it gets down
 */
void movedown();

/**
 * @brief Updates the obstacles positions ang gets the next set of obstacles ready which is chosen randomly out of 3. It's also chosen randomly if there's a snowball in the next set or not.
 */
void obj();

/**
 * @brief Checks if there's a collision betwwen the character and the obstacles
 * @param gameStates the struct with the current states in the two state machines
 * @return -1 if collision, 0 if not
 */
int collision(States* gameStates);

/**
 * @brief Updates the mouse cursor position
 */
void drawMouse();

/**
 * @brief Checks if the player chose an option in the Main Menu
 * @return 1 if the player chose Play, 2 if the player chose Exit, 3 if the player chose Options, 0 if nothing was chosen
 */
int clicksMainMenu();

/**
 * @brief Checks if the player chose an option in the Options Menu
 * @return 1 if the player chose the deer, 2 if the player chose the Santa Claus , 3 if the player chose the snowman, 0 if nothing was chosen
 */
int clicksOptionsMenu();

/**
 * @brief Checks if the player chose an option in the Game Over Menu
 * @return 1 if the player chose Home, 2 if the player chose Play Again, 0 if nothing was chosen
 */
int clicksGameOver();

/**
 * @brief Extracts date and hour from the RTC
 */
void updateDateHour(Game* game);

/**
 * @brief Counts how many days there is until Christmas
 * @return the number of days until Christmas
 */
int daysLeft(Game* game);

/**
 * @brief Calculates the number of days from 1/1/0001 until the date passed in the arguments.
 * @param y year of the date
 * @param m month of the date
 * @param d day of the date
 * @return the number of days from 1/1/0001 until the date passed in the arguments
 */
int rdn(unsigned long y, unsigned long m, unsigned long d);

/**
 * @brief Calculates the number of seconds until the next minute
 * @param game pointer to an object of game struct
 * @return number of seconds until the next minute
 */
int secondsLeft(Game* game);

/**
 * @brief Calculates the number of minutes until the next hour
 * @param game pointer to an object of game struct
 * @return number of minutes until the next hour
 */
int minutesLeft(Game* game);

/**
 * @brief Calculates the number of hours until the next day
 * @param game pointer to an object of game struct
 * @return number of hours until the next day
 */
int hoursLeft(Game* game);

/**
 * @brief Updates the value of the days, hours, minutes and seconds of the countdown
 * @param game pointer to an object of game struct
 */
void countDown(Game* game);

/**
 * @brief Updates the bitmaps of the countdown
 * @param game pointer to an object of game struct
 */
void countDownDraw(Game* game);

/** @} end of Graphics */

#endif /* GRAPHICS_H */
