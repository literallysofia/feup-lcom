#ifndef I8042_H
#define I8042_H

/* keyboard */

#define KBD_IRQ  1

#define ESC_BREAK 		0x81

#define STAT_REG 		0x64
#define KBC_CMD_REG     0x64
#define IN_BUF		    0x64
#define OUT_BUF 		0x60
#define OUT_BUF_FULL 0x01

#define BIT(n)    (0x01<<(n))
#define TWO_BYTES 		0xe0

#define OBF            BIT(0)
#define IBF            BIT(1)

#define WAIT_KBC 		20000

#define PAR_ERR BIT(7)
#define TO_ERR  BIT(6)

#define LEDS_CMD 		0xED
#define ACK 			0xFA
#define RESEND 			0xFE
#define ERROR 			0xFC

#define TRIES 			5

/* mouse */

#define MOUSE_IRQ  12

#define MOUSE_READ_CMD		0x20
#define MOUSE_WRITE_CMD		0x60
#define MOUSE_DIS			0xA7
#define MOUSE_DIS_DATA		0xF5
#define MOUSE_EN			0xA8
#define MOUSE_EN_DATA		0xF4
#define MOUSE_CHECK			0xA9 //checks interface
#define MOUSE_WRITE			0xD4 //writes byte
#define MOUSE_STATUS_REQ	0xE9

#define L_BUTTON			BIT(0)
#define R_BUTTON			BIT(1)
#define M_BUTTON			BIT(2)
#define X_SIGN				BIT(4)
#define Y_SIGN				BIT(5)
#define X_OVFL				BIT(6)
#define Y_OVFL				BIT(7)


#endif /* I8042_H */
