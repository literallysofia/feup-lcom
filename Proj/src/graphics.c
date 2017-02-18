#include "stateMachine.h"
#include "graphics.h"
#include "video_gr.h"
#include "timer.h"
#include "xmasrun.h"
#include "i8042.h"

static int jumpDistance = 168;
static int runDistance = 10;
static int downDistance = 10;
static int up = 1;
static int down = 1;
static int frame = 1;

void positionMouseInit(Game* game) {

	mouse.x = 500;
	mouse.y = 500;
	mouse.width = game->mouse->bitmapInfoHeader.width;
	mouse.height = game->mouse->bitmapInfoHeader.height;

}

void positionInit(Game* game) {

	present.x = H_RES + 696;
	present.y = V_RES - 220;
	present.width = game->gift->bitmapInfoHeader.width;
	present.height = game->gift->bitmapInfoHeader.height;

	candyCane.x = H_RES + 1200;
	candyCane.y = V_RES - 240;
	candyCane.width = game->candy->bitmapInfoHeader.width;
	candyCane.height = game->candy->bitmapInfoHeader.height;

	pine.x = H_RES + 2000;
	pine.y = V_RES - 240;
	pine.width = game->trees->bitmapInfoHeader.width;
	pine.height = game->trees->bitmapInfoHeader.height;

	ball.x = H_RES + 200;
	ball.y = 450;
	ball.width = game->snowball->bitmapInfoHeader.width;
	ball.height = game->snowball->bitmapInfoHeader.height;

	clouds.x = 0;
	clouds.y = 50;
	clouds.width = game->clouds->bitmapInfoHeader.width;
	clouds.height = game->clouds->bitmapInfoHeader.height;

	clouds2.x = H_RES;
	clouds2.y = 50;
	clouds.width = game->clouds->bitmapInfoHeader.width;
	clouds.height = game->clouds->bitmapInfoHeader.height;

	gameScore.pos1 = game->zero;
	gameScore.pos2 = game->zero;
	gameScore.pos3 = game->zero;
	gameScore.pos4 = game->zero;
	gameScore.pos5 = game->zero;
	gameScore.value = 0;
	return;
}

void playAgain(Game* game) {

	player.x = 100;
	player.y = 480;
	dplayer.x = 100;
	dplayer.y = 538;

	jumpDistance = 168;
	runDistance = 10;
	downDistance = 10;
	up = 1;
	down = 1;

	frame = 1;
	return;
}

void updateFrame(Game* game, States* gameStates) {

	if (gameStates->gameState == MAIN_MENU) {
		callBitmap(game->start, 0, 0);

		//countdown

		callBitmap(game->until, 182, 698);

		callBitmap(countdown.pos1S, 684, 694);
		callBitmap(countdown.pos2S, 664, 694);
		callBitmap(countdown.pos1M, 498, 694);
		callBitmap(countdown.pos2M, 478, 694);
		callBitmap(countdown.pos1H, 347, 694);
		callBitmap(countdown.pos2H, 327, 694);
		callBitmap(countdown.pos1D, 212, 694);
		callBitmap(countdown.pos2D, 192, 694);
		callBitmap(countdown.pos3D, 172, 694);

		callBitmap(game->mouse, mouse.x, mouse.y);
	}

	else if (gameStates->gameState == GAME) {

		callBitmap(game->background, 0, 0);
		callBitmap(game->gift, present.x, present.y);
		callBitmap(game->candy, candyCane.x, candyCane.y);
		callBitmap(game->trees, pine.x, pine.y);
		callBitmap(game->snowball, ball.x, ball.y);
		callBitmap(game->snowball, ball.x, ball.y);
		callBitmap(game->clouds, clouds.x, clouds.y);
		callBitmap(game->clouds, clouds2.x, clouds2.y);

		if (gameStates->playerState == JUMP)
			callBitmap(player.bmp1, player.x, player.y);
		else {

			if (gameStates->playerState != DOWN) {

				if (frame == 1) {
					callBitmap(player.bmp1, player.x, player.y);
					if (game->intcounter % 10 == 0)
						frame = 0;
				} else {
					callBitmap(player.bmp2, player.x, player.y);
					if (game->intcounter % 10 == 0)
						frame = 1;
				}

			} else
				callBitmap(dplayer.bmp, dplayer.x, dplayer.y);
		}

		//score
		callBitmap(game->score, 750, 50);
		callBitmap(gameScore.pos1, 960, 50);
		callBitmap(gameScore.pos2, 940, 50);
		callBitmap(gameScore.pos3, 920, 50);
		callBitmap(gameScore.pos4, 900, 50);
		callBitmap(gameScore.pos5, 880, 50);

	}

	else if (gameStates->gameState == GAMEOVER) {
		callBitmap(game->gameover, 0, 0);

		//final score
		callBitmap(game->score, 380, 350);
		callBitmap(gameScore.pos1, 590, 350);
		callBitmap(gameScore.pos2, 570, 350);
		callBitmap(gameScore.pos3, 550, 350);
		callBitmap(gameScore.pos4, 530, 350);
		callBitmap(gameScore.pos5, 510, 350);

		callBitmap(game->mouse, mouse.x, mouse.y);
	}

	else if (gameStates->gameState == MENU_OPTIONS) {
		callBitmap(game->options, 0, 0);
		callBitmap(game->plaque, plaque.x, plaque.y);

		callBitmap(game->mouse, mouse.x, mouse.y);
	}

	return;
}

