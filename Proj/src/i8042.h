#ifndef I8042_H
#define I8042_H

#define KBD_IRQ  1

#define ESC_BREAK 		0x81
#define ENTER_BREAK 	0x9C
#define SPACE_MAKE 		0x39
#define S_MAKE 			0x1F //down
#define S_BREAK 		0x9F
#define W_MAKE 			0x11 //jump

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

// VBE

#define VBE_MODE		0x117

//RTC

#define RTC_SEC					0
#define RTC_MIN					2
#define RTC_HRS					4
#define RTC_DAYMONTH			7
#define RTC_MONTH				8
#define RTC_YEAR				9

#define REGA					10
#define REGB					11
#define REGC					12
#define REGD					13

#define RTC_ADDR_REG			0x70
#define RTC_DATA_REG			0x71

#define RTC_REGA_UIP			BIT(7)
#define RTC_REGB_DM				BIT(2)



#endif /* I8042_H */
