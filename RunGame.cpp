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
		gWindow = SDL_CreateWindow("Asteroids?!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
				if (TTF_Init() == -1){
					printf("couldn't initialize ttf, error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia(){		// load global textures for asteroid and bullet, background and text
	bool success = true;

	laserFontB = TTF_OpenFont("fonts/laser.ttf", SCREEN_HEIGHT/10);
	if ( (laserFontB == NULL) ){
		printf("couldn't load laser font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	bloodyFontB = TTF_OpenFont("fonts/bloody.ttf", SCREEN_HEIGHT/8);
	if ((bloodyFontB == NULL) ){
		printf("couldn't load bloody font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	laserFont = TTF_OpenFont("fonts/laser.ttf", SCREEN_HEIGHT/16);
	if ( (laserFont == NULL) ){
		printf("couldn't load laser font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	bloodyFont = TTF_OpenFont("fonts/bloody.ttf", SCREEN_HEIGHT/10);
	if ((bloodyFont == NULL) ){
		printf("couldn't load bloody font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	// load ship!
	if (!(gShip.loadFromFile("images/falcon_tiny.png"))){
		printf("couldn't load ship\n");
		success = false;
	}

	// load Global Asteroid texture
	std::string pathA = "images/asteroid.png";
	SDL_Surface* loadSurface = IMG_Load(pathA.c_str());
	if (loadSurface == NULL){
		printf("load surface %s error: %s", "asteroid.png", IMG_GetError() );
		success = false;
	}
	else{
		SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 00,00,00) );

		AsteroidWidth0 = loadSurface->w;
		AsteroidHeight0 = loadSurface->h;

		AsteroidTexture = SDL_CreateTextureFromSurface(gRenderer,loadSurface);
		if (AsteroidTexture == NULL){
			printf("create texture error: %s", SDL_GetError() );
			success = false;
		}
		SDL_FreeSurface(loadSurface);
		loadSurface = NULL;
	}

	// load global Bullet texture
	std::string pathB = "images/bullet_tiny.png";
	loadSurface = IMG_Load(pathB.c_str());
	if (loadSurface == NULL){
		printf("load surface %s error: %s", "bullet.png", IMG_GetError() );
		success = false;
	}
	else{
		SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 00,00,00) );

		BulletWidth = loadSurface->w;
		BulletHeight = loadSurface->h;

		BulletTexture = SDL_CreateTextureFromSurface(gRenderer,loadSurface);
		if (BulletTexture == NULL){
			printf("create texture error: %s", SDL_GetError() );
			success = false;
		}
		SDL_FreeSurface(loadSurface);
		loadSurface = NULL;
	}

	// load global Background texture
	std::string pathC = "images/black_medium.png";
	loadSurface = IMG_Load(pathC.c_str() );
	if (loadSurface == NULL){
		printf("load background error: %s\n", IMG_GetError() );
		success = false;
	}
	else{
		Background = SDL_CreateTextureFromSurface(gRenderer, loadSurface);
		if (Background == NULL){
			printf("background texture error: %s", SDL_GetError() );
			success = false;
		}
		SDL_FreeSurface(loadSurface);
		loadSurface = NULL;
	}

	// load main menu and death menu textures
	if (!loadMainMenu()){
		printf("error loading main menu textures");
		success = false;
	}

	if (!loadDeathScreen()){
		printf("error loading death screen textures");
		success = false;
	}

	return success;
}


SDL_Surface* loadFontFromText(std::string text, SDL_Color colorMap, bool laser, bool big){
	if (laser && big){		// laser text and main menu
		return TTF_RenderText_Solid( laserFontB, text.c_str(), colorMap);
	}
	else if (laser){
		return TTF_RenderText_Solid( laserFont, text.c_str(), colorMap);
	}
	else if (big){
		return TTF_RenderText_Solid( bloodyFontB, text.c_str(), colorMap);
	}
	else{
		return TTF_RenderText_Solid( bloodyFont, text.c_str(), colorMap);
	}
}

void handleMenu(){
	// render texts based on mouse position. create global rectangles and colors to use? 
	// first just stick in some text, do mouse shit later
	SDL_RenderCopyEx(gRenderer, TextTextures[Main_Asteroids], NULL, &TextTexture_R[Main_Asteroids], 0, NULL, SDL_FLIP_NONE);


}

bool loadMainMenu(){	// load up main menu textures into TextTextures, rectangles into TextTexture_R
	SDL_Surface* tempSurface = NULL;
	tempSurface = TTF_RenderText_Solid( laserFontB, "ASTEROIDS", mainColor );
	SDL_Rect tempRect = {SCREEN_WIDTH/2 - (tempSurface->w)/2, SCREEN_HEIGHT/8, tempSurface->w, tempSurface->h};
	TextTexture_R[Main_Asteroids] = tempRect;
	TextTextures[Main_Asteroids] = SDL_CreateTextureFromSurface(gRenderer, loadFontFromText("ASTEROIDS", mainColor, true, true));
/*

	TextTextures[Main_Play] = SDL_CreateTextureFromSurface(gRenderer, loadFontFromText("PLAY GAME", mainColor, true, false));
	TextTextures[Main_Play_H] = SDL_CreateTextureFromSurface(gRenderer, loadFontFromText("PLAY GAME", mainColorH, true, false));
	TextTextures[Main_Instruct] = SDL_CreateTextureFromSurface(gRenderer, loadFontFromText("Instructions", mainColor, true, false));
	TextTextures[Main_Instruct_H] = SDL_CreateTextureFromSurface(gRenderer, loadFontFromText("Instructions", mainColorH, true, false));
	TextTextures[Main_Quit] = SDL_CreateTextureFromSurface(gRenderer, loadFontFromText("Quit", mainColor, true, false));
	TextTextures[Main_Quit_H] = SDL_CreateTextureFromSurface(gRenderer, loadFontFromText("Quit", mainColorH, true, false));

	
	TextTextures[Main_Play] = 
	TextTextures[Main_Play_H] = 
	TextTextures[Main_Instruct] = 
	TextTextures[Main_Instruct_H] = 
	TextTextures[Main_Quit] = 
	TextTextures[Main_Quit_H] = 

*/	return 1;
}

bool loadDeathScreen(){		// loads death menu TextTextures and rectangles. note, leave out the score one.
	return 1;
}

void close(){
	gShip.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	TTF_CloseFont(laserFont);
	TTF_CloseFont(bloodyFont);

	for (int d = 0; d < 13; d++){
		SDL_DestroyTexture(TextTextures[d]);
	}
	
	SDL_DestroyTexture(AsteroidTexture);
	SDL_DestroyTexture(BulletTexture);

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

bool collided(Bullet b1, Asteroid a1){
	double angle = b1.getAngle();
	angle *= (3.14/180);
	int bTipX = b1.getPosX() + BulletHeight/2*sin(angle);	// Some geometry, draw out the bullet as a rectangle and find the tip position
	int bTipY = b1.getPosY() - BulletHeight/2*cos(angle);
	int deltaX = bTipX - a1.getPosX();
	int deltaY = bTipY - a1.getPosY();
	if (deltaX*deltaX + deltaY*deltaY <= (a1.getW())*(a1.getH())/4){
		return true;
	}
	else{
		return false;
	}
}

bool Scollided(Asteroid a1){	// ship is global: gShip
	double angle = gShip.getA();
	angle *= (3.14/180);

	int sWidth = gShip.getW();
	int sHeight = gShip.getH();
	int sCircleX = gShip.getX()-sHeight*2/15*sin(angle);
	int sCircleY = gShip.getY()+sHeight*2/15*cos(angle);

	int sTipYL = gShip.getY()-sHeight/2*cos(angle) - sWidth/11*sin(angle);
	int sTipYR = gShip.getY()-sHeight/2*cos(angle) + sWidth/11*sin(angle);
	int sTipXL = gShip.getX()-sWidth/11*cos(angle) + sHeight/2*sin(angle);
	int sTipXR = gShip.getX()+sWidth/11*cos(angle) + sHeight/2*sin(angle);

	int aX = a1.getPosX();
	int aY = a1.getPosY();
	
	int dX = sCircleX - aX;
	int dY = sCircleY - aY;

	int dXL = sTipXL - aX;
	int dXR = sTipXR - aX;
	int dYL = sTipYL - aY;
	int dYR = sTipYR - aY;

	int Rs = sWidth/2;
	int Ra = (a1.getW())/2;

	if (dX*dX + dY*dY < (Rs+Ra)*(Rs+Ra)){
		return true;
	}
	else if ( (dXL*dXL + dYL*dYL < Ra*Ra) || (dXR*dXR + dYR*dYR < Ra*Ra) ){
		return true;
	}
	else{
		return false;
	}
}


