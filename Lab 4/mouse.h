#ifndef MOUSE_H
#define MOUSE_H

int mouse_subscribe_int();
int mouse_unsubscribe_int();
void delay();
int mouse_Read();
int mouse_Write(unsigned long cmd, unsigned long port);
int write_Controller(unsigned long cmd);
int mouse_cmd(unsigned long cmd);

#endif /* MOUSE_H */
