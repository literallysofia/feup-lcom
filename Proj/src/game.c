#include <minix/drivers.h>
#include "game.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "xmasrun.h"
#include "i8042.h"

Game* startGame() {
	Game* game = (Game*) malloc(sizeof(Game));

	game->irq_set_timer = timer_subscribe_int();
	game->irq_set_mouse = mouse_subscribe_int();

	write_Controller(MOUSE_WRITE);
	mouse_cmd(MOUSE_EN_DATA);
	kbd_discard();
	game->irq_set_kbd = kbd_subscribe_int();

	game->intcounter = 0;
	game->scan = 0;

	game->start = loadBitmap("/home/proj/res/menu.bmp");
	game->gameover = loadBitmap("/home/proj/res/menu2.bmp");
	game->options = loadBitmap("/home/proj/res/menuoptions.bmp");
	game->background = loadBitmap("/home/proj/res/background2.bmp");

	game->deer = loadBitmap("/home/proj/res/deer.bmp");
	game->deer2 = loadBitmap("/home/proj/res/deer2.bmp");
	game->deerdown = loadBitmap("/home/proj/res/deerdown.bmp");
	game->santa = loadBitmap("/home/proj/res/santa.bmp");
	game->santa2 = loadBitmap("/home/proj/res/santa2.bmp");
	game->santadown = loadBitmap("/home/proj/res/santadown.bmp");
	game->snowman = loadBitmap("/home/proj/res/snowman.bmp");
	game->snowman2 = loadBitmap("/home/proj/res/snowman2.bmp");
	game->snowmandown = loadBitmap("/home/proj/res/snowmandown.bmp");

	game->gift = loadBitmap("/home/proj/res/gift.bmp");
	game->candy = loadBitmap("/home/proj/res/candy.bmp");
	game->snowball = loadBitmap("/home/proj/res/snowball.bmp");
	game->trees = loadBitmap("/home/proj/res/trees.bmp");
	game->mouse = loadBitmap("/home/proj/res/cursor.bmp");
	game->clouds = loadBitmap("/home/proj/res/clouds.bmp");
	game->plaque = loadBitmap("/home/proj/res/plaque.bmp");

	game->zero = loadBitmap("/home/proj/res/0.bmp");
	game->one = loadBitmap("/home/proj/res/1.bmp");
	game->two = loadBitmap("/home/proj/res/2.bmp");
	game->three = loadBitmap("/home/proj/res/3.bmp");
	game->four = loadBitmap("/home/proj/res/4.bmp");
	game->five = loadBitmap("/home/proj/res/5.bmp");
	game->six = loadBitmap("/home/proj/res/6.bmp");
	game->seven = loadBitmap("/home/proj/res/7.bmp");
	game->eight = loadBitmap("/home/proj/res/8.bmp");
	game->nine = loadBitmap("/home/proj/res/9.bmp");

	game->score = loadBitmap("/home/proj/res/score.bmp");
	game->until = loadBitmap("/home/proj/res/until.bmp");

	game->day = (unsigned long*) malloc(sizeof(unsigned long));
	game->month = (unsigned long*) malloc(sizeof(unsigned long));
	game->year = (unsigned long*) malloc(sizeof(unsigned long));

	game->hour = (unsigned long*) malloc(sizeof(unsigned long));
	game->minutes = (unsigned long*) malloc(sizeof(unsigned long));
	game->seconds = (unsigned long*) malloc(sizeof(unsigned long));

	return game;
}

void interruptHandler(Game* game, States* gameStates) {
	int ipc_status;
	message msg;
	int r;
	int counter = game->intcounter;
	int pCounter = 0;
	unsigned long pByte;

	while (gameStates->gameState != COMP) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			return;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & game->irq_set_timer) {
					game->intcounter = counter++;
					timeManager(game, gameStates);
				}

				if (msg.NOTIFY_ARG & game->irq_set_kbd) { /* subscribed interrupt */
					game->scan = readData();
					kbdManager(game, gameStates);
				}

				if (msg.NOTIFY_ARG & game->irq_set_mouse) {

					sys_inb(OUT_BUF, &pByte);
					game->packet[pCounter] = pByte;

					if ((game->packet[0] & BIT(3)) == 0) {
						pCounter = 0;
					} else {
						if ((pCounter == 2)) {
							mouseManager(game, gameStates);
							pCounter = 0;
						} else

							pCounter++;
					}

				}

				break;
			default:
				break; /* no other notifications expected: do*/
			}
		}
	}

	return;
}

void deleteBitmaps(Game* game) {

	deleteBitmap(game->start);
	deleteBitmap(game->gameover);
	deleteBitmap(game->options);
	deleteBitmap(game->background);
	deleteBitmap(game->deer);
	deleteBitmap(game->deer2);
	deleteBitmap(game->deerdown);
	deleteBitmap(game->santa);
	deleteBitmap(game->santa2);
	deleteBitmap(game->santadown);
	deleteBitmap(game->snowman);
	deleteBitmap(game->snowman2);
	deleteBitmap(game->snowmandown);
	deleteBitmap(game->gift);
	deleteBitmap(game->candy);
	deleteBitmap(game->snowball);
	deleteBitmap(game->trees);
	deleteBitmap(game->mouse);
	deleteBitmap(game->clouds);
	deleteBitmap(game->plaque);
	deleteBitmap(game->zero);
	deleteBitmap(game->one);
	deleteBitmap(game->two);
	deleteBitmap(game->three);
	deleteBitmap(game->four);
	deleteBitmap(game->five);
	deleteBitmap(game->six);
	deleteBitmap(game->seven);
	deleteBitmap(game->eight);
	deleteBitmap(game->nine);
	deleteBitmap(game->score);
	deleteBitmap(game->until);

	return;
}

void leaveGame(Game* game, States* gameStates) {

	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	mouse_unsubscribe_int();

	deleteBitmaps(game);
	deleteGameStates(gameStates);

	free(game);

	return;
}
