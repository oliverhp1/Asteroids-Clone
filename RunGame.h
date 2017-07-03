#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

#include "Asteroid.h"
#include "Bullet.h"
#include "Ship.h"


bool init();
bool loadMedia();

// all 3 of these for the menus
SDL_Surface* loadFontFromText(std::string, SDL_Color, bool, bool);	// bool = true for laser; 2nd bool = true for big
void handleMenu();
bool loadMainMenu();
bool loadDeathScreen();

void close();
bool collided(Bullet, Asteroid);
bool Scollided(Asteroid);
