#ifndef KEYBOARD_H
#define KEYBOARD_H

/** @defgroup Keyboard Keyboard
 * @{
 */

/**
 * @brief Subscribes keyboard interrupts
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbd_subscribe_int(void);

/**
 * @brief Unsubscribes keyboard interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int();

/**
 * @brief Gets the scan code read from the keyboard.
 * @return Returns the scan code read from the keyboard
 */
int readData();

/**
 * @brief Cleans the output of the keyboard buffer.
 */
void kbd_discard();

/**
 * @brief Waits 20 microseconds.
 */
void delay();

/** @} end of Keyboard */

#endif /* KEYBOARD_H */
