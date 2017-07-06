#include "RunGame.h"
#include "globals.h"


enum TextTexture_I {Main_Asteroids, Main_Play, Main_Play_H, Main_Instruct, Main_Instruct_H, Main_Quit, Main_Quit_H, Instructions_Screen_U, Instructions_Screen_D, Death_Dead, Death_Score, Death_Play, Death_Play_H, Death_Quit, Death_Quit_H, Death_Return, Death_Return_H};


bool init(){
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
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
				SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );	// black or white?

				//initialize png loading
				int imgFlags = IMG_INIT_PNG|IMG_INIT_JPG;
				if (!(IMG_Init(imgFlags) & imgFlags)){
					printf("couldn't initialize png and jpg, error: %s\n", IMG_GetError() );
					success = false;
				}
				if (TTF_Init() == -1){
					printf("couldn't initialize ttf, error: %s\n", TTF_GetError() );
					success = false;
				}
				if (MIX_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){			// if laggy, play with this (bitrate)
					printf("couldn't initialize mix, error: %s\n", MIX_GetError() );
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

	bloodyFontB = TTF_OpenFont("fonts/bloody.ttf", SCREEN_HEIGHT/6);
	if ((bloodyFontB == NULL) ){
		printf("couldn't load bloody font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	laserFont = TTF_OpenFont("fonts/laser.ttf", SCREEN_HEIGHT/16);
	if ( (laserFont == NULL) ){
		printf("couldn't load laser font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	bloodyFont = TTF_OpenFont("fonts/bloody.ttf", SCREEN_HEIGHT/11);
	if ((bloodyFont == NULL) ){
		printf("couldn't load bloody font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	laserFontS = TTF_OpenFont("fonts/laser.ttf", SCREEN_HEIGHT/20);
	if ( (laserFont == NULL) ){
		printf("couldn't load laser font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	bloodyFontS = TTF_OpenFont("fonts/bloody.ttf", SCREEN_HEIGHT/20);
	if ((bloodyFont == NULL) ){
		printf("couldn't load bloody font, error: %s\n", TTF_GetError() );
		success = 0;
	}

	// load ship!
	if (!(gShip.loadFromFile("images/falcon3.png"))){
		printf("couldn't load ship\n");
		success = false;
	}

	// load Global Asteroid texture
	std::string pathA = "images/asteroid.png";
	SDL_Surface* loadSurface = IMG_Load(pathA.c_str());
	if (loadSurface == NULL){
		printf("load surface %s error: %s", pathA.c_str(), IMG_GetError() );
		success = false;
	}
	else{
		SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 0xFF,0xFF,0xFF) );

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
	std::string pathB = "images/bullet.png";	// bullet is a transparent png
	loadSurface = IMG_Load(pathB.c_str());
	if (loadSurface == NULL){
		printf("load surface %s error: %s", pathB.c_str(), IMG_GetError() );
		success = false;
	}
	else{
		//SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 0, 0, 0) );
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
	loadSurface = IMG_Load(pathC.c_str());
	if (loadSurface == NULL){
		printf("load background error: %s\n", IMG_GetError() );
		success = false;
	}
	else{
		Background = SDL_CreateTextureFromSurface(gRenderer, loadSurface);
		if (Background == NULL){
			printf("background texture error: %s\n", SDL_GetError() );
			success = false;
		}
		SDL_FreeSurface(loadSurface);
		loadSurface = NULL;
	}

	// load global inferno texture
	std::string pathI = "images/inferno.png";
	loadSurface = IMG_Load(pathI.c_str());
	if (loadSurface == NULL){
		printf("error loading inferno: %s\n", IMG_GetError());
		success = false;
	}
	else{
		InfernoBackground = SDL_CreateTextureFromSurface(gRenderer, loadSurface);
		if (InfernoBackground == NULL){
			printf("error making inferno texture: %s\n", SDL_GetError() );
			success = false;
		}
		SDL_FreeSurface(loadSurface);
		loadSurface = NULL;
	}

	// load music
	menuMusic = MIX_LoadMUS("sounds/imperial_march.wav");
	if (menuMusic == NULL){printf("couldn't load menu music, error: %s\n",MIX_GetError()); success = false;}
	deathMusic = MIX_LoadMUS("sounds/reaper_of_souls.wav");
	if (menuMusic == NULL){printf("couldn't load death music, error: %s\n",MIX_GetError()); success = false;}
	shoot = MIX_LoadWAV("sounds/gun0.wav");
	if (shoot == NULL){printf("couldn't load shoot sound, error: %s\n",MIX_GetError()); success = false;}
	shot = MIX_LoadWAV("sounds/explosion.wav");
	if (shot == NULL){printf("couldn't load shot sound, error: %s\n",MIX_GetError()); success = false;}

	// load main menu and death menu textures, and explosion sprite sheet
	loadMainMenu();
	if (!loadExplosion()){
		success = false;
	}
	loadDeathScreen();

	return success;
}

int handleMenuDisp(){		// return 1: play, 2: instructions, 3: quit.
	int mX = 0;
	int mY = 0;		// mouse coordinates
	bool overPlay = false;
	bool overInstruct = false;
	bool overQuit = false;

	int res = 0;	// return 1 for Play, 2 for Instructions, 3 for Quit

	SDL_GetMouseState(&mX, &mY);

	if ( (mX > TextTexture_R[Main_Play].x) && (mX < TextTexture_R[Main_Play].x + TextTexture_R[Main_Play].w) && (mY > TextTexture_R[Main_Play].y) && (mY < TextTexture_R[Main_Play].y + TextTexture_R[Main_Play].h)){
		overPlay = true;
		res = 1;
	}
	else if ( (mX > TextTexture_R[Main_Instruct].x) && (mX < TextTexture_R[Main_Instruct].x + TextTexture_R[Main_Instruct].w) && (mY > TextTexture_R[Main_Instruct].y) && (mY < TextTexture_R[Main_Instruct].y + TextTexture_R[Main_Instruct].h)){
		overInstruct = true;
		res = 2;
	}
	else if ( (mX > TextTexture_R[Main_Quit].x) && (mX < TextTexture_R[Main_Quit].x + TextTexture_R[Main_Quit].w) && (mY > TextTexture_R[Main_Quit].y) && (mY < TextTexture_R[Main_Quit].y + TextTexture_R[Main_Quit].h)){
		overQuit = true;
		res = 3;
	}

	SDL_RenderCopyEx(gRenderer, TextTextures[Main_Asteroids], NULL, &TextTexture_R[Main_Asteroids], 0, NULL, SDL_FLIP_NONE);

	if (!overPlay){
		SDL_RenderCopyEx(gRenderer, TextTextures[Main_Play], NULL, &TextTexture_R[Main_Play], 0, NULL, SDL_FLIP_NONE);
	}
	else{
		SDL_RenderCopyEx(gRenderer, TextTextures[Main_Play_H], NULL, &TextTexture_R[Main_Play_H], 0, NULL, SDL_FLIP_NONE);
	}

	if (!overInstruct){
		SDL_RenderCopyEx(gRenderer, TextTextures[Main_Instruct], NULL, &TextTexture_R[Main_Instruct], 0, NULL, SDL_FLIP_NONE);
	}
	else{
		SDL_RenderCopyEx(gRenderer, TextTextures[Main_Instruct_H], NULL, &TextTexture_R[Main_Instruct_H], 0, NULL, SDL_FLIP_NONE);
	}

	if (!overQuit){
		SDL_RenderCopyEx(gRenderer, TextTextures[Main_Quit], NULL, &TextTexture_R[Main_Quit], 0, NULL, SDL_FLIP_NONE);
	}
	else{
		SDL_RenderCopyEx(gRenderer, TextTextures[Main_Quit_H], NULL, &TextTexture_R[Main_Quit_H], 0, NULL, SDL_FLIP_NONE);
	}
	return res;
}


int handleMenuClick(SDL_Event &e, int button){		// uses hover info from other function; determines if clicked
	if (e.type == SDL_QUIT){
			return 3;
	}
	else if ( (e.type != SDL_MOUSEBUTTONDOWN) || (e.button.button != SDL_BUTTON_LEFT) ){	// if not a left click
		return 0;
	}
	else{	// if we get here then the left mouse button was pressed
		return button;
	}
}


int handleDeathDisp(){
	int mX = 0;
	int mY = 0;
	bool overPlay = false;
	bool overReturn = false;
	bool overQuit = false;

	int res = 0;	// 3 for quit, 1 for play (so we can reuse the handleMenuClick method)
	SDL_GetMouseState(&mX, &mY);

	if ( (mX > TextTexture_R[Death_Play].x) && (mX < TextTexture_R[Death_Play].x + TextTexture_R[Death_Play].w) && (mY > TextTexture_R[Death_Play].y) && (mY < TextTexture_R[Death_Play].y + TextTexture_R[Death_Play].h)){
		overPlay = true;
		res = 1;
	}
	else if ( (mX > TextTexture_R[Death_Return].x) && (mX < TextTexture_R[Death_Return].x + TextTexture_R[Death_Return].w) && (mY > TextTexture_R[Death_Return].y) && (mY < TextTexture_R[Death_Return].y + TextTexture_R[Death_Return].h)){
		overReturn = true;
		res = 2;
	}
	else if ( (mX > TextTexture_R[Death_Quit].x) && (mX < TextTexture_R[Death_Quit].x + TextTexture_R[Death_Quit].w) && (mY > TextTexture_R[Death_Quit].y) && (mY < TextTexture_R[Death_Quit].y + TextTexture_R[Death_Quit].h)){
		overQuit = true;
		res = 3;
	}
	SDL_RenderCopyEx(gRenderer, TextTextures[Death_Dead], NULL, &TextTexture_R[Death_Dead], 0, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(gRenderer, TextTextures[Death_Score], NULL, &TextTexture_R[Death_Score], 0, NULL, SDL_FLIP_NONE);

	if (!overPlay){
		SDL_RenderCopyEx(gRenderer, TextTextures[Death_Play], NULL, &TextTexture_R[Death_Play], 0, NULL, SDL_FLIP_NONE);
	}
	else{
		SDL_RenderCopyEx(gRenderer, TextTextures[Death_Play_H], NULL, &TextTexture_R[Death_Play_H], 0, NULL, SDL_FLIP_NONE);
	}

	if (!overReturn){
		SDL_RenderCopyEx(gRenderer, TextTextures[Death_Return], NULL, &TextTexture_R[Death_Return], 0, NULL, SDL_FLIP_NONE);
	}
	else{
		SDL_RenderCopyEx(gRenderer, TextTextures[Death_Return_H], NULL, &TextTexture_R[Death_Return_H], 0, NULL, SDL_FLIP_NONE);
	}

	if (!overQuit){
		SDL_RenderCopyEx(gRenderer, TextTextures[Death_Quit], NULL, &TextTexture_R[Death_Quit], 0, NULL, SDL_FLIP_NONE);
	}
	else{
		SDL_RenderCopyEx(gRenderer, TextTextures[Death_Quit_H], NULL, &TextTexture_R[Death_Quit_H], 0, NULL, SDL_FLIP_NONE);
	}

	return res;
}

void handleInferno(SDL_Rect iRect){
	SDL_RenderCopy(gRenderer, Background, NULL, &iRect);
	SDL_RenderCopyEx(gRenderer, TextTextures[Instructions_Screen_U], NULL, &TextTexture_R[Instructions_Screen_U], 0, NULL, SDL_FLIP_NONE);					
	SDL_RenderCopyEx(gRenderer, TextTextures[Instructions_Screen_D], NULL, &TextTexture_R[Instructions_Screen_D], 0, NULL, SDL_FLIP_NONE);					
}


void loadMainMenu(){	// load up main menu textures into TextTextures, rectangles into TextTexture_R
	// since we have to manually place all the text textures, abstracting these isn't much more convenient
	SDL_Surface* tempSurface = NULL;
	tempSurface = TTF_RenderText_Solid( laserFontB, "ASTEROIDS", mainColor );
	SDL_Rect tempRect = {SCREEN_WIDTH/2 - (tempSurface->w)/2, SCREEN_HEIGHT/8, tempSurface->w, tempSurface->h};
	TextTexture_R[Main_Asteroids] = tempRect;
	TextTextures[Main_Asteroids] = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = TTF_RenderText_Solid( laserFont, "PLAY GAME", mainColor );
	tempRect = {SCREEN_WIDTH/2 - (tempSurface->w)/2, 2*SCREEN_HEIGHT/5, tempSurface->w, tempSurface->h};
	TextTexture_R[Main_Play] = tempRect;
	TextTextures[Main_Play] = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	
	tempSurface = TTF_RenderText_Solid( laserFont, "PLAY GAME", mainColorH );
	tempRect = {SCREEN_WIDTH/2 - (tempSurface->w)/2+5, 2*SCREEN_HEIGHT/5-5, tempSurface->w, tempSurface->h};
	TextTexture_R[Main_Play_H] = tempRect;
	TextTextures[Main_Play_H] = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	
	tempSurface = TTF_RenderText_Solid( laserFont, "Instructions", mainColor );
	tempRect = {SCREEN_WIDTH/2 - (tempSurface->w)/2, 3*SCREEN_HEIGHT/5, tempSurface->w, tempSurface->h};
	TextTexture_R[Main_Instruct] = tempRect;
	TextTextures[Main_Instruct] = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = TTF_RenderText_Solid( laserFont, "Instructions", mainColorH );
	tempRect = {SCREEN_WIDTH/2 - (tempSurface->w)/2+5, 3*SCREEN_HEIGHT/5-5, tempSurface->w, tempSurface->h};
	TextTexture_R[Main_Instruct_H] = tempRect;
	TextTextures[Main_Instruct_H] = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = TTF_RenderText_Solid( laserFont, "Quit", mainColor );
	tempRect = {SCREEN_WIDTH/2 - (tempSurface->w)/2, 4*SCREEN_HEIGHT/5, tempSurface->w, tempSurface->h};
	TextTexture_R[Main_Quit] = tempRect;
	TextTextures[Main_Quit] = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = TTF_RenderText_Solid( laserFont, "Quit", mainColorH );
	tempRect = {SCREEN_WIDTH/2 - (tempSurface->w)/2+5, 4*SCREEN_HEIGHT/5-5, tempSurface->w, tempSurface->h};
	TextTexture_R[Main_Quit_H] = tempRect;
	TextTextures[Main_Quit_H] = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	tempSurface = NULL;
}

void loadDeathScreen(){		// loads death menu TextTextures and rectangles. note, leave out the score til game over.
	// since we have to manually place all the text textures, abstracting these isn't much more convenient
	SDL_Surface* tempSurface1 = NULL;

	tempSurface1 = TTF_RenderText_Solid( bloodyFontB, "PREPARE FOR", deathColor );
	SDL_Rect tempRect = {SCREEN_WIDTH/2 - 3*(tempSurface1->w)/5, SCREEN_HEIGHT/3 - (tempSurface1->h)/4, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Instructions_Screen_U] = tempRect;
	TextTextures[Instructions_Screen_U] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);

	tempSurface1 = TTF_RenderText_Solid( bloodyFontB, "INFERNO MODE", deathColor );
	tempRect = {SCREEN_WIDTH/2 - (tempSurface1->w)/2, SCREEN_HEIGHT/2 + (tempSurface1->h)/4, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Instructions_Screen_D] = tempRect;
	TextTextures[Instructions_Screen_D] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);

	tempSurface1 = TTF_RenderText_Solid( bloodyFontB, "YOU HAVE DIED", deathColor );
	tempRect = {SCREEN_WIDTH/2 - (tempSurface1->w)/2, SCREEN_HEIGHT/8, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Death_Dead] = tempRect;
	TextTextures[Death_Dead] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);

	tempSurface1 = TTF_RenderText_Solid( bloodyFont, "PLAY AGAIN", deathColor );
	tempRect = {SCREEN_WIDTH/5, 3*SCREEN_HEIGHT/5, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Death_Play] = tempRect;
	TextTextures[Death_Play] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);
	
	tempSurface1 = TTF_RenderText_Solid( bloodyFont, "PLAY AGAIN", mainColor );
	tempRect = {SCREEN_WIDTH/5+5, 3*SCREEN_HEIGHT/5-5, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Death_Play_H] = tempRect;
	TextTextures[Death_Play_H] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);
	
	tempSurface1 = TTF_RenderText_Solid( bloodyFont, "Quit", deathColor );
	tempRect = {7*SCREEN_WIDTH/10, 3*SCREEN_HEIGHT/5, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Death_Quit] = tempRect;
	TextTextures[Death_Quit] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);

	tempSurface1 = TTF_RenderText_Solid( bloodyFont, "Quit", mainColor );
	tempRect = {7*SCREEN_WIDTH/10+5, 3*SCREEN_HEIGHT/5-5, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Death_Quit_H] = tempRect;
	TextTextures[Death_Quit_H] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);

	tempSurface1 = TTF_RenderText_Solid( bloodyFontS, "Return to main menu", deathColor );
	tempRect = {SCREEN_WIDTH/12, 9*SCREEN_HEIGHT/10, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Death_Return] = tempRect;
	TextTextures[Death_Return] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);

	tempSurface1 = TTF_RenderText_Solid( bloodyFontS, "Return to main menu", mainColor );
	tempRect = {SCREEN_WIDTH/12+5, 9*SCREEN_HEIGHT/10-5, tempSurface1->w, tempSurface1->h};
	TextTexture_R[Death_Return_H] = tempRect;
	TextTextures[Death_Return_H] = SDL_CreateTextureFromSurface(gRenderer, tempSurface1);
	SDL_FreeSurface(tempSurface1);
	tempSurface1 = NULL;
}

bool loadExplosion(){
	//load texture, put widths, load up the clips too
	bool success = true;
	SDL_Surface* loadSurface3 = NULL;
	std::string pathE = "images/explosion.png";
	loadSurface3 = IMG_Load(pathE.c_str());
	if (loadSurface3 == NULL){
		printf("load background error: %s\n", IMG_GetError() );
		success = false;
	}
	else{
		ExplosionWidth = loadSurface3->w;
		ExplosionHeight = loadSurface3->h;
		ExplosionWidth = ExplosionWidth/numExplosions+1;			// might cause roundoff error. check this if explosion looks funny

		ExplosionSpriteSheet = SDL_CreateTextureFromSurface(gRenderer, loadSurface3);
		if (ExplosionSpriteSheet == NULL){
			printf("explosion spritesheet texture error: %s\n", SDL_GetError() );
			success = false;
		}
		SDL_FreeSurface(loadSurface3);
		loadSurface3 = NULL;
	}
	SDL_Rect tempRect3 = {0,0,0,0};
	for (int m = 0; m < numExplosions; m++){		// loading source rectangles
		tempRect3 = {m*ExplosionWidth, 0, ExplosionWidth, ExplosionHeight};
		ExplosionClips[m] = tempRect3;
	}
	return success;
}

void renderAll(bool inferno, SDL_Rect backgroundRect){
	if (inferno){
		SDL_RenderCopy(gRenderer, InfernoBackground, NULL, &backgroundRect);
	}
	else{
		SDL_RenderCopy(gRenderer, Background, NULL, &backgroundRect);
	}

	for (std::vector<Asteroid>::iterator rock = Asteroids.begin(); rock != Asteroids.end(); ++rock){
		rock->render();
	}
	for (std::vector<Bullet>::iterator bullet = Fired.begin(); bullet != Fired.end(); ++bullet){
		bullet->render();
	}
}


void explosion(bool inferno, SDL_Rect backgroundRect){		// take care of the whole rendering loop for the explosion
	int slowDown = 4;	// if too fast, increase this
	SDL_Rect destRect = {0,0,0,0};
	SDL_Rect srcRect = {0,0,0,0};
	for (int frame = 0; frame < slowDown*numExplosions; frame++){
		destRect = {gShip.getX()-ExplosionWidth/2, gShip.getY()-ExplosionHeight/2, ExplosionHeight, ExplosionHeight};
		srcRect = ExplosionClips[frame/slowDown];
		SDL_RenderClear(gRenderer);
		renderAll(inferno,backgroundRect);
		SDL_RenderCopy(gRenderer, ExplosionSpriteSheet, &srcRect, &destRect);
		SDL_RenderPresent(gRenderer);
	}
}

void loadScore(int score){
	SDL_Surface* tempSurface2 = NULL;
	std::string scoreStr = "Score: " + std::to_string(score);
	tempSurface2 = TTF_RenderText_Solid( bloodyFont, scoreStr.c_str(), deathColor );
	SDL_Rect tempRect = {SCREEN_WIDTH/2 + (tempSurface2->w)/3, SCREEN_HEIGHT/3, tempSurface2->w, tempSurface2->h};
	TextTexture_R[Death_Score] = tempRect;
	TextTextures[Death_Score] = SDL_CreateTextureFromSurface(gRenderer, tempSurface2);
	SDL_FreeSurface(tempSurface2);
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

void resetGame(){
	// clear asteroids, bullets, put ship back at mid screen, set score = 0
	score = 0;
	gShip.resetPosition();
	MAX_N_ASTEROIDS = 8;
	N_ASTEROIDS = 0;
	AsteroidVelocityScale = 1;

	for (std::vector<Asteroid>::iterator rock = Asteroids.begin(); rock != Asteroids.end(); ){
		rock = Asteroids.erase(rock);
	}
	for (std::vector<Bullet>::iterator bullet = Fired.begin(); bullet != Fired.end(); ){
		bullet = Fired.erase(bullet);
	}
}


void close(){
	gShip.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	TTF_CloseFont(laserFontB);
	TTF_CloseFont(bloodyFontB);
	TTF_CloseFont(laserFont);
	TTF_CloseFont(bloodyFont);
	TTF_CloseFont(laserFontS);
	TTF_CloseFont(bloodyFontS);

	for (int d = 0; d < 17; d++){
		SDL_DestroyTexture(TextTextures[d]);
	}
	
	SDL_DestroyTexture(AsteroidTexture);
	SDL_DestroyTexture(BulletTexture);
	SDL_DestroyTexture(Background);	

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}
