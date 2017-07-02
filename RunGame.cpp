#include "RunGame.h"
#include "globals.h"


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
	if (!(gShip.loadFromFile("images/falcon.png"))){
		printf("couldn't load ship\n");
		success = false;
	}

	for (int k = 0; k < MAX_N_ASTEROIDS; k++){
		if (!(Asteroids[k].loadFromFile("images/asteroid.png"))){
			printf("couldn't load asteroid\n");
			success = false;
		}
	}
	return success;
}

void close(){
	gShip.free();

	for (int n = 0; n < MAX_N_ASTEROIDS; n++){
		Asteroids[n].free();
	}	

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
	IMG_Quit();
}
