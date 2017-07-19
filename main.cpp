#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
			SDL_Rect infernoRect = {SCREEN_WIDTH/2-InfernoWidth/2, SCREEN_HEIGHT/2-InfernoHeight/2, InfernoWidth, InfernoHeight};
			bool showMenu = true;
			bool showDeath = false;
			bool showInferno = false;
			bool inferno = false;
			bool plusDifficulty = false;

			while (!quit){
				while (showMenu){		// main menu
					if (Mix_PlayingMusic() == 0){
						Mix_PlayMusic(menuMusic, -1);
					}
					SDL_RenderClear(gRenderer);
					SDL_RenderCopy(gRenderer, Background, NULL, &backgroundRect);
					int button = handleMenuDisp();
					while (SDL_PollEvent(&e) != 0){
						switch ( handleMenuClick(e, button) ){		//1: play, 2: instructions (inferno), 3: quit.
							case 1: showMenu = false; Mix_HaltMusic(); break;
							case 2: showMenu = false; Mix_HaltMusic(); showInferno = true; break;
							case 3: showMenu = false; Mix_HaltMusic(); quit = true; break;
						}
					}
					SDL_RenderPresent(gRenderer);	// update screen
				}

				if (showInferno){
					Mix_PlayMusic(infernoMusic, -1);
					SDL_RenderClear(gRenderer);
					handleInferno(backgroundRect);
					SDL_RenderPresent(gRenderer);
					SDL_Delay(5500);
					showInferno = false;
					inferno = true;
					AsteroidVelocityScale = 4;
				}

				while (showDeath){		// death menu
					if (Mix_PlayingMusic() == 0){
						Mix_PlayMusic(deathMusic,-1);
					}
					SDL_RenderClear(gRenderer);
					SDL_RenderCopy(gRenderer, Background, NULL, &backgroundRect);
					int button = handleDeathDisp();
					while (SDL_PollEvent( &e ) != 0){
						switch (handleMenuClick(e, button)){		//1: play, 2: main menu, 3: quit
							case 1: showDeath = false; Mix_HaltMusic(); 
								if (inferno){showInferno = true;}
								resetGame();
								break;
							case 2: showDeath = false; Mix_HaltMusic(); inferno = false; showMenu = true; resetGame(); break;
							case 3: showDeath = false; Mix_HaltMusic(); quit = true; break;
						}
					}
					SDL_RenderPresent(gRenderer);
				}

				if (quit){	// save some time when quitting
					break;
				}
				if (showMenu || showInferno){
					continue;
				}

				if (Mix_PlayingMusic() == 0){		// 
					Mix_PlayMusic(playMusic,-1);
				}
				// raise difficulty if score is mounting: every 10 hits
				if (score%10==1){
					plusDifficulty = true;
				}
				if (plusDifficulty && (score%10==0)){
					MAX_N_ASTEROIDS += 3;
					AsteroidVelocityScale++;
					plusDifficulty = false;
				}

				while (SDL_PollEvent( &e ) != 0){		// gameplay screen
					if(e.type == SDL_QUIT){
						quit = true;
					}
					gShip.handleEvent(e);		// shooting sound inside here
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
					bool isShot = false;

					for (std::vector<Bullet>::iterator it0 = Fired.begin(); it0 != Fired.end(); ){
						if (collided(*it0, *it1)){		// dereference iterators before sticking into function
							it0 = Fired.erase(it0);

					// AB FEATURE
							handleDeath(*it1);

							it1 = Asteroids.erase(it1);
							isShot = true;
							N_ASTEROIDS--;
							score++;
							break;
						}
						else{
							++it0;
						}
					}
					
					if (!isShot){
						if (Scollided(*it1)){
							showDeath = true;
							Mix_HaltMusic();		// stop whatever's playing
							Mix_PlayChannel(-1,shot,0);			// shot explosion sound here?
							explosion(inferno,infernoRect);		// takes care of loop for explosion animation. also renders everything else
							loadScore(score);
							break;
						}

						++it1;		// if shot, iterator already advanced.
					}
				}

			// AB FEATURE- add all the queue'd asteroids here

				for (std::vector<Asteroid>::iterator extraRock = ExtraAsteroids.begin(); extraRock != ExtraAsteroids.end(); ){
					Asteroids.push_back(*extraRock);
					extraRock = ExtraAsteroids.erase(extraRock);
				}

				if (showDeath){
					continue;
				}


				// all drawing: background, rocks, bullets, ship.
				SDL_RenderClear(gRenderer);		// clear screen

				renderExplosions();
			// AB FEATURE- add all the queue'd explosions here
				for (std::vector<SDL_Rect>::iterator extraExplosion = tempExplosionQueue.begin(); extraExplosion != tempExplosionQueue.end(); ){
					RockExplosions[0].push_back(*extraExplosion);
					extraExplosion = tempExplosionQueue.erase(extraExplosion);
				}

				renderAll(inferno,/*backgroundRect,*/infernoRect);		// comment out background for normal play
				gShip.render();			// don't render this in renderAll since we don't want it for the explosion
				
				SDL_RenderPresent(gRenderer);	// update screen
			
			}
		}
	}
	close();
	return 0;
}
