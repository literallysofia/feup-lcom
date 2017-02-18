#ifndef XMASRUN_H
#define XMASRUN_H

/** @defgroup XMasRun XMasRun
 * @{
 */

#include "game.h"
#include "stateMachine.h"

/**
 * @brief Handles everything related to the keyboard interrupts
 * @param game pointer to an object of game struct
 * @param gameStates the struct with the current states in the two state machines
 */
void kbdManager(Game* game, States* gameStates);

/**
 * @brief Handles everything related to the timer interrupts
 * @param game pointer to an object of game struct
 * @param gameStates the struct with the current states in the two state machines
 */
void timeManager(Game* game, States* gameStates);

/**
 * @brief Handles everything related to the mouse interrupts
 * @param game pointer to an object of game struct
 * @param gameStates the struct with the current states in the two state machines
 */
void mouseManager(Game* game, States* gameStates);

/** @} end of XMasRun */

#endif /* XMASRUN_H */
