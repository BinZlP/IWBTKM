#include <stdio.h>
#include <SDL.h>
#include "gameFunc.h"

const int bullet_number_skillused = 10;

int main(void) {
	//int num;
	//Bullet* malloc_bullets = (Bullet*)malloc(5, sizeof(Bullet));
	//initBullet(malloc_bullets, 5);

	//for (int i = 0; i < 5; i++) {
	//	printf("%d %d %d\n",i,malloc_bullets[i].pos.x,malloc_bullets[i].pos.y);
	//}

	//scanf("%d",&num);               

	//int num;
	//Material player;
	//Vector mousePos;
	//Vector unitVector;
	//player.pos.x = 0;
	//player.pos.y = 0;
	//mousePos.x = 3;
	//mousePos.y = 4;
	//printf("calcNorm: %lf\n", calcNorm(mousePos));
	//unitVector = findUnitVector(player, mousePos);
	//printf("cos: %lf\nsin: %lf\n", unitVector.x, unitVector.y);



	/*int num;
	Material player;
	player.status.useSkill = true;

	if (SDL_Init(SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}
	
	printf("%d\n", player.status.useSkill);
	Bullet* testarr = (Bullet*)calloc(5, sizeof(Bullet));
	SDL_TimerID testTimer = SDL_AddTimer(10 * 1000, skillOff, testarr, &player.status);
	*/

	int num;

	scanf("%d", &num);
	return 0;
}