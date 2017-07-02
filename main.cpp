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
#include "Bullet.h"


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

			while (!quit){
				// queue
				while (SDL_PollEvent( &e ) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					gShip.handleEvent(e);
				}

				if (N_ASTEROIDS < MAX_N_ASTEROIDS){
					Asteroids.push_back(Asteroid());
					N_ASTEROIDS++;
				}

				for (std::vector<Asteroid>::iterator it1 = Asteroids.begin(); it1 != Asteroids.end(); ){	// moves and tests if asteroids are shot
					if (false){		// replace 'false' with collision detection
						it1 = Asteroids.erase(it1);
						N_ASTEROIDS--;
					}
					else{
						it1->move();
						++it1;
					}
				}

				for (std::vector<Bullet>::iterator it2 = Fired.begin(); it2 != Fired.end(); ){	// moves/deletes bullets
					if ( it2->move() ){
						it2 = Fired.erase(it2);
					}
					else{
						++it2;
					}
				}

				gShip.move();

				SDL_RenderClear(gRenderer);		// clear screen

				for (std::vector<Asteroid>::iterator rock = Asteroids.begin(); rock != Asteroids.end(); ++rock){
					rock->render();
				}
				for (std::vector<Bullet>::iterator bullet = Fired.begin(); bullet != Fired.end(); ++bullet){
					bullet->render();
				}

				gShip.render();
				SDL_RenderPresent(gRenderer);	// update screen
			}
		}
	}
	close();
	return 0;
}
