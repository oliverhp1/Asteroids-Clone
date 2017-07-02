#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <math.h>

class Asteroid{
	public:
		Asteroid();
		~Asteroid();
		void free();

		bool loadFromFile(std::string path);		//Should we do the loading in the constructor?
		void move();

		void render();


	private:
		SDL_Texture* aTexture;
		int aPosX, aPosY;
		int aVelX, aVelY;		// randomly generate x and y velocities independently. let bouncing take care of direction
		int aOmega;			// randomly generate w to rotate at different speeds
		double aAngle;

		int aWidth, aHeight;
};
