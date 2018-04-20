#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Animation.h"
#include "Entity.h"
#include "Hero.h"
#include <list>

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* texture;
SDL_Rect sourceRectangle;
SDL_Rect destinationRectangle;

int main(int arc, char **argv)
{
	//initiate SDL with the subsystems you want to use ie SDL_INIT_VIDEO
	//we're initaliasing all of them (sound, input, video etc)
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL Fail initialized!!!\n";
		return -1; //.faild, dont contine rest of main code
	}
	else
	{
		cout << "SDL initialized success!!!\n";
	}

	//Need to init SDL_image
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		cout << "sdl image did not load: " << IMG_GetError() << endl;
		SDL_Quit();
		std::system("pause");
		return -1;
	}

	//create window, params are: window title, window pos x, pos y, width, height, window flages
	window = SDL_CreateWindow("666", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 640, 480,
		SDL_WINDOW_SHOWN /* | SDL_WINDOW_FULLSCREEN */);

	if (window != NULL)
	{
		cout << "Window created!!" << endl;
	}
	else
	{
		cout << "Failed to create window!" << endl;
		return -1;
	}

	//create renderer to help draw stuff to the screen
	renderer = 
		SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer != NULL)
	{
		cout << "Renderer created!!" << endl;
	}
	else
	{
		cout << "Renderer Failed" << endl;
		return -1;
	}

	//Load up whatever assets here after init

	//to load up an image, first load it up as a surface
	SDL_Surface* knightSurface = SDL_LoadBMP("assets/knight.bmp");
	//then cenvert to texture
	SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, knightSurface);
	//dont need surface anymore, free memory up
	SDL_FreeSurface(knightSurface);

	//let use pass by reference to work the width and height of our texture in pixels
	SDL_QueryTexture(knightTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h);
	cout << "texture size is : " << sourceRectangle.w << "x" << sourceRectangle.h << endl;

	sourceRectangle.x = 200;
	sourceRectangle.y = 0;
	sourceRectangle.w = 100;
	sourceRectangle.h = 100;
	destinationRectangle.x = sourceRectangle.x = 100;
	destinationRectangle.y = sourceRectangle.y = 50;
	destinationRectangle.w = sourceRectangle.w*2;//copy the width of our texture
	destinationRectangle.h = sourceRectangle.h*2;//copy height

	//run texture
	SDL_Texture* runSpriteSheet = IMG_LoadTexture(renderer, "assets/run.png");
	//this time lets make a source rectangle copying just 1 frame in size at a time
	SDL_Rect runClipRect = {0,0,32,32};//x:0, y:0, w:32, h:32
	SDL_Rect runDestRect = {640-128,480-128,128,128};//x y, get image to buttom right of screen. w/h match clip/frame size

	SDL_Surface* runSurface = IMG_Load("assets/run.png");

	//params: surface , 1/0/to set or unset color key, rgb value to make color key . mapRPG just does its best to find
	//closest match to a surfaces pixel format e.g. 8bit,16bit,32bit and what colors are in that set
	SDL_SetColorKey(runSurface,1,SDL_MapRGB(runSurface->format,128,128,255));

	//now convert to texture
	SDL_Texture* runSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, runSurface);
	//free the surface :0
	SDL_FreeSurface(runSurface);

	//Animation Objects
	Animation anim1(runSpriteSheet, renderer, 4, 32, 32, 0.1);//0.2 = 200 millseconds per frame of animation
	Animation anim2(runSpriteSheetWithNoBG, renderer, 4, 32, 32, 0.8);
	Animation anim3(runSpriteSheet, renderer, 4, 32, 32, 0.05);//50ms per frame

	//setup time stuff
	Uint32 lastUpdate = SDL_GetTicks();//set last update to current time (muilliseconds to reach this bit of code)

	//list of game entities
	list<Entity*> entities;
	Hero* hero = new Hero();
	hero->setAnimation(&anim3);
	hero->setRenderer(renderer);
	//hero->setXY(30, 200);

	//
	Vector heroStartPos(200, 200);
	hero->setPosition(heroStartPos);

	//add our hero to the list
	entities.push_back(hero);

	bool loop = true;

	while (loop)
	{
		//different is current time running the last update time
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert into deltaTime (decimal)
		float DT = timeDiff / 1000.0;//e.g 200ms now = 0.2 for DT
		//now update lastupdate to the current time so the next loop calculates corrently
		lastUpdate = SDL_GetTicks();

		//cout << "dt = " << DT << endl;

		//set drawing colour for renderer, numbers are: RBG(alpha, transparency). All values between 0-255
		SDL_SetRenderDrawColor(renderer, 255, 0, 168, 255);
		//clear screen with current draw colour
		SDL_RenderClear(renderer);
		//TODO draw stuff to renderer here, like images, colours, ui whatever

		//set drawing colour to Baby Blue
		SDL_SetRenderDrawColor(renderer, 0, 228, 255, 255);
		//make a SDL_Rect, to describe a rectangle x,y,w,h
		SDL_Rect rect = { 100, 50, 300, 240 };

		//render rectangle to screen (renderer and arectangle for position)
		SDL_RenderFillRect(renderer, &rect);

		//Draw our 2D images here
		SDL_RenderCopy(renderer, knightTexture, &sourceRectangle, &destinationRectangle);

		//lets do animation stuff here
		int const runNumOfFrames = 4;
		runClipRect.x = runClipRect.w*int((SDL_GetTicks() / 100) % runNumOfFrames);
		//draw current frame to screen
		//SDL_RenderCopy(renderer, runSpriteSheet, &runClipRect, &runDestRect);

		/*//update animations
		anim1.update(DT);
		anim2.update(DT);
		anim3.update(DT);
		//draw from animations
		anim1.draw(50, 50);
		anim2.draw(50, 100, 4.0f);//4.0f is scale, stretch by 4x
		anim3.draw(400,400,true);//flip image horizontallly */

		//DEAL with user input
		//we check what kind of user input events have happened since our last check
		SDL_Event e;
		//loop through all events and temporarily stores event details in an SDL_Event object
		while (SDL_PollEvent(&e))
		{
			//check if user has clicked on the close window button
			if (e.type == SDL_QUIT)
			{
				//exit our loop
				loop = false;
			}
			//check if user has 'pressed' a button(not held)
			if (e.type == SDL_KEYDOWN)
			{
				//see if ESC key was pressed
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					//exit loop
					loop = false;
				}
				//if press up
				if (e.key.keysym.scancode == SDL_SCANCODE_UP)
				{
					Vector heroVelocity = hero->getVelocity();
					heroVelocity.y = -100;
					hero->setVelocity(heroVelocity);
				}
				//if press down
				if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
				{
					Vector heroVelocity = hero->getVelocity();
					heroVelocity.y = 100;
					hero->setVelocity(heroVelocity);
				}
			}
		}

		//loop through and update draw all entities
		for (list<Entity*>::iterator eIt = entities.begin(); eIt != entities.end(); eIt++)
		{
			(*eIt)->update(DT);
			(*eIt)->draw();
		}

		//Then   Get Renderer to output to the window
		SDL_RenderPresent(renderer);

		//sdl_ticks checks how many milliseconds since we started running our game
		/*if (SDL_GetTicks() > 15000)
		{
			loop = false;
		}*/

	}
	//SDL_Delay pauses game for x many milliseconds
	//SDL_Delay(5000);//5000ms = 5 secs

	//clean up any game objects
	delete hero;

	//cleanup renderer and window properly (aka clean up dynamic memory)
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	//shut down subsystems and cleanup any set up it did earlier in sdl_init
	SDL_Quit();

	system("pause");

	return 0;
}