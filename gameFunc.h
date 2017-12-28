#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//#define gravity 0.5
#define charimg 20
#define bulletSpeed 10
#define bulletSize 4

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef enum {
	VERTICAL_LASER,
	HORIZONTAL_LASER,
	BOMBING,
	CROSS_LASER,
	COUNTER_VERTICAL_LASER,
	COUNTER_HORIZONTAL_LASER,
	TRIPLE_CROSS_LASER,
	TIMEBOMB
}AttackType;

typedef struct {
	double x;
	double y;
}Vector;

typedef struct {
	SDL_Rect pos;
	int HP;
	bool isAttack;
	bool readyAttack;
	AttackType whatAttack;
}Monster;

typedef struct {
	int skill_gage;
	bool game_over;
	bool going_up;
	bool going_right;
	bool useSkill;
}PlayerFlag;

typedef struct {
	SDL_Rect pos;
	Vector vel;
	PlayerFlag status;
}Player;

typedef struct {
	SDL_Rect pos;
	Vector vel;
	bool isShot;
}Bullet;

//Functions about game limitation & management
//Uint32 skillOff(Uint32 interval, void* param);

void drawText(SDL_Renderer* renderer, char* string, int size, int line, int blank, int r, int g, int b);

bool charCollision(Player player, SDL_Rect rect);
bool isPlayerHitByBullet(Player player, Bullet bullet);
bool hitMark(Monster monster, Bullet bullet);

int rtnRandomNumber(int range, int min_num);

double calcDistance(SDL_Rect v1, SDL_Rect v2);
double calcNorm(Vector v1);


//Funtions being used in game
void initBullet(Bullet* bullets, int length);
void shootingGun(Bullet* bullets, Player player, Vector mousePos, int length, bool isSkillBullet);
void bulletDestroy(Bullet* bullets, int index, bool isSkillBullet);

Vector findUnitVector(Player player, Vector mousePos);

