#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <math.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
int MAX_N_ASTEROIDS = 5;
int N_ASTEROIDS = 0;
int A_Counter = 0;

SDL_Renderer* gRenderer = NULL;

class Asteroid{
	public:
		Asteroid();
		~Asteroid();
		void free();

		bool loadFromFile(std::string path);		//Should we do the loading in the constructor?
		void move();

		void render();


	private:
		SDL_Texture* aTexture;
		int aPosX, aPosY;
		int aVelX, aVelY;		// randomly generate x and y velocities independently. let bouncing take care of direction
		int aOmega;			// randomly generate w to rotate at different speeds
		double aAngle;

		int aWidth, aHeight;

};


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
	SDL_Rect renderRect = {aPosX, aPosY, aWidth, aHeight};
	SDL_RenderCopyEx(gRenderer, aTexture, NULL, &renderRect, aAngle, NULL, SDL_FLIP_NONE);
}



class Ship{
	public:
		Ship();
		~Ship();
		static const int SHIP_VEL = 10;
		static const int SHIP_ANG_VEL = 5;

		bool loadFromFile( std::string path );
		void free();

		void handleEvent( SDL_Event &e );	// adjust pos/vel
		void move();

		void render();	// use class attributes to render

	private:
		SDL_Texture* sTexture;

		int sPosX, sPosY;
		int sVel, sOmega;
		double angle;

		int sWidth, sHeight;	// for rendering rectangle
};

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
//SDL_Renderer* gRenderer = NULL;

// our ship?!?!?!?!?! weird.. guess it's a global ship.
Ship gShip;
Asteroid aAsteroid1;

Ship::Ship(){
	sTexture = NULL;
//	sPosX = SCREEN_WIDTH/2;
//	sPosY = SCREEN_HEIGHT/2;
	sPosY = 100;
	sPosX = 10;
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






bool init(){
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("sdl initialize error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		//linear texture filtering
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		gWindow = SDL_CreateWindow("Bloody Big Ship", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL){
			printf("couldn't make window error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL){
				printf("couldn't make renderer error: %s\n", SDL_GetError() );
				success = false;
			}
			else{
				//set renderer draw colour
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//initialize png loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)){
					printf("couldn't initialize png, error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia(){
	bool success = true;

	// load ship!
	if (!(gShip.loadFromFile("falcon.png"))){
		printf("couldn't load ship\n");
		success = false;
	}

	if (!(aAsteroid1.loadFromFile("asteroid.png"))){
		printf("couldn't load asteroid\n");
		success = false;
	}
	return success;
}

void close(){
	gShip.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char* args[]){
	if (!init() ){
		printf("couldn't initialize\n");
	}
	else{
		if (!loadMedia() ){
			printf("couldn't load media \n");
		}
		else{
			// MAIN LOOP FLAG
			bool quit = false;
			SDL_Event e;
			//std::vector<Asteroid> Asteroids = {};

			while (!quit){
				// queue
				while (SDL_PollEvent( &e ) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					gShip.handleEvent(e);
				}
				/*
				if (N_ASTEROIDS < MAX_N_ASTEROIDS){
					
					Asteroid aName;
					if (!aName.loadFromFile("asteroid.png")){
						printf("couldn't load asteroid\n");
					}
					Asteroids.push_back(aName);
				}

				// for asteroid in asteroids: asteroid.move(), asteroid.render().
				*/
				aAsteroid1.move();
				gShip.move();
				
				SDL_RenderClear(gRenderer);		// clear screen

				aAsteroid1.render();
				gShip.render();
				SDL_RenderPresent(gRenderer);	// update screen
			}
		}
	}
	close();

	return 0;
}






