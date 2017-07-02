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
			SDL_Rect backgroundRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

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
					it1->move();
					bool shot = false;

					for (std::vector<Bullet>::iterator it0 = Fired.begin(); it0 != Fired.end(); ){
						if (collided(*it0, *it1)){		// dereference iterators before sticking into function
							it0 = Fired.erase(it0);
							it1 = Asteroids.erase(it1);
							shot = true;
							N_ASTEROIDS--;
							break;
						}
						else{
							++it0;
						}
					}
					
					if (Scollided(*it1)){
						SDL_Delay(1000);
						quit = true;
						break;
					}
	
					if (!shot){
						++it1;		// if shot, iterator already advanced.
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

				SDL_RenderCopy(gRenderer, Background, NULL, &backgroundRect); 

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
