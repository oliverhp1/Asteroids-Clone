#include "Asteroid.h"


Asteroid::Asteroid(){
	// initialize with random pos/vel.  if 	rand_i = 0: left;	 1: right 		2: bottom		 3: top.
	int rand_i = rand()%4;
	switch (rand_i){
		case 0:
			aPosX = 0; aPosY = rand()%SCREEN_HEIGHT; break;
		case 1:
			aPosX = SCREEN_WIDTH; aPosY = rand()%SCREEN_HEIGHT; break;
		case 2:
			aPosY = 0; aPosX = rand()%SCREEN_WIDTH; break;
		case 3:
			aPosY = SCREEN_HEIGHT; aPosX = rand()%SCREEN_WIDTH; break;
		default:	// shouldn't happen
			aPosX = 0; aPosY = 0; break;
	}
	aTexture = NULL;
	aVelX = rand()%(SCREEN_WIDTH/100)+2;
	aVelY = rand()%(SCREEN_HEIGHT/100)+2;
	aOmega = rand()%10;		// but angles are in radians, so approximately divide by 100
	if (rand_i < 2){
		aOmega *= -1;		// rotation in both directions
	}
	aAngle = 0;
	N_ASTEROIDS++;
}

Asteroid::~Asteroid(){
	N_ASTEROIDS--;
	free();
}

void Asteroid::free(){
	if (aTexture != NULL){
		SDL_DestroyTexture(aTexture);
		aTexture = NULL;
	}
}

bool Asteroid::loadFromFile(std::string path){
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadSurface = IMG_Load(path.c_str() );
	if (loadSurface == NULL){
		printf("load surface %s error: %s", path.c_str(), IMG_GetError() );
	}
	else{
		aWidth = loadSurface->w;
		aHeight = loadSurface->h;

		newTexture = SDL_CreateTextureFromSurface(gRenderer,loadSurface);
		if (newTexture == NULL){
			printf("create texture error: %s", SDL_GetError() );
		}
		SDL_FreeSurface(loadSurface);
	}

	aTexture = newTexture;
	return aTexture != NULL;
}

void Asteroid::move(){
	aPosY -= aVelY;
	aPosX += aVelX;
	aAngle += aOmega;

	if ( (aPosX < 0) || (aPosX > SCREEN_WIDTH) ){	// elastic collisions? or we can just get rid of them.
		aVelX *= -1;
	}
	if ( (aPosY < 0) || (aPosY > SCREEN_HEIGHT) ){
		aVelY *= -1;
	}
}

void Asteroid::render(){
	SDL_Rect renderRect = {aPosX-aWidth/2, aPosY-aHeight/2, aWidth, aHeight};
	SDL_RenderCopyEx(gRenderer, aTexture, NULL, &renderRect, aAngle, NULL, SDL_FLIP_NONE);
}