void defaultPlayer(Game* game) {
	plaque.x = 150;
	plaque.y = 345;

	player.x = 100;
	player.y = 480;
	player.bmp1 = game->deer;
	player.bmp2 = game->deer2;
	player.width = game->deer->bitmapInfoHeader.width;
	player.height = game->deer->bitmapInfoHeader.height;

	dplayer.x = 100;
	dplayer.y = 538;
	dplayer.bmp = game->deerdown;
	dplayer.width = game->deerdown->bitmapInfoHeader.width;
	dplayer.height = game->deerdown->bitmapInfoHeader.height;

	return;
}

void changePlayer(Game* game, int option) {

	switch (option) {

	case 1:
		player.x = 100;
		player.y = 480;
		player.bmp1 = game->deer;
		player.bmp2 = game->deer2;
		player.width = game->deer->bitmapInfoHeader.width;
		player.height = game->deer->bitmapInfoHeader.height;

		dplayer.x = 100;
		dplayer.y = 538;
		dplayer.bmp = game->deerdown;
		dplayer.width = game->deerdown->bitmapInfoHeader.width;
		dplayer.height = game->deerdown->bitmapInfoHeader.height;
		break;
	case 2:
		player.x = 100;
		player.y = 480;
		player.bmp1 = game->santa;
		player.bmp2 = game->santa2;
		player.width = game->santa->bitmapInfoHeader.width;
		player.height = game->santa->bitmapInfoHeader.height;

		dplayer.x = 100;
		dplayer.y = 538;
		dplayer.bmp = game->santadown;
		dplayer.width = game->santadown->bitmapInfoHeader.width;
		dplayer.height = game->santadown->bitmapInfoHeader.height;
		break;
	case 3:
		player.x = 100;
		player.y = 480;
		player.bmp1 = game->snowman;
		player.bmp2 = game->snowman2;
		player.width = game->snowman->bitmapInfoHeader.width;
		player.height = game->snowman->bitmapInfoHeader.height;

		dplayer.x = 100;
		dplayer.y = 538;
		dplayer.bmp = game->snowmandown;
		dplayer.width = game->snowmandown->bitmapInfoHeader.width;
		dplayer.height = game->snowmandown->bitmapInfoHeader.height;
		break;

	default:
		break;
	}

	return;
}

void drawPlaque(int option) {

	switch (option) {

	case 1:
		plaque.x = 150;
		plaque.y = 345;
		break;
	case 2:
		plaque.x = 463;
		plaque.y = 345;
		break;
	case 3:
		plaque.x = 776;
		plaque.y = 345;
		break;
	default:
		break;
	}

}

