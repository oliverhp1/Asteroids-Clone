#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>


class Ship{
	public:
		Ship();
		~Ship();
		static const int SHIP_VEL = 15;
		static const int SHIP_ANG_VEL = 10;
		static const int offScreen = 20;		// AB FEATURE

		bool loadFromFile( std::string path );
		void free();

		void handleEvent( SDL_Event &e );	// adjust pos/vel
		void move();

		void render();
		
		int getX();
		int getY();
		double getA();
		int getW();
		int getH();
		void resetPosition();

	private:
		SDL_Texture* sTexture;
		SDL_Point* sCenter;

		int sPosX, sPosY;
		int sVel, sOmega;
		double angle;

		int sWidth, sHeight;	// for rendering rectangle
};
