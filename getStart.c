/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include "gameFunc.h"
#include "gameMove.h"

#include "basicTexture.h"

int main(int argc, char* args[])
{
	srand(time(NULL));

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 2;
	}
	if (SDL_Init(SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 2;
	}
	if (TTF_Init() < 0) {
		printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
		return 2;
	}


	//Create window
	SDL_Window* window = SDL_CreateWindow("I Wanna Be the Kwangwoon Man", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}



	//Create renderer for window
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story1 = loadTexture("story/1.png", renderer);
	if (story1 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story2 = loadTexture("story/2.png", renderer);
	if (story2 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	
	SDL_Texture* story3 = loadTexture("story/3.png", renderer);
	if (story3 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story4 = loadTexture("story/4.png", renderer);
	if (story4 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story5 = loadTexture("story/5.png", renderer);
	if (story5 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story6 = loadTexture("story/6.png", renderer);
	if (story6 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story7 = loadTexture("story/7.png", renderer);
	if (story7 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story8 = loadTexture("story/8.png", renderer);
	if (story8 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story9 = loadTexture("story/9.png", renderer);
	if (story9 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story10 = loadTexture("story/10.png", renderer);
	if (story10 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story11 = loadTexture("story/11.png", renderer);
	if (story11 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture* story12 = loadTexture("story/12.png", renderer);
	if (story12 == NULL)
	{
		printf("Failed to load texture image!\n");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	int opening_story = 1;

	int quit = 0;   //Main loop flag
	SDL_Event e;    //Event

					//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT){
				quit = 1;
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_SPACE) {
					opening_story = 12;
				}
				else {
					opening_story++;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				opening_story++;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		//Render texture
		switch (opening_story) {
		case 1: SDL_RenderCopy(renderer, story1, NULL, NULL); break;
		case 2: SDL_RenderCopy(renderer, story2, NULL, NULL); break;
		case 3: SDL_RenderCopy(renderer, story3, NULL, NULL); break;
		case 4: SDL_RenderCopy(renderer, story4, NULL, NULL); break;
		case 5: SDL_RenderCopy(renderer, story5, NULL, NULL); break;
		case 6: SDL_RenderCopy(renderer, story6, NULL, NULL); break;
		case 7: SDL_RenderCopy(renderer, story7, NULL, NULL); break;
		case 8: SDL_RenderCopy(renderer, story8, NULL, NULL); break;
		case 9: SDL_RenderCopy(renderer, story9, NULL, NULL); break;
		case 10: SDL_RenderCopy(renderer, story10, NULL, NULL); break;
		case 11: SDL_RenderCopy(renderer, story11, NULL, NULL); break;
		case 12: SDL_RenderCopy(renderer, story12, NULL, NULL); break;
		case 13: 
			SDL_DestroyTexture(story1);
			SDL_DestroyTexture(story2);
			SDL_DestroyTexture(story3);
			SDL_DestroyTexture(story4);
			SDL_DestroyTexture(story5);
			SDL_DestroyTexture(story6);
			SDL_DestroyTexture(story7);
			SDL_DestroyTexture(story8);
			SDL_DestroyTexture(story9);
			SDL_DestroyTexture(story10);
			SDL_DestroyTexture(story11);
			SDL_DestroyTexture(story12);
			quit = mainScreen(renderer); break;
		}

		//Update screen
		SDL_RenderPresent(renderer);
	}

	//Destroy renderer
	SDL_DestroyRenderer(renderer);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}