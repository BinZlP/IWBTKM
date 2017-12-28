#include "gameFunc.h"
#include "gameMove.h"

void endingCredit(SDL_Renderer* renderer) {
	SDL_Texture* end1 = loadTexture("story/end1.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	SDL_Texture* end2 = loadTexture("story/end2.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	SDL_Texture* end3 = loadTexture("story/end3.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	SDL_Texture* end4 = loadTexture("story/end4.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	SDL_Texture* end5 = loadTexture("story/end5.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	SDL_Texture* end6 = loadTexture("story/end6.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	SDL_Texture* end7 = loadTexture("story/end7.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	SDL_Texture* end8 = loadTexture("story/end8.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	SDL_Texture* end9 = loadTexture("story/end9.png", renderer);
	if (end1 == NULL)
	{
		printf("Failed to load texture image!\n");
		return;
	}

	int opening_end = 1;

	int quit = 0;   //Main loop flag
	SDL_Event e;    //Event

					//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = 1;
			}
			else if (e.type == SDL_KEYDOWN) {
				opening_end++;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				opening_end++;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		//Render texture
		switch (opening_end) {
		case 1: SDL_RenderCopy(renderer, end1, NULL, NULL); break;
		case 2: SDL_RenderCopy(renderer, end2, NULL, NULL); break;
		case 3: SDL_RenderCopy(renderer, end3, NULL, NULL); break;
		case 4: SDL_RenderCopy(renderer, end4, NULL, NULL); break;
		case 5: SDL_RenderCopy(renderer, end5, NULL, NULL); break;
		case 6: SDL_RenderCopy(renderer, end6, NULL, NULL); break;
		case 7: SDL_RenderCopy(renderer, end7, NULL, NULL); break;
		case 8: SDL_RenderCopy(renderer, end8, NULL, NULL); break;
		case 9: SDL_RenderCopy(renderer, end9, NULL, NULL); break;
		case 10:
			SDL_DestroyTexture(end1);
			SDL_DestroyTexture(end2);
			SDL_DestroyTexture(end3);
			SDL_DestroyTexture(end4);
			SDL_DestroyTexture(end5);
			SDL_DestroyTexture(end6);
			SDL_DestroyTexture(end7);
			SDL_DestroyTexture(end8);
			SDL_DestroyTexture(end9);
			quit = 1; break;
		}

		//Update screen
		SDL_RenderPresent(renderer);
	}
	return;
}