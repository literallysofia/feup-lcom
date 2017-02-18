#ifndef STATEMACHINE_H
#define STATEMACHINE_H

/** @defgroup StateMachine StateMachine
 * @{
 */

/// Represents the menus
typedef enum {
	MAIN_MENU, GAME, GAMEOVER, MENU_OPTIONS, COMP
} state_game;

/// Represents the transitions between menus
typedef enum {
	PLAY, HOME, COLLISION, PLAYAGAIN, OPTIONS, END, ESC
} event_game;

/// Represents the character states
typedef enum {
	RUN, JUMP, DOWN
} state_t;

/// Represents trasitions between the character states
typedef enum {
	UP, LOW, BACK
} event_t;

/// Represents the current states in the two state machines
typedef struct {
	state_game gameState;
	state_t playerState;
} States;

/**
 * @brief Initializes both state machines
 * @return the struct with the current states in the two state machines
 */
States* newGameStates();

/**
 * @brief Updates the state related to the menu
 * @param evt the transition to the next state
 * @param gameStates the struct with the current states in the two state machines
 */
void check_menu(event_game evt, States* gameStates);

/**
 * @brief Updates the state related to the character
 * @param evt the transition to the next state
 * @param gameStates the struct with the current states in the two state machines
 */
void check_move(event_t evt, States* gameStates);

/**
 * @brief Frees the space allocated for the state machine
 * @param gameStates the struct with the current states in the two state machines
 */
void deleteGameStates(States* gameStates);

/** @} end of StateMachine */

#endif /* STATEMACHINE_H */
