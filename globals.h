#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "Asteroid.h"
#include "Ship.h"
#include "Bullet.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const int MAX_N_ASTEROIDS;
extern int N_ASTEROIDS;
extern int score;
extern std::vector<Asteroid> Asteroids;

extern int AsteroidWidth0, AsteroidHeight0, BulletWidth, BulletHeight;


extern SDL_Texture* AsteroidTexture;
extern SDL_Texture* BulletTexture;
extern SDL_Texture* Background;

extern Ship gShip;

extern std::vector<Bullet> Fired;

extern SDL_Renderer* gRenderer;
extern SDL_Window* gWindow;

extern std::vector<SDL_Texture*> MainMenuText;
extern std::vector<SDL_Texture*> DeathText;

extern TTF_Font* laserFont;
extern TTF_Font* bloodyFont;