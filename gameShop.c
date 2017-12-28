#include "gameFunc.h"
#include "gameMove.h"


int rtnPrice(int whatthing, int level);

void enterShop(SDL_Renderer* renderer,int* MONEY, int* BULLET_DAMAGE, int* SKILL_DAMAGE, int* CHARGE_RATE, double* REWARD_UP) {
	
	//Load texture
	SDL_Texture* background_shop = loadTexture("test_background.png", renderer);
	if (background_shop == NULL) {
		return;
	}

	SDL_Texture* gunUpgrade = loadTexture("gunUpgrade.png", renderer);
	if (gunUpgrade == NULL) {
		return;
	}

	SDL_Texture* skillUpgrade = loadTexture("skillUpgrade.png", renderer);
	if (skillUpgrade == NULL) {
		return;
	}

	SDL_Texture* chargeUpgrade = loadTexture("chargeUpgrade.png", renderer);
	if (chargeUpgrade == NULL) {
		return;
	}

	SDL_Texture* rewardUpgrade = loadTexture("rewardUpgrade.png", renderer);
	if (rewardUpgrade == NULL) {
		return;
	}


	SDL_Event e;

	//Game variables
	SDL_Rect upsideBox = { 0,0,SCREEN_WIDTH,40 };
	SDL_Rect bottomBox = { 0,440,SCREEN_WIDTH,40 };

	SDL_Rect backBox = { 20,2,78,36 };
	SDL_Rect upgradeImageBox[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			upgradeImageBox[i][j].x = 70 + 300 * j;
			upgradeImageBox[i][j].y = 60 + 200 * i;
			upgradeImageBox[i][j].w = 160;
			upgradeImageBox[i][j].h = 120;
		}
	}
	SDL_Rect upgradeBox[2][2];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			upgradeBox[i][j].x = 70 + 300 * j;
			upgradeBox[i][j].y = 190 + 200 * i;
			upgradeBox[i][j].w = 160;
			upgradeBox[i][j].h = 50;
		}
	}


	Vector mousePos;
	int mouse_xpos = 0;
	int mouse_ypos = 0;

	int gun_up_price, skill_up_price, charge_up_price, reward_up_price;


	char cur_gold[100] = "";
	char info[255] = "Welcome to the shop!";
	char status[255] = "";
	
	char gun_upbutton[100] = "";
	char skill_upbutton[100] = "";
	char charge_upbutton[100] = "";
	char reward_upbutton[100] = "";

	Uint32 upgradeDuration = SDL_GetTicks() + 500;
	bool justupgrade = false;

	bool shop_quit = false;

	while (!shop_quit) {
		
		int gun_level = *BULLET_DAMAGE;
		int skill_level = *SKILL_DAMAGE / 5;
		int charge_level = *CHARGE_RATE;
		int reward_level = (*REWARD_UP * 10)/2 - 4;

		gun_up_price = rtnPrice(1, gun_level);
		skill_up_price = rtnPrice(2, skill_level);
		charge_up_price = rtnPrice(3, charge_level);
		reward_up_price = rtnPrice(4, reward_level);

		sprintf(gun_upbutton, "LV %d : %d G", gun_level+1, gun_up_price);
		sprintf(skill_upbutton, "LV %d : %d G", skill_level+1, skill_up_price);
		sprintf(charge_upbutton, "LV %d : %d G", charge_level+1, charge_up_price);
		sprintf(reward_upbutton, "LV %d : %d G", reward_level+1, reward_up_price);

		sprintf(status, "[Status] Gun: %d, Skill: %d, Charge: %d, Reward: %.1lf", *BULLET_DAMAGE, *SKILL_DAMAGE, *CHARGE_RATE, *REWARD_UP);

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT) {
				shop_quit = true;
			}
			else if (e.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mouse_xpos, &mouse_ypos);
				mousePos.x = (double)mouse_xpos;
				mousePos.y = (double)mouse_ypos;
				//printf("%lf %lf\n", mousePos.x, mousePos.y);



			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 2; j++) {
						if (mousePos.x >= upgradeBox[i][j].x&&mousePos.x <= upgradeBox[i][j].x + upgradeBox[i][j].w) {
							if (mousePos.y >= upgradeBox[i][j].y&&mousePos.y <= upgradeBox[i][j].y + upgradeBox[i][j].h) {
								if (!justupgrade) {
									int uptype = i * 2 + j + 1;
									switch (uptype) {
									case 1:
										if (*MONEY >= gun_up_price) {
											*MONEY -= gun_up_price;
											*BULLET_DAMAGE += 1;
											sprintf(info, "Bullet's damage is increased to %d.", *BULLET_DAMAGE);
											upgradeDuration = SDL_GetTicks() + 500;
											justupgrade = true;
										}
										else {
											sprintf(info, "You don't have enough money.");
										}
										break;

									case 2:
										if (*MONEY >= skill_up_price) {
											*MONEY -= skill_up_price;
											*SKILL_DAMAGE += 5;
											sprintf(info, "Skill damage is increased to %d.", *SKILL_DAMAGE);
											upgradeDuration = SDL_GetTicks() + 500;
											justupgrade = true;
										}
										else {
											sprintf(info, "You don't have enough money.");
										}
										break;

									case 3:
										if (*MONEY >= charge_up_price) {
											*MONEY -= charge_up_price;
											*CHARGE_RATE += 1;
											sprintf(info, "Charge rate is increased to %d.", *CHARGE_RATE);
											upgradeDuration = SDL_GetTicks() + 500;
											justupgrade = true;
										}
										else {
											sprintf(info, "You don't have enough money.");
										}
										break;

									case 4:
										if (*MONEY >= reward_up_price) {
											*MONEY -= reward_up_price;
											*REWARD_UP += 0.2;
											sprintf(info, "Now you can earn %.1lfx reward.", *REWARD_UP);
											upgradeDuration = SDL_GetTicks() + 500;
											justupgrade = true;
										}
										else {
											sprintf(info, "You don't have enough money.");
										}

									}
								}
								
							}
						}
					}
				}
				if (mousePos.x >= backBox.x&&mousePos.x <= backBox.x + backBox.w) {
					if (mousePos.y >= backBox.y&&mousePos.y <= backBox.y + backBox.h) {
						return;
					}
				}
			
			
			}
		}

		//Prevent fast click bug
		if (SDL_TICKS_PASSED(SDL_GetTicks(), upgradeDuration))
			justupgrade = false;

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);




		//******** Render shop *********
		SDL_RenderCopy(renderer, background_shop, NULL, NULL);

		//Draw frame
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &upsideBox);
		SDL_RenderFillRect(renderer, &bottomBox);
		drawText(renderer, "SHOP", 40, 0, 280, 255, 255, 255);

		//Show Information
		sprintf(cur_gold, "GOLD: %d", *MONEY);
		drawText(renderer, cur_gold, 20, 46, 5, 255, 255, 0);
		drawText(renderer, status, 16, 46, 280, 255, 255, 255);
		drawText(renderer, info, 20, 44, 5, 255, 255, 255);

		//Draw shop exit button
		SDL_SetRenderDrawColor(renderer, 150, 150, 255, 255);
		if (mousePos.x >= backBox.x&&mousePos.x <= backBox.x + backBox.w) {
			if (mousePos.y >= backBox.y&&mousePos.y <= backBox.y + backBox.h) {
				SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
			}
		}//If mouse cursor is on shop button
		SDL_RenderFillRect(renderer, &backBox);
		drawText(renderer, "BACK", 24, 1, 20, 0, 0, 0);


		//Draw shopping contents
		SDL_RenderCopy(renderer, gunUpgrade, NULL, &upgradeImageBox[0][0]);
		SDL_RenderCopy(renderer, skillUpgrade, NULL, &upgradeImageBox[0][1]);
		SDL_RenderCopy(renderer, chargeUpgrade, NULL, &upgradeImageBox[1][0]);
		SDL_RenderCopy(renderer, rewardUpgrade, NULL, &upgradeImageBox[1][1]);

		SDL_SetRenderDrawColor(renderer, 150, 150, 255, 255);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				SDL_RenderFillRect(renderer, &upgradeBox[i][j]);
			}
		}
		SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				if (upgradeBox[i][j].x <= mousePos.x&&upgradeBox[i][j].x + upgradeBox[i][j].w >= mousePos.x) {
					if (upgradeBox[i][j].y <= mousePos.y&&upgradeBox[i][j].y + upgradeBox[i][j].h >= mousePos.y) {
						SDL_RenderFillRect(renderer, &upgradeBox[i][j]);
					}
				}
			}
		}
		
		drawText(renderer, gun_upbutton, 20, 21, 80, 0, 0, 0);
		drawText(renderer, skill_upbutton, 20, 21, 380, 0, 0, 0);
		drawText(renderer, charge_upbutton, 20, 41, 80, 0, 0, 0);
		drawText(renderer, reward_upbutton, 20, 41, 380, 0, 0, 0);



		//Update screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(background_shop);
	SDL_DestroyTexture(gunUpgrade);
	SDL_DestroyTexture(skillUpgrade);
	SDL_DestroyTexture(chargeUpgrade);
	SDL_DestroyTexture(rewardUpgrade);
	return;
}

int rtnPrice(int whatthing, int level) {
	switch (whatthing) {
	case 1: return 500 * level*level*level; break;
	case 2: return 200 * level*level; break;
	case 3: return 2000 * level*level; break;
	case 4: return 5000 * level*level; break;
	}
}