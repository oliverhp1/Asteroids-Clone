#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <math.h>

class Asteroid{
	public:
		Asteroid();
		Asteroid(int,int,int,int,int);
		~Asteroid();

		static const int screenOffset = 50;
		void move();
		
		int getPosX();
		int getPosY();
		int getW();
		int getH();
		int getSize();
// AB FEATURE
		void handleDeath();

		void render();


	private:
		int aPosX, aPosY;
		int aVelX, aVelY;		// randomly generate x and y velocities independently. let bouncing take care of direction
		int aOmega;			// randomly generate w (angular velocity)
		double aAngle;
		int sizeClass;
		int AsteroidWidth, AsteroidHeight;
};
