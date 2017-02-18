#ifndef KEYBOARD_H
#define KEYBOARD_H

int kbd_subscribe_int(void);
int kbd_unsubscribe_int();

void delay();

int readData();

int kbd_Read();
int kbd_Write();

#endif /* KEYBOARD_H */
