#include "Ship.h"
#include "globals.h"


Ship::Ship(){
	sTexture = NULL;
	sPosX = SCREEN_WIDTH/2;
	sPosY = SCREEN_HEIGHT/2;
	sVel = 0;
	sOmega = 0;
	angle = 0;
	sWidth = 0;
	sHeight = 0;
}

Ship::~Ship(){
	free();
}

bool Ship::loadFromFile(std::string path){
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadSurface = IMG_Load(path.c_str());
	if (loadSurface == NULL){
		printf("load %s error: %s\n", path.c_str(), IMG_GetError() );
	}
	else{
		sWidth = loadSurface->w;
		sHeight = loadSurface->h;

		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadSurface);
		if (newTexture == NULL){
			printf("create texture from %s error: %s\n", path.c_str(), SDL_GetError() );
		}
		SDL_FreeSurface(loadSurface);
	}

	sTexture = newTexture;
	return sTexture != NULL;

}


void Ship::free(){
	if (sTexture != NULL){
		SDL_DestroyTexture(sTexture);
		sTexture = NULL;
		sWidth = 0;
		sHeight = 0;
	}
}

void Ship::handleEvent(SDL_Event &e){
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch (e.key.keysym.sym){
			case SDLK_UP: sVel -= SHIP_VEL; break;		// seems in SDL the y motion direction is flipped.. but not the x
			case SDLK_LEFT: sOmega -= SHIP_ANG_VEL; break;
			case SDLK_RIGHT: sOmega += SHIP_ANG_VEL; break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_UP: sVel += SHIP_VEL; break;
			case SDLK_LEFT: sOmega += SHIP_ANG_VEL; break;
			case SDLK_RIGHT: sOmega -= SHIP_ANG_VEL; break;
		}
	}
}

void Ship::move(){
	angle += sOmega;
	double angle_rad = angle*3.14159/180;
	double cosx = cos(angle_rad);
	double sinx = sin(angle_rad);

	sPosY += ceil(sVel*cosx);
	sPosX -= ceil(sVel*sinx);	// counterintuitive, but these are since angle is defined relative to North (y axis)

	if ( (sPosX < 0) || (sPosX > SCREEN_WIDTH) ){
		sPosX += sVel*sinx;
	}

	if ( (sPosY < 0) || (sPosY > SCREEN_HEIGHT) ){
		sPosY -= sVel*cosx;
	}
}

void Ship::render(){
	SDL_Rect renderingRect = {sPosX-sWidth/2, sPosY-sHeight/2, sWidth, sHeight};	// left bound, lower bound, width, height
	SDL_RenderCopyEx( gRenderer, sTexture, NULL, &renderingRect, angle, NULL, SDL_FLIP_NONE);
}
