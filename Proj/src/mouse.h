#ifndef MOUSE_H
#define MOUSE_H

/** @defgroup Mouse Mouse
 * @{
 */

/**
 * @brief Subscribes and enables mouse interrupts.
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribes mouse interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int();

/**
 * @brief Waits 20 microseconds.
 */
void ddelay();

/**
 * @brief Reads the output buffer
 * @return Return data, and -1 if it was not successfull
 */
int mouse_Read();

/**
 * @brief Writes in the mouse
 * @param cmd command to write in the mouse
 * @param port to write in the mouse
 * @return data, and -1 if it was not successfull
 */
int mouse_Write(unsigned long cmd, unsigned long port);

/**
 * @brief Calls mouse_Writer, used to send the cmd to write in the mouse
 * @param cmd command to write in the mouse
 * @return 0 upon success and non-zero otherwise
 */
int write_Controller(unsigned long cmd);

/**
 * @brief Writes in the mouse, verifying the ACK of the mouse
 * @param cmd command to write in the mouse
 * @return 0 upon success and non-zero otherwise
 */
int mouse_cmd(unsigned long cmd);

/** @} end of Mouse */

#endif /* MOUSE_H */
