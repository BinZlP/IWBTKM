#pragma once

//MENU
int mainScreen(SDL_Renderer* renderer);
int enterGame(SDL_Renderer* renderer);
void endingCredit(SDL_Renderer* renderer);

//MAP
int map1(SDL_Renderer* renderer, int BULLET_DAMAGE, int SKILL_DAMAGE, int CHARGE_RATE);
int map2(SDL_Renderer* renderer, int BULLET_DAMAGE, int SKILL_DAMAGE, int CHARGE_RATE);
int map3(SDL_Renderer* renderer, int BULLET_DAMAGE, int SKILL_DAMAGE, int CHARGE_RATE);
int map4(SDL_Renderer* renderer, int BULLET_DAMAGE, int SKILL_DAMAGE, int CHARGE_RATE);
int map5(SDL_Renderer* renderer, int BULLET_DAMAGE, int SKILL_DAMAGE, int CHARGE_RATE);
int map6(SDL_Renderer* renderer, int BULLET_DAMAGE, int SKILL_DAMAGE, int CHARGE_RATE);
