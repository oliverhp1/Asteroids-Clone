#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>
#include <math.h>

class Bullet{
public:
	Bullet();
	~Bullet();
//	void free();

	static const int bVelocity = 23;

	double getAngle();
	int getPosX();
	int getPosY();

	bool move();
	void render();

private:
//	SDL_Texture* bTexture;
	double bAngle;
	int bPosX, bPosY;
//	int bWidth, bHeight;	// for rendering rectangle
};