Bitmap* numberToBitmap(Game* game, int number) {

	switch (number) {

	case 0:
		return game->zero;
	case 1:
		return game->one;
	case 2:
		return game->two;
	case 3:
		return game->three;
	case 4:
		return game->four;
	case 5:
		return game->five;
	case 6:
		return game->six;
	case 7:
		return game->seven;
	case 8:
		return game->eight;
	case 9:
		return game->nine;
	}

}

void scoreCounter(Game* game) {
	gameScore.value++;

	int pos1 = gameScore.value % 10;
	int pos2 = (gameScore.value % 100) / 10;
	int pos3 = (gameScore.value % 1000) / 100;
	int pos4 = (gameScore.value % 10000) / 1000;
	int pos5 = (gameScore.value % 100000) / 10000;

	gameScore.pos1 = numberToBitmap(game, pos1);
	gameScore.pos2 = numberToBitmap(game, pos2);
	gameScore.pos3 = numberToBitmap(game, pos3);
	gameScore.pos4 = numberToBitmap(game, pos4);
	gameScore.pos5 = numberToBitmap(game, pos5);

	return;
}

int jump() {

	float pixelInt = 6; //velocidade

	if (up == 1) {
		if (jumpDistance != 0) {
			player.y -= pixelInt;
			jumpDistance -= pixelInt;

		} else
			up = 0;

		return 1;

	} else if (up == 0) {
		if (jumpDistance != 168) {
			player.y += pixelInt;
			jumpDistance += pixelInt;

		} else
			up = 2; //acaba o jump

		return 1;
	} else {
		up = 1;
		return 0;
	}
}

void movedown() {

	float pixelInt = 1; //velocidade

	if (down == 1) {
		if (downDistance != 0) {
			dplayer.y -= pixelInt;
			downDistance -= pixelInt;

		} else
			down = 0;

	} else if (down == 0) {
		if (downDistance != 10) {
			dplayer.y += pixelInt;
			downDistance += pixelInt;

		} else
			down = 2; //acaba o jump
	} else
		down = 1;

	return;
}

void obj() {
	float pixelInt = 8;
	float pixelIntFast = 12;

	int r;

	r = rand() % 2;
	int g, c, p;

	if (r == 0) {
		g = 100;
		c = 1040;
		p = 1700;
	}

	if (r == 1) {
		p = 100;
		g = 960;
		c = 1480;
	}

	if (r == 2) {
		c = 100;
		p = 1480;
		g = 2400;
	}

	present.x -= pixelInt;
	candyCane.x -= pixelInt;
	pine.x -= pixelInt;
	ball.x -= pixelIntFast;

	if (present.x < -96 && candyCane.x < -96 && pine.x <= -96) {
		present.x = H_RES + g;
		candyCane.x = H_RES + c;
		pine.x = H_RES + p;
	}

	int r2;
	r2 = rand() % 1;

	if (r2 == 0) {
		if (present.x < 1000 && candyCane.x < 1000 && pine.x < 1000) {
			ball.x = H_RES;
		}
	}

	float pixelClouds = 1;

	clouds.x -= pixelClouds;
	clouds2.x -= pixelClouds;

	if (clouds.x == 0)
		clouds2.x = H_RES;
	if (clouds2.x == 0)
		clouds.x = H_RES;

	return;
}

