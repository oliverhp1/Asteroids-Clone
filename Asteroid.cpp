#include "Asteroid.h"
#include "globals.h"

Asteroid::Asteroid(){
	// initialize with random pos/vel.  if 	rand_i = 0: left;	 1: right 		2: bottom		 3: top.
	int rand_i = rand()%4;
	switch (rand_i){
		case 0:
			aPosX = -1*screenOffset; aPosY = rand()%SCREEN_HEIGHT; break;
		case 1:
			aPosX = SCREEN_WIDTH+screenOffset; aPosY = rand()%SCREEN_HEIGHT; break;
		case 2:
			aPosY = -1*screenOffset; aPosX = rand()%SCREEN_WIDTH; break;
		case 3:
			aPosY = SCREEN_HEIGHT+screenOffset; aPosX = rand()%SCREEN_WIDTH; break;
		default:	// shouldn't happen
			aPosX = -1*screenOffset; aPosY = -1*screenOffset; break;
	}
	
	aVelX = (rand()%(SCREEN_WIDTH/200)+1) * AsteroidVelocityScale;
	aVelY = (rand()%(SCREEN_HEIGHT/200)+1) * AsteroidVelocityScale;
	aOmega = (rand()%10) * AsteroidVelocityScale;		// but angles are in radians, so approximately divide by 100
	if (rand_i < 2){
		aOmega *= -1;		// rotation in both directions
	}
	aAngle = 0;
	
// AB FEATURE
	sizeClass = 3;		// by default construct 3, shrink to 2 and 1 (med and small respectively)
	AsteroidWidth = AsteroidWidth0*sizeClass/4;
	AsteroidHeight = AsteroidHeight0*sizeClass/4;
}

Asteroid::Asteroid(int nSize, int nVelX, int nVelY, int nPosX, int nPosY){	// n stands for new

	int rand_i = 2*(rand()%2)-1;	// for random rotation: randomly generate 1 or -1
	aOmega = (rand()%10) * AsteroidVelocityScale; // * rand_i;

	aPosX = nPosX;
	aPosY = nPosY;
	aVelX = nVelX;
	aVelY = nVelY;
	sizeClass = nSize;

	aAngle = 0;
	AsteroidWidth = AsteroidWidth0*sizeClass/4;
	AsteroidHeight = AsteroidHeight0*sizeClass/4;
}

Asteroid::~Asteroid(){
}

int Asteroid::getPosX(){
	return aPosX;
}

int Asteroid::getPosY(){
	return aPosY;
}

int Asteroid::getW(){
	return AsteroidWidth;
}

int Asteroid::getH(){
	return AsteroidHeight;
}

int Asteroid::getSize(){
	return sizeClass;
}



// AB FEATURE: create 2 new asteroids or not

void Asteroid::handleDeath(){
/*	if ( (sizeClass > 3) || (sizeClass < 1) ){
		printf("invalid size\n");
		return;
	}
*/	
	// geometry- use velocity magnitudes to change direction by +/- 45 degrees
	double vMag = sqrt(aVelX*aVelX + aVelY*aVelY);
	double sinA = aVelY/vMag;
	double cosA = aVelX/vMag;
	double vMod = vMag/sqrt(2);
	if (sizeClass == 3){
		// These are all simplified angle addition expressions
		Asteroids.push_back(Asteroid(2,(int) (vMod*(cosA-sinA)), (int) (vMod*(cosA+sinA)), aPosX, aPosY));
		Asteroids.push_back(Asteroid(2,(int) (vMod*(sinA+cosA)), (int) (vMod*(sinA-cosA)), aPosX, aPosY));
		N_ASTEROIDS += 2;
	}
	else if (sizeClass == 2){
		Asteroids.push_back(Asteroid(1,(int) (vMod*(cosA-sinA)), (int) (vMod*(cosA+sinA)), aPosX, aPosY));
		Asteroids.push_back(Asteroid(1,(int) (vMod*(sinA+cosA)), (int) (vMod*(sinA-cosA)), aPosX, aPosY));
		N_ASTEROIDS += 2;
	}
}





void Asteroid::move(){
	aPosY -= aVelY;
	aPosX += aVelX;
	aAngle += aOmega;

	if ( (aPosX < -1*screenOffset) || (aPosX > SCREEN_WIDTH+screenOffset) ){	// elastic collisions? or we can just get rid of them.
		aVelX *= -1;
	}
	if ( (aPosY < -1*screenOffset) || (aPosY > SCREEN_HEIGHT+screenOffset) ){
		aVelY *= -1;
	}
}

void Asteroid::render(){
	SDL_Rect renderRect = {aPosX-AsteroidWidth/2, aPosY-AsteroidHeight/2, AsteroidWidth, AsteroidHeight};
	SDL_RenderCopyEx(gRenderer, AsteroidTexture, NULL, &renderRect, aAngle, NULL, SDL_FLIP_NONE);
}

