#include <SDL.h> 
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


const int screen_x = 640;                                         //screen x resolution 
const int screen_y = 480;                                         //screen y resolution 
const int screen_bpp = 32;                                          //screen color depth 

SDL_Surface *image = NULL;                                          //ship image 
SDL_Surface *screen = NULL;                                         //application window 
SDL_Surface *background = NULL;                                     //app background 
SDL_Surface *bullet = NULL;                                         //bullet image 

SDL_Event event;                                                    //to handle input 

SDL_Surface *load_image(char* file, int transp)                     //image loading function 
{
	SDL_Surface *loadedimage = NULL;                                //original image 
	SDL_Surface *optimizedimage = NULL;                             //upgraded image 

	loadedimage = IMG_Load(file);                                   //load the original to loadedimage 

	if (loadedimage != NULL)                                    //if loadedimage exists 
	{
		optimizedimage = SDL_DisplayFormat(loadedimage);        //optimize the image 
		SDL_FreeSurface(loadedimage);                           //get rid of old image 
	}
	if (transp == 1)                                            //if you want transparency 
	{
		if (optimizedimage != NULL)                             //if optomized image exists 
		{
			Uint32 colorkey = SDL_MapRGB(optimizedimage->format, 0, 0xFF, 0xFF);     //set color key 
			SDL_SetColorKey(optimizedimage, SDL_SRCCOLORKEY, colorkey);               //key the image 
		}
	}
	return optimizedimage;                                          //return new image 
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)             //function to apply surfaces 
{
	SDL_Rect offset;                                                //rectangle to handle x & y change 

	offset.x = x;                                                   //set x axis change 
	offset.y = y;                                                   //set y axis change 

	SDL_BlitSurface(source, NULL, destination, &offset);               //actually blit the surface 
}

bool SDL_init()                                                     //function to initialize SDL 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {                   //initialize everything, video, audio, etc 
		return false;
	}                                         //something went wrong 
	screen = SDL_SetVideoMode(screen_x, screen_y, screen_bpp, SDL_SWSURFACE | SDL_DOUBLEBUF);    //set up the screen 
	if (screen == NULL) {
		return false;
	}                                          //something went wrong 
	SDL_WM_SetCaption("The Game", NULL);                             //set the window caption 
	return true;                                                    //all went well 
}
bool load_files()                                                   //function to load all files 
{
	image = load_image("ship.png", 1);                               //load the ship with transparency 
	background = load_image("background2.png", 0);                    //load the background without trans 
	bullet = load_image("bullet3.png", 1);                           //load bullet without transparency 

	if (image == NULL || background == NULL || bullet == NULL)  //if any of the images err 
	{
		return false;                                           //something went wrong 
	}
	return true;                                                    //all is well 
}

void cleanup()                                                      //function to clean SDL up 
{
	SDL_FreeSurface(image);                                         //free the ship image 
	SDL_FreeSurface(bullet);                                        //free the bullet image 
	SDL_Quit();                                                     //exit SDL 
}


int main(int argc, char* args[])
{

	int x = screen_x / 2 - 30, y = screen_y - 60;                   //ship x and y pos 
	int xvel = 5, yvel = 5;   //ship speed


	bool activeBullets[20] = { false };                           //current active bullets 
	int bulx[20], buly[20];                                 //bullet x and y pos 
	int bulvel = 6;                                                 //bullet's velocity 


	bool dbul = false;
	bool quit = false;

	if (SDL_init() == false)                                     //initialize SDL 
	{
		return 1;
	}
	if (load_files() == false)                                   //load all of the files 
	{
		return 1;
	}

	bool keysHeld[323] = { false };                                   //initialize the key enemurator 

	while (quit == false)
	{

		while (SDL_PollEvent(&event))                                    //while there is an event to handle 
		{
			if (event.type == SDL_QUIT)                                 //if the user tells the program to quit 
			{
				quit = true;                                    //quit 
			}
			if (event.type == SDL_KEYDOWN)                              //if a key is pushed down 
			{
				if ((event.key.keysym.sym) == SDLK_F12)
				{
					if (dbul == true) { dbul = false; }
					else if (dbul == false) { dbul = true; }
				}
				if ((event.key.keysym.sym) == SDLK_LALT)          //if that key is space 
				{
					for (int temp1 = 0; temp1 < 20; temp1++)
					{
						if (activeBullets[temp1] == false)
						{
							activeBullets[temp1] = true;
							bulx[temp1] = x;
							buly[temp1] = y;
							temp1 = 20;
						}
					}

				}

				keysHeld[event.key.keysym.sym] = true;          //make any pushed keys true 
			}

			if (event.type == SDL_KEYUP)                                //if a key is up 
			{
				keysHeld[event.key.keysym.sym] = false;         //make any unpushed keys false 
			}
		}


		if (keysHeld[SDLK_ESCAPE])                                    //if user pushes escape 
		{
			quit = true;                                               //quit 
		}

		if (keysHeld[SDLK_LEFT])                                      //if user pushes left 
		{
			if (x - xvel > 0) {                                        //if image won't go off screen 
				x -= xvel;                                                //move image left 
			}
		}
		if (keysHeld[SDLK_RIGHT])                                     //if user pushes right 
		{
			if (x + xvel < screen_x - 60) {                            //as long as image wont leave the screen 
				x += xvel;                                                //move image right 
			}
		}
		if (keysHeld[SDLK_UP])                                        //if up is pushed 
		{
			if (y - yvel > 0) {                                        //and image is going to stay on screen 
				y -= yvel;                                                //move image up 
			}
		}
		if (keysHeld[SDLK_DOWN])                                        //if down is pushed 
		{
			if (y + yvel < screen_y - 60) {                            //if image is safe from leaving screen 
				y += yvel;                                                //move image down 
			}
		}



		apply_surface(0, 0, background, screen);                         //apply the background to the screen 
		apply_surface(x, y, image, screen);                              //apply ship to screen 

		if (dbul == true)
		{
			for (int temp2 = 0; temp2 < 5; temp2++)
			{

				if (activeBullets[temp2])                                    //if there are any bullets on the screen 
				{

					apply_surface(bulx[temp2], buly[temp2], bullet, screen);   //apply bullet to screen 
					apply_surface(bulx[temp2] + 60, buly[temp2], bullet, screen);
					buly[temp2] -= bulvel;                                  //move bullet up 

					if (buly[temp2] <= 0)                               //if bullet is offscreen 
					{
						activeBullets[temp2] = false;                   //that bullet is no longer on 
					}


				}
			}
		}
		if (dbul == false)
		{
			for (int temp2 = 0; temp2 < 5; temp2++)
			{

				if (activeBullets[temp2])                                    //if there are any bullets on the screen 
				{

					apply_surface(bulx[temp2] + 26, buly[temp2], bullet, screen);   //apply bullet to screen 
					buly[temp2] -= bulvel;                                  //move bullet up 

					if (buly[temp2] <= 0)                               //if bullet is offscreen 
					{
						activeBullets[temp2] = false;                   //that bullet is no longer on 
					}


				}
			}
		}

		SDL_Flip(screen);
		SDL_Delay(3);
	}

	cleanup();
	return 0;
}
