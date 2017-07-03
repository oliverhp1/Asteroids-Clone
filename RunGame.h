#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

#include "Asteroid.h"
#include "Bullet.h"
#include "Ship.h"


bool init();
bool loadMedia();
bool loadFontFromText(std::string, SDL_Color, bool, bool);	// bool = true for laser, false for bloody. 2nd: main menu true
void close();
bool collided(Bullet, Asteroid);
bool Scollided(Asteroid);
