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
		~Asteroid();

		static const int screenOffset = 50;
		void move();
		
		int getPosX();
		int getPosY();
		int getW();
		int getH();

		void render();


	private:
		int aPosX, aPosY;
		int aVelX, aVelY;		// randomly generate x and y velocities independently. let bouncing take care of direction
		int aOmega;			// randomly generate w to rotate at different speeds
		double aAngle;
		int AsteroidWidth, AsteroidHeight;
};
