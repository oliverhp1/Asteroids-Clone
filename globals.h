#pragma once

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

#include "Asteroid.h"
#include "Ship.h"
#include "Bullet.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern int MAX_N_ASTEROIDS;
extern int N_ASTEROIDS;
extern int AsteroidVelocityScale;
extern int score;
extern int numExplosions;
extern int numAsteroidExplosions;

extern std::vector<Asteroid> Asteroids;
extern std::vector<Asteroid> ExtraAsteroids;

extern int AsteroidWidth0, AsteroidHeight0, BulletWidth, BulletHeight, AsteroidExplosionWidth, AsteroidExplosionHeight, ExplosionWidth, ExplosionHeight, InfernoWidth, InfernoHeight;


extern SDL_Texture* AsteroidTexture;
extern SDL_Texture* BulletTexture;
extern SDL_Texture* Background;
extern SDL_Texture* InfernoBackground;
extern SDL_Texture* ExplosionSpriteSheet;
extern SDL_Texture* AsteroidExplosionSpriteSheet;

extern SDL_Rect ExplosionClips[8];	// rectangles that split up the spritesheet
extern SDL_Rect AsteroidExplosionClips[4];

// AB FEATURE: array of vectors containing all explosions and where to render them
extern std::vector<SDL_Rect> rockExplosions[4];
extern std::vector<SDL_Rect> tempExplosionQueue;

extern Ship gShip;

extern std::vector<Bullet> Fired;

extern SDL_Renderer* gRenderer;
extern SDL_Window* gWindow;

extern SDL_Texture* TextTextures[17];
extern SDL_Rect TextTexture_R[17];

extern TTF_Font* laserFontB;
extern TTF_Font* bloodyFontB;
extern TTF_Font* laserFont;
extern TTF_Font* bloodyFont;
extern TTF_Font* laserFontS;
extern TTF_Font* bloodyFontS;

extern Mix_Music* menuMusic;
extern Mix_Music* playMusic;
extern Mix_Music* infernoMusic;
extern Mix_Music* deathMusic;
extern Mix_Chunk* shoot;
extern Mix_Chunk* shot;
extern Mix_Chunk* asteroidExplode;

extern SDL_Color mainColor;		// silver ish.  Also use this color for death screen selecting
extern SDL_Color mainColorH;	// metallic blue? but not too close to cyan
extern SDL_Color deathColor;	// slightly darker red

