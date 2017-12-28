#include "gameFunc.h"
#include "gameMove.h"

int map4(SDL_Renderer* renderer, int BULLET_DAMAGE, int SKILL_DAMAGE, int CHARGE_RATE) {

	//player.status default settings
	const double TIMESTEP = 1 / 60.;
	const double DOT_VEL = 20.;

	const int BULLET_NUMBER_DEFAULT = 5;

	const int MONSTER_WIDTH = 152;
	const int MONSTER_HEIGHT = 209;

	const int laser_thickness = 15;
	const int bomb_radius = 100;

	srand(time(NULL));

	//int mixer_result = 0;
	//int mix_flags = MIX_INIT_MP3;

	//Loading character texture
	SDL_Texture* texture = loadTexture("test_background.png", renderer);
	if (texture == NULL) {
		printf("Failed to load texture image!\n");
		return 2;
	}

	SDL_Texture* player_character = loadTexture("kwman_resized_40.png", renderer);
	if (player_character == NULL) {
		printf("Failed to load texture image!\n");
		SDL_DestroyTexture(texture);
		return 2;
	}

	//Loading Monster Texture
	SDL_Texture* monster_C = loadTexture("C_02.gif", renderer);
	if (player_character == NULL) {
		printf("Failed to load texture image!\n");
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(player_character);
		return 2;
	}

	//Bombing circle texture
	SDL_Texture* bombCircle = loadTexture("circle_red_100.png", renderer);
	if (bombCircle == NULL) {
		printf("Failed to load texture image!\n");
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(player_character);
		SDL_DestroyTexture(monster_C);
		return 2;
	}


	int quit = 0;   //Main loop flag
	SDL_Event e;    //Event

					//Character Position Setting
	Player player;
	player.pos.x = 0;
	player.pos.y = 220;
	player.pos.w = charimg;
	player.pos.h = charimg;
	player.status.game_over = false;
	player.status.going_right = true;
	player.status.going_up = false;
	player.status.skill_gage = 0;
	player.status.useSkill = false;
	player.vel.x = 0;
	player.vel.y = 0;

	//SDL_Rect player_pos = { 0,0,40,40 };
	double posX = 0, posY = 240 - charimg, velX = 0, velY = 0;

	//Bullet array
	Bullet bullets[5];
	initBullet(bullets, BULLET_NUMBER_DEFAULT);
	Bullet* skill_bullet = NULL;

	Bullet bullet1;
	bullet1.pos.x = SCREEN_WIDTH - bulletSize;
	bullet1.pos.y = 60;
	bullet1.pos.w = bulletSize * 2;
	bullet1.pos.h = bulletSize * 2;
	bullet1.vel.x = -bulletSpeed;
	bullet1.vel.y = 0;

	Bullet bullet2;
	bullet2.pos.x = SCREEN_WIDTH - bulletSize;
	bullet2.pos.y = 420;
	bullet2.pos.w = bulletSize * 2;
	bullet2.pos.h = bulletSize * 2;
	bullet2.vel.x = -bulletSpeed;
	bullet2.vel.y = 0;

	SDL_Rect lasers[4];
	for (int i = 0; i < 4; i++) {
		lasers[i].x = 0;
		lasers[i].y = 0;
		lasers[i].w = 0;
		lasers[i].h = 0;
	}

	//Mouse Position
	Vector mousePos;
	int mousePos_x;
	int mousePos_y;

	//Timer Variable
	Uint32 attackDuration;
	Uint32 duration[10];
	for (int i = 0; i < 10; i++)
		duration[i] = 0;
	Uint32 dead_dur;

	//Monster setting
	Monster bookC;
	bookC.pos.x = SCREEN_WIDTH - MONSTER_WIDTH;
	bookC.pos.y = SCREEN_HEIGHT - MONSTER_HEIGHT - 180;
	bookC.pos.w = MONSTER_WIDTH;
	bookC.pos.h = MONSTER_HEIGHT;
	bookC.HP = 5000;
	bookC.isAttack = false;
	bookC.readyAttack = false;
	bookC.whatAttack = VERTICAL_LASER;

	//Field flag
	bool skillBuff = false;
	bool dead_delay = false;
	double hpBar = bookC.HP;
	SDL_Rect hpBarScreen = { 0,0,SCREEN_WIDTH,20 };
	SDL_Rect skillBarScreen = { 0, SCREEN_HEIGHT - 20,100,20 };
	SDL_Rect bombPos;

	//Game info. strings
	const char* gameover = "Game OVER!";
	const char* gameclear = "CLEAR!";
	const char* returnMenu = "Press any key to return to menu.";
	const char* warning = "Warning! Be careful if you are nearby red lines!";
	const char* bombwarning = "Keep away from red circle!";

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = 1;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_w: if (!player.status.going_up) { velY = 0; player.status.going_up = true; } velY += DOT_VEL; break;
				case SDLK_s: if (player.status.going_up) { velY = 0; player.status.going_up = false; } velY -= DOT_VEL;  break;
				case SDLK_a: if (player.status.going_right) { velX = 0; player.status.going_right = false; } velX -= DOT_VEL; break;
				case SDLK_d: if (!player.status.going_right) { velX = 0; player.status.going_right = true; }; velX += DOT_VEL; break;
				case SDLK_SPACE: if (!player.status.game_over) {
					shootingGun(bullets, player, mousePos, sizeof(bullets) / sizeof(Bullet), false);
				}	break;
				case SDLK_x:
					if (player.status.skill_gage == 100 && !skillBuff) {
						player.status.useSkill = true;
						player.status.skill_gage = 0;
						skillBuff = true;
						skill_bullet = (Bullet*)calloc(1, sizeof(Bullet));
						printf("\n\n\nSKILL ON\n\n\n");
						shootingGun(skill_bullet, player, mousePos, 1, true);
					}
					break;
					//Skill gage increasement & shooting test
					//case SDLK_e: player.status.skill_gage = 100; player.vel.x = 0; player.vel.y = 0; break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mousePos_x, &mousePos_y);
				//printf("mousePos: %d %d\n", mousePos_x, mousePos_y);
				mousePos.x = (double)mousePos_x;
				mousePos.y = (double)mousePos_y;
				//printf("mousePos_struct: %lf %lf\n", mousePos.x, mousePos.y);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (!player.status.game_over) {
					shootingGun(bullets, player, mousePos, sizeof(bullets) / sizeof(Bullet), false);
				}
			}
		}

		//******About User******
		//Player's motion
		posX += velX*TIMESTEP;
		posY -= velY*TIMESTEP;
		if (posX > SCREEN_WIDTH - charimg) {
			posX = SCREEN_WIDTH - charimg;
			velX = 0;
		}
		else if (posX < 0) {
			posX = 0;
			velX = 0;
		}
		if (posY > SCREEN_HEIGHT - charimg - 20) {
			posY = SCREEN_HEIGHT - charimg - 20;
			velY = 0;
		}
		else if (posY < 20) {
			posY = 20;
			velY = 0;
		}
		player.pos.x = posX;
		player.pos.y = posY;

		//Setting bullets' position
		for (int i = 0; i < sizeof(bullets) / sizeof(Bullet); i++) {
			if (bullets[i].isShot) {
				bullets[i].pos.x += bullets[i].vel.x;
				bullets[i].pos.y += bullets[i].vel.y;
			}

			if (bullets[i].pos.x > SCREEN_WIDTH || bullets[i].pos.x<0 || bullets[i].pos.y>SCREEN_HEIGHT || bullets[i].pos.y<0) {
				bulletDestroy(bullets, i, false);
			}
		}
		bullet1.pos.x += bullet1.vel.x;
		bullet2.pos.x += bullet2.vel.x;

		if (skillBuff) {
			if (skill_bullet->isShot) {
				skill_bullet->pos.x += skill_bullet->vel.x;
				skill_bullet->pos.y += skill_bullet->vel.y;
			}
			if (skill_bullet->pos.x > SCREEN_WIDTH || skill_bullet->pos.x<0 || skill_bullet->pos.y>SCREEN_HEIGHT || skill_bullet->pos.y<0) {
				skill_bullet->isShot = false;
				//bulletDestroy(skill_bullet, 0, true);
			}
		}


		//If user uses skill
		if (player.status.useSkill) {
			if (skillBuff && !skill_bullet->isShot) {
				skillBuff = false;
				bulletDestroy(skill_bullet, 0, true);
				free(skill_bullet);
				player.status.useSkill = false;
			}
		}


		//******About Monster & Map*********
		//If monster get hit by player's bullet
		for (int i = 0; i < BULLET_NUMBER_DEFAULT; i++) {
			if (hitMark(bookC, bullets[i])) {
				if (bookC.HP >= BULLET_DAMAGE) {
					bookC.HP -= BULLET_DAMAGE;
					bulletDestroy(bullets, i, false);
					if (player.status.skill_gage + CHARGE_RATE < 100)
						player.status.skill_gage += CHARGE_RATE;
					else
						player.status.skill_gage = 100;
				}
				else {
					bookC.HP = 0;
					bulletDestroy(bullets, i, false);
					if (player.status.skill_gage + CHARGE_RATE < 100)
						player.status.skill_gage += CHARGE_RATE;
					else
						player.status.skill_gage = 100;
				}
			}
		}
		if (skillBuff) {
			if (hitMark(bookC, *skill_bullet)) {
				if (bookC.HP >= SKILL_DAMAGE) {
					bookC.HP -= SKILL_DAMAGE;
				}
				else {
					bookC.HP = 0;
				}
				bulletDestroy(skill_bullet, 0, true);
				skillBuff = false;
			}
		}


		//Monster's attack
		if (!bookC.isAttack) {
			bookC.isAttack = true;
			attackDuration = SDL_GetTicks() + 2 * 1000;

		}
		else {
			if (SDL_TICKS_PASSED(SDL_GetTicks(), attackDuration) && !bookC.readyAttack) {
				//printf("***** Monster starts attack! *****\n");
				bookC.whatAttack = rtnRandomNumber(4, 0);
				switch (bookC.whatAttack) {
				case VERTICAL_LASER:
					duration[0] = SDL_GetTicks() + 300 * 1;
					duration[1] = SDL_GetTicks() + 300 * 2;
					duration[2] = SDL_GetTicks() + 300 * 2 + 2 * 1000;
					duration[3] = SDL_GetTicks() + 300 * 2 + 2 * 1000 + 300 * 1;
					duration[4] = SDL_GetTicks() + 300 * 2 + 2 * 1000 + 300 * 2;
					duration[5] = SDL_GetTicks() + 300 * 2 + 2 * 1000 + 300 * 2 + 1500;

					for (int i = 0; i < 3; i++) {
						lasers[i].x = 0;
						lasers[i].y = 120 * (i + 1) - laser_thickness;
						lasers[i].w = SCREEN_WIDTH;
						lasers[i].h = laser_thickness * 2;
					}

					bookC.readyAttack = true;
					break;

				case HORIZONTAL_LASER:
					duration[0] = SDL_GetTicks() + 300 * 1;
					duration[1] = SDL_GetTicks() + 300 * 2;
					duration[2] = SDL_GetTicks() + 300 * 3;
					duration[3] = SDL_GetTicks() + 300 * 3 + 2 * 1000;
					duration[4] = SDL_GetTicks() + 300 * 3 + 2 * 1000 + 300 * 1;
					duration[5] = SDL_GetTicks() + 300 * 3 + 2 * 1000 + 300 * 2;
					duration[6] = SDL_GetTicks() + 300 * 3 + 2 * 1000 + 300 * 3;
					duration[7] = SDL_GetTicks() + 300 * 3 + 2 * 1000 + 300 * 3 + 1500;

					for (int i = 0; i < sizeof(lasers) / sizeof(SDL_Rect); i++) {
						lasers[i].x = 128 * (i + 1) - laser_thickness;
						lasers[i].y = 0;
						lasers[i].w = laser_thickness * 2;
						lasers[i].h = SCREEN_HEIGHT;
					}

					bookC.readyAttack = true;
					break;

				case BOMBING:
					duration[0] = SDL_GetTicks() + 2 * 1000;
					duration[1] = SDL_GetTicks() + 2 * 1000 + 3 * 1000;
					bombPos.x = player.pos.x - bomb_radius;
					bombPos.y = player.pos.y - bomb_radius;
					bombPos.w = bomb_radius * 2;
					bombPos.h = bomb_radius * 2;

					bookC.readyAttack = true;
					break;

				case CROSS_LASER:
					duration[0] = SDL_GetTicks() + 2 * 1000;
					duration[1] = SDL_GetTicks() + 2 * 1000 + 3 * 1000;

					lasers[0].x = 0;
					lasers[0].y = player.pos.y - laser_thickness * 2;
					lasers[0].w = SCREEN_WIDTH;
					lasers[0].h = laser_thickness * 4;
					lasers[1].x = player.pos.x - laser_thickness * 2;
					lasers[1].y = 0;
					lasers[1].w = laser_thickness * 4;
					lasers[1].h = SCREEN_HEIGHT;

					bookC.readyAttack = true;
					break;

				case COUNTER_VERTICAL_LASER:
					duration[0] = SDL_GetTicks() + 700;
					duration[1] = SDL_GetTicks() + 1000;
					duration[2] = SDL_GetTicks() + 700 * 2;
					duration[3] = SDL_GetTicks() + 700 + 1000;
					duration[4] = SDL_GetTicks() + 700 * 2 + 1000;
					duration[5] = SDL_GetTicks() + 1000 + 2 * 1000;
					duration[6] = SDL_GetTicks() + 700 + 1000 + 2 * 1000;
					duration[7] = SDL_GetTicks() + 700 * 2 + 1000 + 2 * 1000;

					lasers[0].x = 0;
					lasers[0].y = player.pos.y - laser_thickness;
					lasers[0].w = SCREEN_WIDTH;
					lasers[0].h = laser_thickness * 2;

					bookC.readyAttack = true;
					break;

				case COUNTER_HORIZONTAL_LASER:
					duration[0] = SDL_GetTicks() + 700;
					duration[1] = SDL_GetTicks() + 1000;
					duration[2] = SDL_GetTicks() + 700 * 2;
					duration[3] = SDL_GetTicks() + 700 + 1000;
					duration[4] = SDL_GetTicks() + 700 * 2 + 1000;
					duration[5] = SDL_GetTicks() + 1000 + 2 * 1000;
					duration[6] = SDL_GetTicks() + 700 + 1000 + 2 * 1000;
					duration[7] = SDL_GetTicks() + 700 * 2 + 1000 + 2 * 1000;

					lasers[0].x = player.pos.x - laser_thickness;
					lasers[0].y = 0;
					lasers[0].w = laser_thickness * 2;
					lasers[0].h = SCREEN_HEIGHT;

					bookC.readyAttack = true;
					break;

				default: bookC.readyAttack = true; break;
				}

			}







			//******About Game Management**********
			//If game over
			if (isPlayerHitByBullet(player, bullet1)) {
				player.status.game_over = true;
			}
			if (isPlayerHitByBullet(player, bullet2)) {
				player.status.game_over = true;
			}
			//Player was hit by other bullet
			if (charCollision(player, bookC.pos)) {
				player.status.game_over = true;
			}//Player bumped with monster








			 //********About Rendering Screen**********
			 //Clear screen
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);


			//Render texture to screen
			SDL_RenderCopy(renderer, texture, NULL, NULL);




			//Render bullets
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			for (int i = 0; i < BULLET_NUMBER_DEFAULT; i++) {
				if (!(bullets + i)->isShot)
					continue;
				SDL_RenderFillRect(renderer, &(bullets + i)->pos);
			}
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(renderer, &bullet1.pos);
			SDL_RenderFillRect(renderer, &bullet2.pos);
			if (skillBuff)
				SDL_RenderFillRect(renderer, &skill_bullet->pos);

			SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
			if (bookC.isAttack&&bookC.readyAttack) {
				switch (bookC.whatAttack) {
				case VERTICAL_LASER:
					SDL_RenderDrawLine(renderer, 0, 120 * 1, 640, 120 * 1);
					//printf("%s\n", warning);

					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[0] && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[2]))) {
						SDL_RenderDrawLine(renderer, 0, 120 * 2, 640, 120 * 2);
						//printf("%s\n", warning);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[1] && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[2]))) {
						SDL_RenderDrawLine(renderer, 0, 120 * 3, 640, 120 * 3);
						//printf("%s\n", warning);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[2])) {
						SDL_RenderFillRect(renderer, &lasers[0]);
						if (charCollision(player, lasers[0])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[3])) {
						SDL_RenderFillRect(renderer, &lasers[1]);
						if (charCollision(player, lasers[1])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[4])) {
						SDL_RenderFillRect(renderer, &lasers[2]);
						if (charCollision(player, lasers[2])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[5])) {
						for (int i = 0; i < sizeof(duration) / sizeof(Uint32); i++)
							duration[i] = 0;
						for (int i = 0; i < sizeof(lasers) / sizeof(SDL_Rect); i++) {
							lasers[i].x = 0;
							lasers[i].y = 0;
							lasers[i].w = 0;
							lasers[i].h = 0;
						}
						bookC.isAttack = false;
						bookC.readyAttack = false;
					}
					break;

				case HORIZONTAL_LASER:
					SDL_RenderDrawLine(renderer, 128 * 1, 0, 128 * 1, SCREEN_HEIGHT);
					//printf("%s\n", warning);

					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[0] && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[3]))) {
						SDL_RenderDrawLine(renderer, 128 * 2, 0, 128 * 2, SCREEN_HEIGHT);
						//printf("%s\n", warning);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[1] && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[3]))) {
						SDL_RenderDrawLine(renderer, 128 * 3, 0, 128 * 3, SCREEN_HEIGHT);
						//printf("%s\n", warning);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[2] && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[3]))) {
						SDL_RenderDrawLine(renderer, 128 * 4, 0, 128 * 4, SCREEN_HEIGHT);
						//printf("%s\n", warning);
					}

					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[3])) {
						SDL_RenderFillRect(renderer, &lasers[0]);
						//printf("LASER BEAM\n");
						if (charCollision(player, lasers[0])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[4])) {
						SDL_RenderFillRect(renderer, &lasers[1]);
						if (charCollision(player, lasers[1])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[5])) {
						SDL_RenderFillRect(renderer, &lasers[2]);
						if (charCollision(player, lasers[2])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[6])) {
						SDL_RenderFillRect(renderer, &lasers[3]);
						if (charCollision(player, lasers[3])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[7])) {
						for (int i = 0; i < sizeof(duration) / sizeof(Uint32); i++)
							duration[i] = 0;
						for (int i = 0; i < sizeof(lasers) / sizeof(SDL_Rect); i++) {
							lasers[i].x = 0;
							lasers[i].y = 0;
							lasers[i].w = 0;
							lasers[i].h = 0;
						}
						bookC.isAttack = false;
						bookC.readyAttack = false;
					}
					break;

				case BOMBING:
					if (!SDL_TICKS_PASSED(!SDL_GetTicks(), duration[0])) {
						SDL_SetTextureAlphaMod(bombCircle, 100);
						SDL_RenderCopy(renderer, bombCircle, NULL, &bombPos);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[0])) {
						SDL_SetTextureAlphaMod(bombCircle, 255);
						SDL_RenderCopy(renderer, bombCircle, NULL, &bombPos);
						SDL_Rect bp = { bombPos.x + bomb_radius,bombPos.y + bomb_radius,0,0 };
						SDL_Rect pl = { player.pos.x + charimg / 2,player.pos.y + charimg / 2,0,0 };
						double inCircle = calcDistance(pl, bp);
						//printf("%lf\n", inCircle);
						if (inCircle < bomb_radius + charimg / 2) {
							//printf("Hit by bomb!\n");
							player.status.game_over = true;
						}

					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[1])) {
						for (int i = 0; i < sizeof(duration) / sizeof(Uint32); i++)
							duration[i] = 0;
						bookC.isAttack = false;
						bookC.readyAttack = false;
					}
					break;

				case CROSS_LASER:
					SDL_RenderDrawLine(renderer, lasers[0].x, lasers[0].y + laser_thickness * 2, lasers[0].w, lasers[0].y + laser_thickness * 2); SDL_RenderDrawLine(renderer, lasers[0].x, lasers[0].y + laser_thickness * 2, lasers[0].w, lasers[0].y + laser_thickness * 2);
					SDL_RenderDrawLine(renderer, lasers[1].x + laser_thickness * 2, lasers[1].y, lasers[1].x + laser_thickness * 2, lasers[1].h);
					//printf("%s\n", warning);
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[0]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[1])) {
						for (int i = 0; i < 2; i++) {
							SDL_RenderFillRect(renderer, &lasers[i]);
							if (charCollision(player, lasers[i])) {
								player.status.game_over = true;
							}
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[1])) {
						for (int i = 0; i < sizeof(duration) / sizeof(Uint32); i++)
							duration[i] = 0;
						bookC.isAttack = false;
						bookC.readyAttack = false;
					}
					break;

				case COUNTER_VERTICAL_LASER:
					if (!(SDL_TICKS_PASSED(SDL_GetTicks(), duration[1])))
						SDL_RenderDrawLine(renderer, lasers[0].x, lasers[0].y + laser_thickness, lasers[0].w, lasers[0].y + laser_thickness);

					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[0]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[3])) {
						if (lasers[1].x == 0 && lasers[1].y == 0) {
							lasers[1].x = 0;
							lasers[1].y = player.pos.y - laser_thickness;
							lasers[1].w = SCREEN_WIDTH;
							lasers[1].h = laser_thickness * 2;
						}
						SDL_RenderDrawLine(renderer, lasers[1].x, lasers[1].y + laser_thickness, lasers[1].w, lasers[1].y + laser_thickness);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[1]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[5])) {
						SDL_RenderFillRect(renderer, &lasers[0]);
						if (charCollision(player, lasers[0])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[2]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[4])) {
						if (lasers[2].x == 0 && lasers[2].y == 0) {
							lasers[2].x = 0;
							lasers[2].y = player.pos.y - laser_thickness;
							lasers[2].w = SCREEN_WIDTH;
							lasers[2].h = laser_thickness * 2;
						}
						SDL_RenderDrawLine(renderer, lasers[2].x, lasers[2].y + laser_thickness, lasers[2].w, lasers[2].y + laser_thickness);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[3]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[6])) {
						SDL_RenderFillRect(renderer, &lasers[1]);
						if (charCollision(player, lasers[1])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[4]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[7])) {
						SDL_RenderFillRect(renderer, &lasers[2]);
						if (charCollision(player, lasers[2])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[7])) {
						for (int i = 0; i < sizeof(duration) / sizeof(Uint32); i++)
							duration[i] = 0;
						for (int i = 0; i < sizeof(lasers) / sizeof(SDL_Rect); i++) {
							lasers[i].x = 0;
							lasers[i].y = 0;
							lasers[i].w = 0;
							lasers[i].h = 0;
						}
						bookC.isAttack = false;
						bookC.readyAttack = false;
					}

					break;

				case COUNTER_HORIZONTAL_LASER:
					if (!(SDL_TICKS_PASSED(SDL_GetTicks(), duration[1])))
						SDL_RenderDrawLine(renderer, lasers[0].x + laser_thickness, lasers[0].y, lasers[0].x + laser_thickness, lasers[0].h);

					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[0]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[3])) {
						if (lasers[1].x == 0 && lasers[1].y == 0) {
							lasers[1].x = player.pos.x - laser_thickness;
							lasers[1].y = 0;
							lasers[1].w = laser_thickness * 2;
							lasers[1].h = SCREEN_HEIGHT;
						}
						SDL_RenderDrawLine(renderer, lasers[1].x + laser_thickness, lasers[1].y, lasers[1].x + laser_thickness, lasers[1].h);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[1]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[5])) {
						SDL_RenderFillRect(renderer, &lasers[0]);
						if (charCollision(player, lasers[0])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[2]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[4])) {
						if (lasers[2].x == 0 && lasers[2].y == 0) {
							lasers[2].x = player.pos.x - laser_thickness;
							lasers[2].y = 0;
							lasers[2].w = laser_thickness * 2;
							lasers[2].h = SCREEN_HEIGHT;
						}
						SDL_RenderDrawLine(renderer, lasers[2].x + laser_thickness, lasers[2].y, lasers[2].x + laser_thickness, lasers[2].h);
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[3]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[6])) {
						SDL_RenderFillRect(renderer, &lasers[1]);
						if (charCollision(player, lasers[1])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[4]) && !SDL_TICKS_PASSED(SDL_GetTicks(), duration[7])) {
						SDL_RenderFillRect(renderer, &lasers[2]);
						if (charCollision(player, lasers[2])) {
							player.status.game_over = true;
						}
					}
					if (SDL_TICKS_PASSED(SDL_GetTicks(), duration[7])) {
						for (int i = 0; i < sizeof(duration) / sizeof(Uint32); i++)
							duration[i] = 0;
						for (int i = 0; i < sizeof(lasers) / sizeof(SDL_Rect); i++) {
							lasers[i].x = 0;
							lasers[i].y = 0;
							lasers[i].w = 0;
							lasers[i].h = 0;
						}
						bookC.isAttack = false;
						bookC.readyAttack = false;
					}

					break;

				default: bookC.isAttack = false; bookC.readyAttack = false; break;
				}
			}
		}

		//Rendering character & monster
		if (!player.status.game_over)
			SDL_RenderCopy(renderer, player_character, NULL, &player.pos);
		SDL_RenderCopy(renderer, monster_C, NULL, &(bookC.pos));

		//Display monster's HP bar
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_Rect hpBarDefault = { 0,0,SCREEN_WIDTH,20 };
		SDL_RenderFillRect(renderer, &hpBarDefault);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		double hpBar2 = (double)bookC.HP / hpBar;
		hpBarScreen.w = SCREEN_WIDTH * hpBar2;
		SDL_RenderFillRect(renderer, &hpBarScreen);

		//Render info tab
		SDL_Rect infoBar = { 0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20 };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &infoBar);
		if (bookC.isAttack&&bookC.readyAttack) {
			switch (bookC.whatAttack) {
			case VERTICAL_LASER:
				drawText(renderer, warning, 15, 46, 105, 255, 255, 255); break;
			case HORIZONTAL_LASER:
				drawText(renderer, warning, 15, 46, 105, 255, 255, 255); break;
			case BOMBING:
				drawText(renderer, bombwarning, 15, 46, 105, 255, 255, 255); break;
			case CROSS_LASER:
				drawText(renderer, warning, 15, 46, 105, 255, 255, 255); break;
			case COUNTER_VERTICAL_LASER:
				drawText(renderer, warning, 15, 46, 105, 255, 255, 255); break;
			case COUNTER_HORIZONTAL_LASER:
				drawText(renderer, warning, 15, 46, 105, 255, 255, 255); break;

			default:
				bookC.isAttack = false;
				bookC.readyAttack = false;
				break;
			}
		}


		//Display skill gage bar
		SDL_Rect skillBarDefadult = { 0,SCREEN_HEIGHT - 20,100,20 };
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderFillRect(renderer, &skillBarDefadult);
		if (player.status.skill_gage < 100) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			double skillBar = (double)player.status.skill_gage / 100;
			skillBarScreen.w = 100 * skillBar;
			SDL_RenderFillRect(renderer, &skillBarScreen);
		}
		else if (player.status.skill_gage == 100) {
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			skillBarScreen.w = 100;
			SDL_RenderFillRect(renderer, &skillBarScreen);
		}


		//Render text
		char info[256];
		//len = sprintf(info, "%d(%d %d) %d(%d %d) %d(%d %d) %d(%d %d) %d(%d %d)", 0, bullets[0].pos.x, bullets[0].pos.y, 1, bullets[1].pos.x, bullets[1].pos.y, 2, bullets[2].pos.x, bullets[2].pos.y, 3, bullets[3].pos.x, bullets[3].pos.y, 4, bullets[4].pos.x, bullets[4].pos.y, 5, bullets[5].pos.x, bullets[5].pos.y);
		sprintf(info, "BOSS HP: %d", bookC.HP);
		//printf("%s\n", info);
		drawText(renderer, info, 10, 0, 5, 0, 0, 0);
		sprintf(info, "%d%%", player.status.skill_gage);
		drawText(renderer, info, 10, 46, 5, 150, 150, 150);


		//Show GAME OVER when character has been killed
		if (player.status.game_over) {
			drawText(renderer, gameover, 100, 0, 5, 255, 0, 0);
			drawText(renderer, returnMenu, 30, 10, 5, 0, 0, 0);
			if (!dead_delay) {
				dead_dur = SDL_GetTicks() + 1000;
				dead_delay = true;
			}
			if (SDL_TICKS_PASSED(SDL_GetTicks(), dead_dur)) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_KEYDOWN) {
						SDL_DestroyTexture(texture);
						SDL_DestroyTexture(player_character);
						SDL_DestroyTexture(monster_C);
						SDL_DestroyTexture(bombCircle);
						return 0;
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN) {
						SDL_DestroyTexture(texture);
						SDL_DestroyTexture(player_character);
						SDL_DestroyTexture(monster_C);
						SDL_DestroyTexture(bombCircle);
						return 0;
					}
				}
			}
		}
		//If game clear
		else if (bookC.HP == 0) {
			drawText(renderer, gameclear, 100, 1, 5, 0, 0, 0);
			drawText(renderer, returnMenu, 30, 10, 5, 0, 0, 0);

			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_KEYDOWN) {
					SDL_DestroyTexture(texture);
					SDL_DestroyTexture(player_character);
					SDL_DestroyTexture(monster_C);
					SDL_DestroyTexture(bombCircle);
					return 1;
				}

				else if (e.type == SDL_MOUSEBUTTONDOWN) {
					SDL_DestroyTexture(texture);
					SDL_DestroyTexture(player_character);
					SDL_DestroyTexture(monster_C);
					SDL_DestroyTexture(bombCircle);
					return 1;
				}

			}
		}

		//Update screen
		SDL_RenderPresent(renderer);


		//*******etc*******
		//ex bullet
		if (bullet1.pos.x < 0) {
			bullet1.pos.x = SCREEN_WIDTH;
			bullet1.pos.y += 20;
			if (bullet1.pos.y > 420)
				bullet1.pos.y = 60;
		}
		if (bullet2.pos.x < 0) {
			bullet2.pos.x = SCREEN_WIDTH;
			bullet2.pos.y -= 20;
			if (bullet2.pos.y < 60)
				bullet2.pos.y = 420;
		}
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(player_character);
	SDL_DestroyTexture(monster_C);
	SDL_DestroyTexture(bombCircle);

	return 0;
}
