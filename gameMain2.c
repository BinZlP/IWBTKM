#include "gameFunc.h"
#include "gameMove.h"


int enterGame(SDL_Renderer* renderer) {
	//Load save file data
	int deathcount = 0, BULLET_DAMAGE = 0, SKILL_DAMAGE = 0, CHARGE_RATE = 0, MONEY = 0;
	double REWARD_UP = 1.;
	int stage_clear[6] = { 0,0,0,0,0,0 };

	FILE* fp = fopen("save.txt", "rt");
	if (fp == NULL) {
		fp = fopen("save.txt", "wt");
		fprintf(fp, "0 1 5 1 0 1.0 0 0 0 0 0 0");
		fclose(fp);
		fp = fopen("save.txt", "rt");
	}

	fscanf(fp, "%d %d %d %d %d %lf %d %d %d %d %d %d",
		&deathcount, &BULLET_DAMAGE, &SKILL_DAMAGE, &CHARGE_RATE, &MONEY, &REWARD_UP,
		&stage_clear[0], &stage_clear[1], &stage_clear[2], &stage_clear[3], &stage_clear[4], &stage_clear[5]);
	fclose(fp);

	printf("************** SAVE FILE LOADED *************\n");
	printf("%d %d %d %d %d %.1lf\n", deathcount, BULLET_DAMAGE, SKILL_DAMAGE, CHARGE_RATE, MONEY, REWARD_UP);
	for (int i = 0; i < 6; i++)
		printf("%d ", stage_clear[i]);
	printf("\n");
	printf("*********************************************\n");



	//Load texture
	SDL_Texture* background_lobby = loadTexture("test_background.png", renderer);
	if (background_lobby == NULL){
		return 1;
	}



	//Event flag & Main loop flag
	SDL_Event e;
	bool game_quit = false;
	int game_clear_flag = 0;

	//Game variables
	SDL_Rect upsideBox = {0,0,SCREEN_WIDTH,40};
	SDL_Rect bottomBox = { 0,440,SCREEN_WIDTH,40 };
	SDL_Rect stageBox[2][3];
	for (int ii = 0; ii < 2; ii++) {
		for (int jj = 0; jj < 3; jj++) {
			stageBox[ii][jj].x = 220 * jj;
			stageBox[ii][jj].y = 210 * ii+40;
			stageBox[ii][jj].w = 200;
			stageBox[ii][jj].h = 190;
		}
	}
	SDL_Rect shopBox = { SCREEN_WIDTH - 80,2,78,36 };
	SDL_Rect saveBox = { SCREEN_WIDTH - 80, 442,78,36 };


	Vector mousePos = { 0,0 };
	int mouse_xpos = 0, mouse_ypos = 0;

	char title[100] = "";
	char failure[100] = "";
	char money[100] = "";
	char reward[100] = "";
	char cleared_time[100] = "";

	char* information[6] = { "Tutorial", "Stage 1", "Stage 2", "Stage 3", "Stage 4", "BOSS" };
	int info_reward[6] = { 500, 2000, 4000, 8000, 50000, 250000 };

	Uint32 showDuration;
	bool push_save = false;
	bool push_inv_button = false;

	//Main loop
	while (!game_quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			game_clear_flag = 0;

			//User requests quit
			if (e.type == SDL_QUIT) {
				game_quit = true;
			}
			else if (e.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mouse_xpos, &mouse_ypos);
				mousePos.x = (double)mouse_xpos;
				mousePos.y = (double)mouse_ypos;
				//printf("%lf %lf\n", mousePos.x, mousePos.y);



			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (mousePos.x >= shopBox.x&&mousePos.x <= shopBox.x + shopBox.w) {
					if (mousePos.y >= shopBox.y&&mousePos.y <= shopBox.y + shopBox.h) {
						enterShop(renderer, &MONEY, &BULLET_DAMAGE, &SKILL_DAMAGE, &CHARGE_RATE, &REWARD_UP);
					}
				}
				if (mousePos.x >= saveBox.x&&mousePos.x <= saveBox.x + saveBox.w) {
					if (mousePos.y >= saveBox.y&&mousePos.y <= saveBox.y + saveBox.h) {
						fp = fopen("save.txt", "wt");
						fprintf(fp, "%d %d %d %d %d %lf %d %d %d %d %d %d",
							deathcount, BULLET_DAMAGE, SKILL_DAMAGE, CHARGE_RATE, MONEY, REWARD_UP,
							stage_clear[0], stage_clear[1], stage_clear[2], stage_clear[3], stage_clear[4], stage_clear[5]
						);
						fclose(fp);
						push_save = true;
						showDuration = SDL_GetTicks() + 1000;
					}
				}
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 3; j++) {
						if (stageBox[i][j].x <= mousePos.x&&stageBox[i][j].x + stageBox[i][j].w >= mousePos.x) {
							if (stageBox[i][j].y <= mousePos.y&&stageBox[i][j].y + stageBox[i][j].h >= mousePos.y) {
								int stage_num = (i  * 3) + j + 1;
								if (stage_clear[stage_num - 2] == 0) {
									push_inv_button = true;
									showDuration = SDL_GetTicks() + 1000;
								}
								else {
									switch (stage_num) {
									case 1: game_clear_flag = map1(renderer, BULLET_DAMAGE, SKILL_DAMAGE, CHARGE_RATE); break;
									case 2: game_clear_flag = map2(renderer, BULLET_DAMAGE, SKILL_DAMAGE, CHARGE_RATE); break;
									case 3: game_clear_flag = map3(renderer, BULLET_DAMAGE, SKILL_DAMAGE, CHARGE_RATE); break;
									case 4: game_clear_flag = map4(renderer, BULLET_DAMAGE, SKILL_DAMAGE, CHARGE_RATE); break;
									case 5: game_clear_flag = map5(renderer, BULLET_DAMAGE, SKILL_DAMAGE, CHARGE_RATE); break;
									case 6: game_clear_flag = map6(renderer, BULLET_DAMAGE, SKILL_DAMAGE, CHARGE_RATE); break;
									}
									mousePos.x = 0;
									mousePos.y = 0;
									//printf("%d\n", game_clear_flag);
									if (game_clear_flag == 1) {
										MONEY += (int)((double)info_reward[stage_num - 1] * REWARD_UP);
										if (i == 1 && j == 2 && stage_clear[5] == 0) {
											endingCredit(renderer);
										}
										stage_clear[stage_num-1] += 1;
									}
									else {
										deathcount++;
									}
								}
							}
						}
					}
				}
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);


		//******** Render lobby *********
		SDL_RenderCopy(renderer, background_lobby, NULL, NULL);


		//Draw frame
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &upsideBox);
		SDL_RenderFillRect(renderer, &bottomBox);
		drawText(renderer, "SELECT STAGE", 40, 0, 175, 255, 255, 255);

		//Show Information
		sprintf(failure, "DEATH: %d", deathcount);
		sprintf(money, "GOLD: %d", MONEY);
		drawText(renderer, money, 20, 0, 5, 255, 255, 0);
		drawText(renderer, failure, 15, 2, 5, 255, 255, 255);

		//Draw shop enter button
		SDL_SetRenderDrawColor(renderer, 150, 150, 255, 255);
		if (mousePos.x >= shopBox.x&&mousePos.x <= shopBox.x + shopBox.w) {
			if (mousePos.y >= shopBox.y&&mousePos.y <= shopBox.y + shopBox.h) {
				SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
			}
		}//If mouse cursor is on shop button
		SDL_RenderFillRect(renderer, &shopBox);
		drawText(renderer, "SHOP", 24, 1, 565, 0, 0, 0);

		//Draw save button
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		if (mousePos.x >= saveBox.x&&mousePos.x <= saveBox.x + saveBox.w) {
			if (mousePos.y >= saveBox.y&&mousePos.y <= saveBox.y + saveBox.h) {
				SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
			}
		}
		SDL_RenderFillRect(renderer, &saveBox);
		drawText(renderer, "SAVE", 24, 45, 570, 0, 0, 0);

		//Draw stage enter button
		SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				SDL_SetRenderDrawColor(renderer, 255, 250-(i*150+j*50), 250 - (i * 150 + j * 50), 255);
				SDL_RenderFillRect(renderer, &stageBox[i][j]);
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				if (stageBox[i][j].x <= mousePos.x&&stageBox[i][j].x + stageBox[i][j].w >= mousePos.x) {
					if (stageBox[i][j].y <= mousePos.y&&stageBox[i][j].y + stageBox[i][j].h >= mousePos.y) {
						int stage_num = (i * 3) + j;
						SDL_RenderFillRect(renderer, &stageBox[i][j]);
						sprintf(title, information[stage_num]);
						sprintf(reward, "REWARD: %.0lf", (double)info_reward[stage_num] * REWARD_UP);
						sprintf(cleared_time, "CLEAR: %d Times", stage_clear[stage_num]);
					}
				}
			}
		}//If mouse cursor is on button

		if (!push_save) {
			if (push_inv_button) {
				if (!SDL_TICKS_PASSED(SDL_GetTicks(), showDuration)) {
					drawText(renderer, "You must clear prev. stage.", 36, 44, 5, 255, 0, 0);
				}
				else {
					push_inv_button = false;
				}
			}
			else {
				drawText(renderer, title, 20, 44, 5, 255, 255, 255);
				drawText(renderer, reward, 20, 46, 5, 255, 255, 255);
				drawText(renderer, cleared_time, 20, 44, 320, 255, 255, 255);
			}
		}
		else {
			if (!SDL_TICKS_PASSED(SDL_GetTicks(), showDuration))
				drawText(renderer, "Saved successfully!", 40, 44, 5, 0, 255, 0);
			else {
				push_save = false;
			}
		}
		
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(background_lobby);
	return 1;
}