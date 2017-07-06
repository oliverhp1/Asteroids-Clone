#include "Bullet.h"
#include "globals.h"

Bullet::Bullet(){
	bAngle = gShip.getA();
	bPosX = gShip.getX();
	bPosY = gShip.getY();
}

Bullet::~Bullet(){
}

double Bullet::getAngle(){
	return bAngle;
}

int Bullet::getPosX(){
	return bPosX;
}

int Bullet::getPosY(){
	return bPosY;
}

bool Bullet::move(){		// make this a bool, return whether it's out of the screen!
	bool gone = false;

	double bAngle_rad = bAngle*3.14/180;
	bPosY -= bVelocity*cos(bAngle_rad);
	bPosX += bVelocity*sin(bAngle_rad);

	if ( (bPosX < 0) || (bPosX > SCREEN_WIDTH) || (bPosY < 0) || (bPosY > SCREEN_HEIGHT) ){
		gone = true;
	}

	return gone;
}

void Bullet::render(){
	SDL_Rect renderingRect = {bPosX - BulletWidth/2, bPosY - BulletHeight/2, BulletWidth, BulletHeight};

	SDL_RenderCopyEx( gRenderer, BulletTexture, NULL, &renderingRect, bAngle, NULL, SDL_FLIP_NONE);
}




