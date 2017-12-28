//Using SDL and standard IO
#include "gameFunc.h"
#include "gameMove.h"

int mainScreen(SDL_Renderer* renderer)
{
	SDL_Texture* mainBackground = loadTexture("main.png", renderer);
	if (mainBackground == NULL)
	{
		printf("Failed to load texture image!\n");
		return 1;
	}

	SDL_Texture* info = loadTexture("infoShow.png", renderer);
	if (info == NULL)
	{
		printf("Failed to load texture image!\n");
		return 1;
	}

	bool quit2 = false;
	SDL_Event e;    //Event

	//Mouse position
	Vector mousePos = { 0,0 };
	int mouse_xpos = 0;
	int mouse_ypos = 0;

	//Rendering information tab flag
	bool infoShow = false;

	SDL_Rect decoBullets[3];
	for (int i = 0; i < 3; i++){
		decoBullets[i].x = 0;
		decoBullets[i].y = 10;
		decoBullets[i].h = bulletSize;
		decoBullets[i].w = bulletSize;
	}

	SDL_Rect startButton = { 240,200,160,70 };
	SDL_Rect infoButton = { 240,290,160,70 };
	SDL_Rect quitButton = { 240,380,160,70 };

	//Game start screen
	while (!quit2)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit2 = true;
			}
			else if (e.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mouse_xpos, &mouse_ypos);
				mousePos.x = (double)mouse_xpos;
				mousePos.y = (double)mouse_ypos;
				//printf("%lf %lf\n", mousePos.x, mousePos.y);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (infoShow) {
					infoShow = false;
				}
				else if (mouse_xpos>=240&&mouse_xpos<=400) {
					if (mouse_ypos >= startButton.y&&mouse_ypos <= (startButton.y + startButton.h)) {
						int game = enterGame(renderer);
						if(game == 1)
							quit2 = true;
					}
					else if (mouse_ypos >= infoButton.y&&mouse_ypos <= (infoButton.y + infoButton.h)) {
						infoShow = true;
					}
					else if (mouse_ypos >= quitButton.y&&mouse_ypos <= (quitButton.y + quitButton.h)) {
						quit2 = true;
					}
				}
			}
		}


		//Clear screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		//Render texture
		SDL_RenderCopy(renderer, mainBackground, NULL, NULL);

		SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
		SDL_RenderFillRect(renderer, &startButton);
		SDL_RenderFillRect(renderer, &infoButton);
		SDL_RenderFillRect(renderer, &quitButton);

		SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
		if (mousePos.x >= startButton.x&&mousePos.x <= startButton.x + startButton.w) {
			if (mousePos.y >= startButton.y&&mousePos.y <= startButton.y + startButton.h)
				SDL_RenderFillRect(renderer, &startButton);
			else if (mousePos.y >= infoButton.y&&mousePos.y <= infoButton.y + infoButton.h)
				SDL_RenderFillRect(renderer, &infoButton);
			else if (mousePos.y >= quitButton.y&&mousePos.y <= quitButton.y + quitButton.h)
				SDL_RenderFillRect(renderer, &quitButton);
		}


		drawText(renderer, "START", 40, 21, 255, 0, 0, 0);
		drawText(renderer, "HOW TO PLAY", 20, 31, 250, 0, 0, 0);
		drawText(renderer, "QUIT", 40, 39, 270, 0, 0, 0);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRects(renderer, decoBullets, 3);

		if (infoShow) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, info, NULL, NULL);
		}

		//Update screen
		SDL_RenderPresent(renderer);

		for (int i = 0; i < 3; i++) {
			decoBullets[i].x += 10 * (0.5*(i + 1));
			if (decoBullets[i].x > SCREEN_WIDTH)
				decoBullets[i].x = 0;
		}
	}

	SDL_DestroyRenderer(mainBackground);
	SDL_DestroyRenderer(info);
	return 1;

}