#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
			bool showMenu = true;
			bool showDeath = false;

			while (!quit){
				while (showMenu){		// main menu
					SDL_RenderClear(gRenderer);
					SDL_RenderCopy(gRenderer, Background, NULL, &backgroundRect);
					int button = handleMenuDisp();
					while (SDL_PollEvent(&e) != 0){
						switch ( handleMenuClick(e, button) ){		//1: play, 2: instructions, 3: quit.
							case 1: showMenu = false; break;
							case 2: showMenu = false; quit = true; break;	// make instructions screen
							case 3: showMenu = false; quit = true; break;
						}
					}
					SDL_RenderPresent(gRenderer);	// update screen
				}

				while (showDeath){		// death menu
					SDL_RenderClear(gRenderer);
					SDL_RenderCopy(gRenderer, Background, NULL, &backgroundRect);
					int button = handleDeathDisp();
					while (SDL_PollEvent( &e ) != 0){
						switch (handleMenuClick(e, button)){		//1: play, 2: main menu, 3: quit
							case 1: showDeath = false; break;
							case 2: showDeath = false; showMenu = true; break;
							case 3: showDeath = false; quit = true; break;
						}
					}
					SDL_RenderPresent(gRenderer);
				}

				if (quit){	// save some time when quitting
					break;
				}
				if (showMenu){
					continue;
				}

				while (SDL_PollEvent( &e ) != 0){		// gameplay screen
					if(e.type == SDL_QUIT){
						quit = true;
					}
					gShip.handleEvent(e);
				}

				if (N_ASTEROIDS < MAX_N_ASTEROIDS){
					Asteroids.push_back(Asteroid());
					N_ASTEROIDS++;
				}

				// moves/deletes bullets
				for (std::vector<Bullet>::iterator it2 = Fired.begin(); it2 != Fired.end(); ){	
					if ( it2->move() ){
						it2 = Fired.erase(it2);
					}
					else{
						++it2;
					}
				}
				gShip.move();

				// all collisions! moves and tests if asteroids are shot, then if ship has collided
				for (std::vector<Asteroid>::iterator it1 = Asteroids.begin(); it1 != Asteroids.end(); ){
					it1->move();
					bool shot = false;

					for (std::vector<Bullet>::iterator it0 = Fired.begin(); it0 != Fired.end(); ){
						if (collided(*it0, *it1)){		// dereference iterators before sticking into function
							it0 = Fired.erase(it0);
							it1 = Asteroids.erase(it1);
							shot = true;
							N_ASTEROIDS--;
							score++;
							break;
						}
						else{
							++it0;
						}
					}
					
					if (Scollided(*it1)){
						showDeath = true;
						loadScore(score);
						resetGame();
						break;
					}
	
					if (!shot){
						++it1;		// if shot, iterator already advanced.
					}
				}


				// all drawing- background, rocks, bullets, ship.
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