int collision(States* gameStates) {

	int x1_deer = player.x;
	int x2_deer = player.x + player.width;
	int y1_deer = player.y;
	int y2_deer = player.y + player.height;

	int x1_ddeer = dplayer.x;
	int x2_ddeer = dplayer.x + dplayer.width;
	int y1_ddeer = dplayer.y;
	int y2_ddeer = dplayer.y + dplayer.height;

	int x1_pres = present.x;
	int x2_pres = present.x + present.width;
	int y1_pres = present.y;
	int y2_pres = present.y + present.height;

	int x1_candy = candyCane.x;
	int x2_candy = candyCane.x + candyCane.width;
	int y1_candy = candyCane.y;
	int y2_candy = candyCane.y + candyCane.height;

	int x1_trees = pine.x;
	int x2_trees = pine.x + pine.width;
	int y1_trees = pine.y;
	int y2_trees = pine.y + pine.height;

	int x1_ball = ball.x;
	int x2_ball = ball.x + ball.width;
	int y1_ball = ball.y;
	int y2_ball = ball.y + ball.height;

	if (gameStates->playerState == JUMP || gameStates->playerState == RUN) {

		if ((x1_pres > x1_deer && x1_pres < x2_deer)
				|| (x2_pres > x1_deer && x2_pres < x2_deer)) {
			if (y2_deer > y1_pres)
				return -1;
		}

		if ((x1_candy > x1_deer && x1_candy < x2_deer)
				|| (x2_candy > x1_deer && x2_candy < x2_deer)) {
			if (y2_deer > y1_candy)
				return -1;
		}

		if ((x1_trees > x1_deer && x1_trees < x2_deer)
				|| (x2_trees > x1_deer && x2_trees < x2_deer)) {
			if (y2_deer > y1_trees)
				return -1;
		}

		if ((x1_ball > x1_deer && x1_ball < x2_deer)
				|| (x2_ball > x1_deer && x2_ball < x2_deer)) {
			return -1;
		}

	}

	if (gameStates->playerState == DOWN) {

		if ((x1_pres > x1_deer && x1_pres < x2_deer)
				|| (x2_pres > x1_deer && x2_pres < x2_deer)) {
			return -1;
		}

		if ((x1_candy > x1_deer && x1_candy < x2_deer)
				|| (x2_candy > x1_deer && x2_candy < x2_deer)) {
			return -1;
		}

		if ((x1_trees > x1_deer && x1_trees < x2_deer)
				|| (x2_trees > x1_deer && x2_trees < x2_deer)) {
			return -1;
		}

	}

	return 0;
}

void drawMouse(Game* game) {

	if (game->packet[0] & X_SIGN)
		mouse.x = mouse.x - ((game->packet[1] ^= 0xFF) + 1);
	else
		mouse.x = mouse.x + game->packet[1];

	if (game->packet[0] & Y_SIGN)
		mouse.y = mouse.y + ((game->packet[2] ^= 0xFF) + 1);
	else
		mouse.y = mouse.y - game->packet[2];

	if (mouse.x < 0)
		mouse.x = 0;
	if (mouse.x > H_RES - mouse.width)
		mouse.x = H_RES - mouse.width;
	if (mouse.y < 0)
		mouse.y = 0;
	if (mouse.y > V_RES - mouse.height)
		mouse.y = V_RES - mouse.height;

	return;

}

int clicksMainMenu() {

	int playX = 118;
	int playY = 411;
	int playWidth = 251;
	int playHeight = 110;

	int exitX = 662;
	int exitY = 409;
	int exitWidth = 251;
	int exitHeigth = 110;

	int optionsX = 392;
	int optionsY = 560;
	int optionsWidth = 250;
	int optionsHeigth = 112;

	if (mouse.x > playX && mouse.x < playX + playWidth) {
		if (mouse.y > playY && mouse.y < playY + playHeight)
			return 1;
	}

	if (mouse.x > exitX && mouse.x < exitX + exitWidth) {
		if (mouse.y > exitY && mouse.y < exitY + exitHeigth)
			return 2;
	}

	if (mouse.x > optionsX && mouse.x < optionsX + optionsWidth) {
		if (mouse.y > optionsY && mouse.y < optionsY + optionsHeigth)
			return 3;
	}

	return 0;

}

int clicksOptionsMenu() {

	int deerX = 116;
	int deerY = 431;
	int deerWidth = 168;
	int deerHeight = 194;

	int noelX = 432;
	int noelY = 431;
	int noelWidth = 168;
	int noelHeight = 194;

	int snowX = 746;
	int snowY = 431;
	int snowWidth = 168;
	int snowHeight = 194;

	int homeX = 431;
	int homeY = 690;
	int homeWidth = 167;
	int homeHeight = 55;

	if (mouse.x > deerX && mouse.x < deerX + deerWidth) {
		if (mouse.y > deerY && mouse.y < deerY + deerHeight)
			return 1;
	}

	if (mouse.x > noelX && mouse.x < noelX + noelWidth) {
		if (mouse.y > noelY && mouse.y < noelY + noelHeight)
			return 2;
	}

	if (mouse.x > snowX && mouse.x < snowX + snowWidth) {
		if (mouse.y > snowY && mouse.y < snowY + snowHeight)
			return 3;
	}

	if (mouse.x > homeX && mouse.x < homeX + homeWidth) {
		if (mouse.y > homeY && mouse.y < homeY + homeHeight)
			return 4;
	}

	return 0;

}

