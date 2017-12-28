#include "gameFunc.h"

void drawText(SDL_Renderer* renderer, char* string, int size, int line, int blank, int r, int g, int b) {
	TTF_Font* font = TTF_OpenFont("arial.ttf", size);
	if (!font) {
		printf("TTF_OpenFont error: %s\n", TTF_GetError());
	}

	SDL_Color fontColor = { r, g, b };

	SDL_Surface *textSurface = TTF_RenderText_Blended(font, string, fontColor);

	SDL_Texture* text1 = SDL_CreateTextureFromSurface(renderer, textSurface);
	//SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
	SDL_Rect rect_d = { blank, line * 10, };
	TTF_SizeText(font, string, &rect_d.w, &rect_d.h);

	SDL_RenderCopy(renderer, text1, NULL, &rect_d);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(text1);
	TTF_CloseFont(font);
}

int rtnRandomNumber(int range, int min_num) {
	int num;
	num = rand() % range + min_num;
	return num;
}

bool charCollision(Player player, SDL_Rect rect) {
	if ((player.pos.x+player.pos.w>rect.x)&&(player.pos.x<rect.x+rect.w)) {
		if ((player.pos.y>rect.y)&&(player.pos.y<rect.y+rect.h)) {
			return true;
		}
	}
	return false;
}

bool hitMark(Monster monster, Bullet bullet) {
	if (monster.pos.x<bullet.pos.x) {

		if ((monster.pos.y<bullet.pos.y)&&(monster.pos.y+monster.pos.h>bullet.pos.y)) {
			return true;
		}
	}
	return false;
}

bool isPlayerHitByBullet(Player player, Bullet bullet) {
	SDL_Rect ppos = player.pos;
	ppos.x += (ppos.w)/2;
	ppos.y += (ppos.h)/2;
	SDL_Rect bpos = bullet.pos;
	bpos.x += (bpos.w) / 2;
	bpos.y += (bpos.h) / 2;
	if (calcDistance(ppos,bpos)<=15) {
		//printf("You're Dead!\n");
		return true;
	}
	else
		return false;
}

//Return sin(o) & cos(o)
Vector findUnitVector(Player player, Vector mousePos) {
	Vector subVector = { 0,0 };
	Vector unitVector;
	double speed = bulletSpeed;
	subVector.x = (double)mousePos.x - (double)player.pos.x;
	subVector.y = (double)mousePos.y - (double)player.pos.y;
	double svNorm = calcNorm(subVector);
	unitVector.x = subVector.x / svNorm;
	unitVector.y = subVector.y / svNorm;

	return unitVector;
}

double calcDistance(SDL_Rect v1, SDL_Rect v2) {
	return sqrt((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
}

double calcNorm(Vector v1) {
	return sqrt((v1.x)*(v1.x) + (v1.y)*(v1.y));
}
