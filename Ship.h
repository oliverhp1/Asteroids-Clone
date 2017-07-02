#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <math.h>


class Ship{
	public:
		Ship();
		~Ship();
		static const int SHIP_VEL = 12;
		static const int SHIP_ANG_VEL = 8;

		bool loadFromFile( std::string path );
		void free();

		void handleEvent( SDL_Event &e );	// adjust pos/vel
		void move();

		void render();	// use class attributes to render

	private:
		SDL_Texture* sTexture;

		int sPosX, sPosY;
		int sVel, sOmega;
		double angle;

		int sWidth, sHeight;	// for rendering rectangle
};