int clicksGameOver() {

	int homeX = 115;
	int homeY = 512;
	int homeWidth = 251;
	int homeHeight = 110;

	int playAgainX = 644;
	int playAgainY = 510;
	int playAgainWidth = 251;
	int playAgainHeigth = 110;

	if (mouse.x > homeX && mouse.x < homeX + homeWidth) {
		if (mouse.y > homeY && mouse.y < homeY + homeHeight)
			return 1;
	}

	if (mouse.x > playAgainX && mouse.x < playAgainX + playAgainWidth) {
		if (mouse.y > playAgainY && mouse.y < playAgainY + playAgainHeigth)
			return 2;
	}

	return 0;

}

void updateDateHour(Game* game) {

	do {
		if (!isRTCUpdating()) {
			getDate(game->day, game->month, game->year);
			getHour(game->hour, game->minutes, game->seconds);
		}

	} while (isRTCUpdating());

	return;
}

int daysLeft(Game* game) {//days left until the next 25

	int days;

	if ((*game->month) == 12 && (*game->day) > 25) {
		days = rdn((*game->year), (*game->month), (*game->day))
				- rdn((*game->year) + 1, 12, 25);
	} else
		days = rdn((*game->year), (*game->month), (*game->day))
				- rdn((*game->year), 12, 25);

	return days;

}

int rdn(unsigned long y, unsigned long m, unsigned long d) {
	//Rata Die algorithm
	//Based on: https://yvdinesh.quora.com/Rata-Die-Algorithm

	if (y < 2000) { //the year of the rtc is just 2 digits
		y = y + 2000;
	}

	if (m < 3)
		y--, m += 12;

	int x = 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5 + d - 306;

	x = x * -1;

	return x;
}

int secondsLeft(Game* game) {//seconds left until the next minute

	return 60 - (*game->seconds);
}

int minutesLeft(Game* game) { //minutes left until the next hour
	return 60 - (*game->minutes);
}

int hoursLeft(Game* game) { //hours left until the next day
	return 24 - (*game->hour);
}

void countDown(Game* game) {

	int d = daysLeft(game);
	int h = hoursLeft(game);
	int m = minutesLeft(game);
	int s = secondsLeft(game);

	if (s > 0 && m != 0) {
		m = m - 1;
	}

	if ((m > 0 && h != 0)||(s > 0 && h != 0)) {
		h = h - 1;
	}

	if ((h > 0 && d != 0)||(m > 0 && d != 0) || (s > 0 && d != 0)){
		d = d - 1;
	}

	countdown.days = d;
	countdown.hours = h;
	countdown.minutes = m;
	countdown.seconds = s;

	return;
}

void countDownDraw(Game* game) {

	int pos1S = countdown.seconds % 10;
	int pos2S = (countdown.seconds % 100) / 10;
	int pos1M = countdown.minutes % 10;
	int pos2M = (countdown.minutes % 100) / 10;
	int pos1H = countdown.hours % 10;
	int pos2H = (countdown.hours % 100) / 10;
	int pos1D = countdown.days % 10;
	int pos2D = (countdown.days % 100) / 10;
	int pos3D = (countdown.days % 1000) / 100;

	countdown.pos1S = numberToBitmap(game, pos1S);
	countdown.pos2S = numberToBitmap(game, pos2S);
	countdown.pos1M = numberToBitmap(game, pos1M);
	countdown.pos2M = numberToBitmap(game, pos2M);
	countdown.pos1H = numberToBitmap(game, pos1H);
	countdown.pos2H = numberToBitmap(game, pos2H);
	countdown.pos1D = numberToBitmap(game, pos1D);
	countdown.pos2D = numberToBitmap(game, pos2D);
	countdown.pos3D = numberToBitmap(game, pos3D);

	return;

}

