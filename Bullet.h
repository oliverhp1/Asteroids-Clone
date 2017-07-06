#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <stdio.h>
#include <math.h>

class Bullet{
public:
	Bullet();
	~Bullet();

	static const int bVelocity = 23;

	double getAngle();
	int getPosX();
	int getPosY();

	bool move();
	void render();

private:
	double bAngle;
	int bPosX, bPosY;
};
