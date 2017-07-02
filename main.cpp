#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "Asteroid.h"
#include "Ship.h"
#include "RunGame.h"
#include "globals.h"

/*
// globals
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int MAX_N_ASTEROIDS = 5;
int N_ASTEROIDS = 0;
int A_Counter = 0;
Asteroid Asteroids[MAX_N_ASTEROIDS] = {};

Ship gShip;

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;
*/


int main(int argc, char* args[]){
	if (!init() ){
		printf("couldn't initialize\n");
	}
	else{
		if (!loadMedia() ){
			printf("couldn't load media \n");
		}
		else{
			// MAIN LOOP FLAG
			bool quit = false;
			SDL_Event e;
			bool is_shot[MAX_N_ASTEROIDS] = {false};	// each entry corresponds to an asteroid; when one is shot we use this to reinitialize

			while (!quit){
				// queue
				while (SDL_PollEvent( &e ) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					gShip.handleEvent(e);
				}
				
				for (int i = 0; i < MAX_N_ASTEROIDS; i++){
					if (is_shot[i]){
						Asteroids[i] = Asteroid();
						A_Counter++;
					}
					else{
						Asteroids[i].move();
					}
				}

				gShip.move();

				SDL_RenderClear(gRenderer);		// clear screen

				for (int j = 0; j < MAX_N_ASTEROIDS; j++){
					Asteroids[j].render();
				}
				gShip.render();
				SDL_RenderPresent(gRenderer);	// update screen
			}
		}
	}
	close();
	return 0;
}
