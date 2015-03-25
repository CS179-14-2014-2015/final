#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#define PI 3.14159265

using namespace std;	//FINALLY!!! <3

//Screen dimension constants
const int SCOREBOARD_HEIGHT = 60;
const int PLAYFIELD_HEIGHT = 540;

const int SCREEN_WIDTH = 1170;
const int SCREEN_HEIGHT = SCOREBOARD_HEIGHT+PLAYFIELD_HEIGHT;

enum Tiles{
	GRASS, BRICK, WATER, STEEL, SLIDE_DOWN, SLIDE_LEFT, SLIDE_UP, SLIDE_RIGHT, TOTAL_TILES
};

enum Directions{
	SOUTH, WEST, NORTH, EAST, TOTAL_DIRECTIONS
};

enum Controls{
	UP, LEFT, DOWN, RIGHT, SHOOT, PLACEBOMB
};

enum PowerUps{
	LIFE, BOMB, SHIELD, BULLETUPGRADE, SPEEDUP
};

struct Circle{
		int x, y;
		int r;
};

class LTexture{
   		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mLength;

	public:
		//Initialize variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile(string);

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText(string, SDL_Color);
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor(Uint8, Uint8, Uint8);

		//Set blending
		void setBlendMode(SDL_BlendMode);

		//Set alpha modulation
		void setAlpha(Uint8 alpha);

		//Renders texture at given point
		void render(SDL_Rect*, SDL_Rect* = NULL, double = 0.0, SDL_Point* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);
		void render(int, int, SDL_Rect* = NULL, double = 0.0, SDL_Point* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);

		//Gets image dimensions
		int getWidth();
		int getLength();
};

class LTimer{
	    //The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

	public:
		//The timer status
		bool mPaused;
		bool mStarted;
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();
};

class Tile{
		int tileType;	//Used to determine the reaction of the tile
		int walkability;	//The level of mobility a player needs to have to traverse the tile
		int direction;	//Only really used for SLIDE tiles; determines the rotation of the tile/where the tile is facing
		vector<SDL_Rect> renderSrc;	//The area of the texture/s to be rendered
		
	public:
		static const int HEIGHT = 30, WIDTH = 30;
		
		Tile(int tileType):
			tileType(tileType), direction(0) {
				switch(tileType) {
					case GRASS:
						renderSrc.push_back({0, 0, 32, 32});
						walkability = 0;
						break;
					case BRICK:
						renderSrc.push_back({32, 0, 32, 32});
						walkability = 2;
						break;
					case WATER:
						renderSrc.push_back({64, 0, 32, 32});
						walkability = 1;
						break;
					case STEEL:
						renderSrc.push_back({96, 0, 32, 32});
						walkability = 3;
						break;
					case SLIDE_DOWN: case SLIDE_LEFT: case SLIDE_UP: case SLIDE_RIGHT:
						if(tileType == SLIDE_DOWN) {direction = SOUTH;}
						if(tileType == SLIDE_LEFT) {direction = WEST;}
						if(tileType == SLIDE_UP) {direction = NORTH;}
						if(tileType == SLIDE_RIGHT) {direction = EAST;}
						for(int i = 0; i < 4; ++i) {
							renderSrc.push_back({32*i, 32, 32, 32});
						}
						walkability = 0;
						break;
				}
			};
		
		int getTileType();
		int getWalkability();
		int getDirection();
		void render(int, SDL_Rect);
};

class Map{
		SDL_Rect t;
	
 	public:
		static const int ROWS = PLAYFIELD_HEIGHT/Tile::HEIGHT, COLS = SCREEN_WIDTH/Tile::WIDTH;
		
		Map();
		
		Tile* tileMap[COLS][ROWS];
		
		Tile* tile(int, int);
		void hit(int, int);
		void render(int);
};

class Bullet{
	public:
		static const int HEIGHT = 5, WIDTH = 5, VEL = 5;
		double x, y, w, h;

		int dir;
		SDL_Rect bullet;

		Bullet(double xStart, double yStart, int direction):
			x(xStart), y(yStart), w(WIDTH), h(HEIGHT), dir(direction) {};

		bool move();
		bool move(Uint32 t);
		void render();
		bool blanks();
};

class Player{    

	int dir;
	int lifeXPos;
	LTexture* playerTex;
	LTexture* playerLifeTex;
	Circle collider;
	SDL_Scancode con[6];
	bool wasPreviouslyOnSlidewalk;	//Used to check entry and exit of SLIDE tile
	SDL_Rect lastEnteredSlidewalk;
	
	void react();

	public:
		static const int WIDTH = 20, HEIGHT = 20;
		static const int SHIELD_DURATION = 10;
		static const int BOMB_DURATION = 50;
		SDL_Rect playerRect;

		int score = 0;
		int life = 5;
		int vel = 2;
        bool shieldEnable;
        bool bulletUpEnable;
    	bool bombEnable;
    	bool collisionReady;
        LTimer shieldTimer;
        LTimer bombTimer;

		Player(LTexture* texture, int lifeAvailableXPos, int x, int y, bool enableBombUp, bool enableBulletUp, bool enableShieldUp, SDL_Scancode up, SDL_Scancode left, SDL_Scancode down, SDL_Scancode right, SDL_Scancode shoot, SDL_Scancode placebomb):
			playerRect{x, y, texture->getWidth(), texture->getLength()},
			dir(SOUTH), playerTex(texture), collider{x+WIDTH/2,y+WIDTH/2,WIDTH/2}, bombEnable(enableBombUp), bulletUpEnable(enableBulletUp), shieldEnable(enableShieldUp), lifeXPos(lifeAvailableXPos), con{up, left, down, right, shoot, placebomb},
			wasPreviouslyOnSlidewalk(false), lastEnteredSlidewalk{0, 0, 0, 0} {};

		void act(const Uint8*);
		void act(SDL_Scancode);
		void move(int, int);

		Circle& getCollider();
		void shiftColliders();
		void render();
		void renderLifeTexture();
		void activatePowerUp(int id, SDL_Rect& Rect);
		void placeBomb();
		void shoot();

};

class PowerUp{
 	public:
 		static const int WIDTH = 20, HEIGHT = 20;
	 	int id;
		Circle collider;

		LTexture* powerUpTex;
		//Rect is changed to public
	    SDL_Rect powerUpRect;
		PowerUp(LTexture* texture, int pwrUp_id);

		void render();
		Circle& getCollider();
		int getPowerUpID();
};

class Bomb{
		Circle collider;
		
    public:
        static const int WIDTH = 20, HEIGHT = 20;
        static const int TIMER = 3;

        LTimer timer;
        int bombPosX, bombPosY;
        int scope;
        bool start;

        Bomb(int x, int y):
            bombPosX(x), bombPosY(y), collider{x+Player::WIDTH-(x+Player::WIDTH)%Tile::WIDTH+Tile::WIDTH/2, y+Player::HEIGHT-(y+Player::HEIGHT)%Tile::HEIGHT+Tile::HEIGHT/2, WIDTH/2}, scope(1), start(false) {timer.start();};
        Circle& getCollider();
		void shiftColliders();
        void render();
        void renderExplosion();
        void blowUp(int x, int y);
};

class Enemy{
		Circle collider;
		
	public:
		static const int WIDTH = 16, HEIGHT = 16;
		int posX, posY, angle, vx, vy, path;
		bool initDir;

		Enemy();
		Circle& getCollider();
		void shiftColliders();
		void shoot();
		void move(Uint32 t);
		//Flag for enemy - player collision
		bool collisionReady;
		void hit();
		void render();
};

class Score{
    public:
        string name;
        int score;
        Score(string playerName, int playerScore);
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Collision checkers
bool checkCollision(Circle& c1, Circle& c2);
bool checkCollision(Circle& c1, SDL_Rect r);
bool checkBombCollide(Circle& player);
bool checkEnemyCollide(Circle& player, int*, bool collisionReady);
bool checkIfEnclosed(SDL_Rect& smallerArea, SDL_Rect& largerArea);

//Gets grass tiles x and y pos and stores in x and y vectors
void getGrassTilesPos();

//Reset game
void restart();

//sorts score in text file
bool sortByScore(const Score &lhs,const Score &rhs);

//Frees media and shuts down SDL
void close();

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Globally used font
TTF_Font* gFont = NULL;

//Textures
LTexture gPlayerOneTexture;
LTexture gPlayerTwoTexture;
LTexture gEnemyTexture;

LTexture gBombPowerUPTexture;
LTexture gShieldTexture;
LTexture gLifeTexture;
LTexture gBulletUpgradeTexture;
LTexture gSpeedUpTexture;

LTexture gPlayer1ScoreTexture;
LTexture gPlayer2ScoreTexture;
LTexture gTimeTextTexture;
LTexture gMainTexture;
LTexture gPlayerOneWins;
LTexture gPlayerTwoWins;
LTexture gLifeAvailableTexture;

LTexture gBombTexture;
LTexture gExplosionTexture;
LTexture gPauseTexture;
LTexture gWinnerNameTexture;

LTexture gSpriteSheet;
Tile* gTiles[TOTAL_TILES];

//Timers
LTimer gTimer;
LTimer gDsplyPwrUpsTimer;

//Vectors
vector<Map> gLevels;
vector<Player> gPlayers;
vector<Bullet> gBullets;
vector<PowerUp> gPowerUps;
vector<Enemy> gEnemies;
vector<Bullet> gEnemyBullets;
vector<Bomb> gBomb;
vector<int> xTile;
vector<int> yTile;

//Rand device
random_device type;

//Initializes the current level
int gLevel = 0;	//Note: program crashes for some reason if set to 1
const int LEVELS = 3;	//Number of levels to be created (Now 3)

//For resetting the game
bool reset = false;

//For pausing the game
bool disableCon = false;

//frame counter
int frame = 0;

//For reading the map layout
ifstream mapReader("maps.txt");

//For the high score
ifstream myfile_Read("score.txt");
ofstream myfile;
string line;
string delim = ",";
string playerName = "";
string playerScore = "";
void recordScore();

//The current input text.
string winnerName = "Winner: ";

int numScore = 0;
int winnerScore = 0;

vector<string> data;
vector<string> names;
vector<string> strScore;
vector<int> intScore;
vector<Score> highScore;

int main(int argc, char *args[]) {
	//Start up SDL and create window
	if(!init()) {
		printf("Failed to initialize!\n");
	} else {
		//Load media
		if(!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else{
			int levelDuration = 60;

			//Main loop flags
			bool quit = false;
			bool start = false;
			bool paused = false;
			bool gameOver = false;

			//initial player values
			bool enableBombUp = false;
			bool enableBulletUp = false;
			bool enableShieldUp = false;
			int p1LifeAvailablePosX = 60;
			int p2LifeAvailablePosX = SCREEN_WIDTH-SCREEN_WIDTH/6;
			int p1_posX = 5, p1_posY = 5, p2_posX = SCREEN_WIDTH-Player::WIDTH-5, p2_posY = PLAYFIELD_HEIGHT-Player::HEIGHT-5;

			//Level initialization
			for(int i = 0; i < LEVELS; ++i) {
				gLevels.emplace_back();
			}

			getGrassTilesPos();

			//Create players
			gPlayers.emplace_back(&gPlayerOneTexture, p1LifeAvailablePosX, p1_posX, p1_posY, enableBombUp, enableBulletUp, enableShieldUp, SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_C, SDL_SCANCODE_X);
			gPlayers.emplace_back(&gPlayerTwoTexture, p2LifeAvailablePosX, p2_posX, p2_posY, enableBombUp, enableBulletUp, enableShieldUp, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_N, SDL_SCANCODE_M);

			//Power ups variables
			static const int NSETS = 8;
			static const int NPOWERUPS = 5;
			static const int DSPLYTIMEPWRUP = 10;

            //LIFE, BOMB, SHIELD, BULLETUPGRADE, SPEEDUP
			static const int powerUpsSet[NSETS][NPOWERUPS] = {{0, 2, 0, 1, 2}, {0, 2, 0, 0, 1}, {2, 0, 1, 0, 1}, {1, 0, 2, 0, 2}, {0, 0, 0, 2, 2}, {1, 3, 0, 1, 1}, {0, 0, 1, 1, 2}, {3, 0, 0,1, 2}};
			LTexture powerUpsTex[NPOWERUPS] = {gLifeTexture, gBombPowerUPTexture, gShieldTexture, gBulletUpgradeTexture, gSpeedUpTexture};

			int set = 0;
			bool nextSet = true;
			int powerUpsTime[NSETS] = {110, 95, 80, 70, 45, 30, 15, 7};
			int counter = 0;

			//Event handler
			SDL_Event event;
			const Uint8* state = SDL_GetKeyboardState(NULL);

			//Set text colour as black
			SDL_Color textColor = {255, 255, 255, 255};

			//In memory text stream
			stringstream timeText;
			stringstream player1Score;
			stringstream player2Score;

            //for inputing the text
            gWinnerNameTexture.loadFromRenderedText( winnerName.c_str(), textColor );

			//While application is running
			while(!quit) {
				//The rerender text flag
                bool renderText = false;

				while(SDL_PollEvent(&event)) {
                    //User requests quit
					if(event.type == SDL_QUIT) {
						quit = true;
					}
					//Pause/Unpause
					if(event.type == SDL_KEYDOWN) {
						if(event.key.keysym.sym == SDLK_RETURN) {
							gTimer.start();
							start = true;
						}
						if(event.key.keysym.sym == SDLK_p) {
							if(gTimer.isPaused()) {
								gTimer.unpause();
								paused = false;
							} else {
								gTimer.pause();
								paused = true;
							}
						}
						else if(event.key.repeat == 0) {
							for(int i = 0; i < gPlayers.size(); ++i) {
								gPlayers[i].act(event.key.keysym.scancode);
							}
						}
					}
				}
				
				for(int i = 0; i < gPlayers.size(); ++i) {gPlayers[i].act(state);}

				//Loads new set of Powerups when nextSet flag is set to true (time dependent)
				if(nextSet && set < NSETS) {
					for(int i = 0; i < NPOWERUPS; i++) {
						for(int j = 0; j < powerUpsSet[set][i]; j++) {
							gPowerUps.emplace_back(&powerUpsTex[i], i);
							gEnemies.emplace_back();
						}
					}
					nextSet = false;
				}
				if(!start) {
					SDL_RenderClear(gRenderer);
					gMainTexture.render(0,0);
				} else if(paused) {
					SDL_RenderClear(gRenderer);
					gPauseTexture.render(0,0);
					disableCon = true;
				} else if(gameOver) {
					gLevel = 0;
                    SDL_RenderClear(gRenderer);
                    bool inputPlayerOne = false;
                    //Enable text input
                    SDL_StartTextInput();
                     while(SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) {
                            quit = true;
					    }
                        if(event.type == SDL_TEXTINPUT) {
                            winnerName += event.text.text;
                            renderText = true;
                        }
                        if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
                            winnerName.erase(winnerName.length()-1);
                            renderText = true;
                        }
                    }
					
					if(gPlayers[0].score > gPlayers[1].score) {
						winnerScore = gPlayers[0].score;
						gPlayerOneWins.render(0,0);

						if( renderText==true ) {
                            //Text is not empty
                            if( winnerName != "" ) {
                                //Render new text
                                gWinnerNameTexture.loadFromRenderedText( winnerName.c_str(), textColor );
                            }
                        }
                        gWinnerNameTexture.render( 400, 400);

					} else if(gPlayers[1].score > gPlayers[0].score) {
						winnerScore = gPlayers[1].score;
						gPlayerTwoWins.render(0,0);

                        if( renderText==true ) {
                            //Text is not empty
                            if( winnerName != "" ) {
                                //Render new text
                                gWinnerNameTexture.loadFromRenderedText( winnerName.c_str(), textColor );
                            }
                        }
                        gWinnerNameTexture.render( 400, 400);
                    } else if(gPlayers[1].score == gPlayers[0].score) { //tie
                        SDL_RenderClear(gRenderer);
                        gameOver = false;
                        gTimer.start();
                        restart();
                    }
				} else if(reset) {
					SDL_RenderClear(gRenderer);
					if(gPlayers[0].life > gPlayers[1].life) {
						gPlayers[0].score++;
						restart();
					} else if(gPlayers[1].life > gPlayers[0].life) {
						gPlayers[1].score++;
						restart();
					}
				} else if(!reset) {
                    disableCon = false;
					//Viewports
					SDL_Rect scoreboard = {0, 0, SCREEN_WIDTH, SCOREBOARD_HEIGHT};
					SDL_Rect playfield = {0, SCOREBOARD_HEIGHT, SCREEN_WIDTH, PLAYFIELD_HEIGHT};

					SDL_RenderClear(gRenderer);
					SDL_RenderSetViewport(gRenderer, &scoreboard);

					SDL_SetRenderDrawColor(gRenderer, 0x00, 0x80, 0x80, 0xFF);
					SDL_RenderFillRect(gRenderer, &scoreboard);

					//Set text to be rendered
					timeText.str("");
					timeText << "Time: " << levelDuration - gTimer.getTicks()/1000;
                    player1Score.str("");
                    player1Score<<"Player 1: "<<gPlayers[0].score;
                    player2Score.str("");
                    player2Score<<"Player 2: "<<gPlayers[1].score;
					//Render text
					if(!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor)) {
						printf("Unable to render time texture!\n");
					}
					if(!gPlayer1ScoreTexture.loadFromRenderedText(player1Score.str().c_str(), textColor)) {
						printf("Unable to render player1score texture!\n");
					}
					if(!gPlayer2ScoreTexture.loadFromRenderedText(player2Score.str().c_str(), textColor)) {
						printf("Unable to render player2score texture!\n");
					}

					gTimeTextTexture.render((SCREEN_WIDTH-gTimeTextTexture.getWidth())/2, (SCOREBOARD_HEIGHT-gTimeTextTexture.getLength())/2);
					gPlayer1ScoreTexture.render((SCREEN_WIDTH-gPlayer1ScoreTexture.getWidth())/2-300, (SCOREBOARD_HEIGHT-gPlayer1ScoreTexture.getLength())/2);
					gPlayer2ScoreTexture.render((SCREEN_WIDTH-gPlayer2ScoreTexture.getWidth())/2+250, (SCOREBOARD_HEIGHT-gPlayer2ScoreTexture.getLength())/2);
					gPlayerOneTexture.render(30, 15);
					gPlayerTwoTexture.render(SCREEN_WIDTH-SCREEN_WIDTH/6-30, 15);

					for(int i = 0; i < gPlayers.size(); i++) {
						gPlayers[i].renderLifeTexture();
						if(gPlayers[i].life <= 0) {
							reset = true;
						}
					}
					
					if((levelDuration - gTimer.getTicks()/1000) == 0) {gameOver = true;}

					SDL_RenderSetViewport(gRenderer, &playfield);
					
					gLevels[gLevel].render(frame);

					for(int i = 0; i < gEnemyBullets.size(); ++i) {
                        if(gEnemyBullets[i].move(gTimer.getTicks())||gEnemyBullets[i].blanks()) {
                            gEnemyBullets[i].render();
                        } else {
                            gEnemyBullets.erase(gEnemyBullets.begin()+i);
                        }
                    }

					if((levelDuration - gTimer.getTicks()/1000) < powerUpsTime[set] && set < NSETS) {
						for(int i = 0; i < gPowerUps.size(); i++) {
							gPowerUps[i].render();
							gEnemies[i].move(gTimer.getTicks());
							gEnemies[i].render();
							if(!gDsplyPwrUpsTimer.isStarted()) {
								gDsplyPwrUpsTimer.start();
							}
							if(gDsplyPwrUpsTimer.getTicks()/1000 > DSPLYTIMEPWRUP) {
								gPowerUps.clear();
								gEnemies.clear();
								gDsplyPwrUpsTimer.stop();
								nextSet = true;
								set++;
								printf("Powerups cleared\n");
							}
						}
					}

					for(int i = 0; i < gBullets.size(); ++i) {
                        if(gBullets[i].move()||gBullets[i].blanks()) {
                            gBullets[i].render();
                        }
                        else {
                            gBullets.erase(gBullets.begin()+i);
                        }
                    }

                    for(int i = 0; i<gBomb.size(); i++) {
                        if(gBomb[i].timer.getTicks()/1000 > Bomb::TIMER) {
                            gBomb[i].blowUp(gBomb[i].bombPosX, gBomb[i].bombPosY);
                            gBomb[i].renderExplosion();
                            gBomb.erase(gBomb.begin()+i);
                        }
                        else {
                        	gBomb[i].render();
                        }
                    }

                    for(int i = 0; i < gPlayers.size(); i++) {
						gPlayers[i].render();
						if(gPlayers[i].shieldEnable == true && gPlayers[i].shieldTimer.getTicks()/1000>Player::SHIELD_DURATION) {
                            gPlayers[i].shieldEnable = false;
                            gPlayers[i].shieldTimer.stop();
                        }
                        if(gPlayers[i].bombEnable == true && gPlayers[i].bombTimer.getTicks()/1000>Player::BOMB_DURATION) {
                            gPlayers[i].bombEnable = false;
                            gPlayers[i].bombTimer.stop();
                        }
						for(int j = 0; j < gPowerUps.size(); j++) {
							if(checkCollision(gPlayers[i].getCollider(), gPowerUps[j].getCollider())) {
                                gPlayers[i].activatePowerUp(gPowerUps[j].getPowerUpID(),gPowerUps[j].powerUpRect);
                                gPowerUps.erase(gPowerUps.begin()+j);
						    }
						}
					}

                    if(gPlayers[0].shieldEnable == true) {gPlayerOneTexture.loadFromFile("Assets/p1_shield.png");}
                    else {gPlayerOneTexture.loadFromFile("Assets/p1.png");}

                    if(gPlayers[1].shieldEnable == true) {gPlayerTwoTexture.loadFromFile("Assets/p2_shield.png");}
                    else {gPlayerTwoTexture.loadFromFile("Assets/p2.png");}
				}
				SDL_RenderPresent(gRenderer);
				
				++frame;
			}
		}
    }
	close();
	SDL_StopTextInput();
    recordScore();
    return 0;
}

LTexture::LTexture() {
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mLength = 0;
}

LTexture::~LTexture() {
	//Deallocate
	free();
}

bool LTexture::loadFromFile(string path) {
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if(loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

		if(newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			//Get image dimensions
			mWidth = loadedSurface->w;
			mLength = loadedSurface->h;
		}

		//Get rid of loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(string textureText, SDL_Color textColor) {
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if(textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	} else {
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(mTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		} else {
			//Get image dimensions
			mWidth = textSurface->w;
			mLength = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return mTexture != NULL;
}
#endif

void LTexture::free() {
	//Free texture if it exists
	if(mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mLength = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Rect* dst, SDL_Rect* src, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//Set clip rendering dimensions
	if(dst->w == 0 || dst->h == 0) {
		dst->w = mWidth;
		dst->h = mLength;
	}
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, src, dst, angle, center, flip);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mLength};

	//Set clip rendering dimensions
	if(clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getLength() {
	return mLength;
}

LTimer::LTimer() {
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;
	mPaused = false;
	mStarted = false;
}

void LTimer::start() {
	//Start the timer
	mStarted = true;
	//Unpause the timer
	mPaused = false;
	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop() {
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause() {
	//If the timer is running and isn't already paused
	if(mStarted && !mPaused) {
		//Pause the timer
		mPaused = true;
		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks()-mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause() {
	//If the timer is running and paused
	if(mStarted && mPaused) {
		//Unpause the timer
		mPaused = false;

		//Reset the startng ticks
		mStartTicks = SDL_GetTicks()-mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks() {
	//The actual timer time
	Uint32 time = 0;
	//If the timer is running
	if(mStarted) {
		//If the timer is paused
		if(mPaused) {
			//Return the number of ticks when the timer is paused
			time = mPausedTicks;
		} else {
			//Return the current time minus the start time
			time = SDL_GetTicks()-mStartTicks;
		}
	}
	return time;
}

bool LTimer::isStarted() {
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused() {
	//Timer is running and paused
	return mPaused && mStarted;
}

int Tile::getTileType() {
	return tileType;
}

int Tile::getWalkability() {
	return walkability;
}

int Tile::getDirection() {
	return direction;
}

void Tile::render(int frame, SDL_Rect renderDst) {
	gSpriteSheet.render(&renderDst, &renderSrc[frame/2%renderSrc.size()], direction*90);
}

Map::Map() {
	if(mapReader.is_open()) {
		string mapName;
		
		if(!mapReader.eof()) {
			while(mapReader.get() != '[') {}
			getline(mapReader, mapName, ']');
		}
		
		for(int i = 0; i < Map::ROWS; ++i) {
			for(int j = 0; j < Map::COLS; ++j) {
				int tileType;
				if(!mapReader.eof()) {
					mapReader >> tileType;
					if(tileType < 0 || tileType >= TOTAL_TILES) {tileType = 0;}
				} else {
					tileType = 0;
				}
				tileMap[j][i] = gTiles[tileType];
			}
		}
	}
	
	t.w = Tile::WIDTH;
	t.h = Tile::HEIGHT;
}

Tile* Map::tile(int x, int y) {
	if(x >= 0 && x < Tile::WIDTH*Map::COLS
		&& y >= 0 && y < Tile::HEIGHT*Map::ROWS) {
		return tileMap[x/Tile::WIDTH][y/Tile::HEIGHT];
	} else {
		return gTiles[STEEL];
	}
}

void Map::hit(int x, int y) {
	if(tile(x, y) == gTiles[BRICK]) {
		tileMap[x/Tile::WIDTH][y/Tile::HEIGHT] = gTiles[GRASS];
	}
}

void Map::render(int frame) {
	for(int i = 0; i < ROWS; ++i) {
		for(int j = 0; j < COLS; ++j) {
			t.x = j*Tile::WIDTH;
			t.y = i*Tile::HEIGHT;
			
			tileMap[j][i]->render(frame, t);
		}
	}
}

void Player::act(const Uint8* state) {
    if(!disableCon) {
		if(state[con[UP]]) {move(0, -vel); dir = NORTH;}
		if(state[con[LEFT]]) {move(-vel, 0); dir = WEST;}
		if(state[con[DOWN]]) {move(0, vel); dir = SOUTH;}
		if(state[con[RIGHT]]) {move(vel, 0); dir = EAST;}
	}

	react();
}

void Player::act(SDL_Scancode key) {
	if(!disableCon) {
		if(key == con[SHOOT]) {shoot();}
		if(key == con[PLACEBOMB] && bombEnable) {placeBomb();}
	}
}

void Player::react() {
	SDL_Rect tileBoxOfOrigin = {playerRect.x-playerRect.x%Tile::WIDTH, playerRect.y-playerRect.y%Tile::HEIGHT, Tile::WIDTH, Tile::HEIGHT};
	
	if(gLevels[gLevel].tile(playerRect.x, playerRect.y)->getTileType() >= SLIDE_DOWN
	&& gLevels[gLevel].tile(playerRect.x, playerRect.y)->getTileType() <= SLIDE_RIGHT
	&& (checkIfEnclosed(playerRect, tileBoxOfOrigin) || wasPreviouslyOnSlidewalk)) {
		if(checkIfEnclosed(playerRect, tileBoxOfOrigin)) {
			lastEnteredSlidewalk = tileBoxOfOrigin;
		}
		dir = (gLevels[gLevel].tile(lastEnteredSlidewalk.x, lastEnteredSlidewalk.y)->getDirection()+1)%4;
		wasPreviouslyOnSlidewalk = true;
		
		switch(dir) {
			case SOUTH: move(0, vel); break;
			case WEST: move(-vel, 0); break;
			case NORTH: move(0, -vel); break;
			case EAST: move(vel, 0); break;
		}
	} else if(checkIfEnclosed(playerRect, tileBoxOfOrigin)) {
		wasPreviouslyOnSlidewalk = false;
	}
}

void Player::move(int vx, int vy) {
    playerRect.x += vx;//whatever the position of the rectangle is will also be the position where the rectangel will be rendered
    shiftColliders();

    if((gLevels[gLevel].tile(playerRect.x, playerRect.y))->getWalkability() > 0
	|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y))->getWalkability() > 0
	|| (gLevels[gLevel].tile(playerRect.x, playerRect.y+HEIGHT))->getWalkability() > 0
	|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y+HEIGHT))->getWalkability() > 0
	|| (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider())) 
	|| (checkBombCollide(collider))
	|| (checkEnemyCollide(collider, &life, collisionReady))
	) {
		playerRect.x -= vx;
		shiftColliders();
    }
    
	playerRect.y += vy;//whatever the position of the rectangle is will also be the position where the rectangel will be rendered
    shiftColliders();

    if((gLevels[gLevel].tile(playerRect.x, playerRect.y))->getWalkability() > 0
	|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y))->getWalkability() > 0
	|| (gLevels[gLevel].tile(playerRect.x, playerRect.y+HEIGHT))->getWalkability() > 0
	|| (gLevels[gLevel].tile(playerRect.x+WIDTH, playerRect.y+HEIGHT))->getWalkability() > 0
	|| (checkCollision(gPlayers[0].getCollider(), gPlayers[1].getCollider())) 
	|| (checkBombCollide(collider))
	|| (checkEnemyCollide(collider, &life, collisionReady))
	) {
		playerRect.y -= vy;
		shiftColliders();
    }
}

Circle& Player::getCollider() {
	return collider;
}

void Player::shiftColliders() {
	collider.x = playerRect.x+collider.r;
	collider.y = playerRect.y+collider.r;
}

void Player::shoot() {
   if(bulletUpEnable == false) {
        if(dir == EAST) gBullets.emplace_back((playerRect.x+WIDTH), (playerRect.y+HEIGHT/2), dir);
        else if(dir == WEST) gBullets.emplace_back((playerRect.x-WIDTH/4), (playerRect.y+HEIGHT/2), dir);
        else if(dir == SOUTH) gBullets.emplace_back((playerRect.x+WIDTH/2), (playerRect.y+HEIGHT), dir);
        else if(dir == NORTH) gBullets.emplace_back((playerRect.x+WIDTH/2), (playerRect.y-HEIGHT/4), dir);
    }
    else if(bulletUpEnable == true) {
        gBullets.emplace_back((playerRect.x+WIDTH), (playerRect.y+HEIGHT/2), EAST);
        gBullets.emplace_back((playerRect.x-WIDTH), (playerRect.y+HEIGHT/2), WEST);
        gBullets.emplace_back((playerRect.x+WIDTH/2), (playerRect.y+HEIGHT), SOUTH);
        gBullets.emplace_back((playerRect.x+WIDTH/2), (playerRect.y-HEIGHT), NORTH);
    }
}

void Player::placeBomb() {
    gBomb.emplace_back(playerRect.x, playerRect.y);
}

void Player::render() {
     playerTex->render(&playerRect, NULL, 90*dir);
}

void Player::renderLifeTexture() {
	for(int i = 0; i < life; i++) {
		gLifeAvailableTexture.render(lifeXPos+(gLifeAvailableTexture.getWidth()*i), 15);
	}
}

void Player::activatePowerUp(int id, SDL_Rect& Rect) {
	switch(id) {
		case LIFE:
			printf("life\n");
			if(life <= 7)life++;
			break;
		case BOMB:
            printf("bomb\n");
            bombTimer.start();
            bombEnable = true;
            break;
		case SHIELD:
			printf("shield\n");
			shieldTimer.start();
			shieldEnable = true;
			break;
		case BULLETUPGRADE:
			printf("bullet upgraded\n");
            bulletUpEnable = true;
			break;
		case SPEEDUP:
			printf("speedup\n");
            vel++;
			break;
	}
}

PowerUp::PowerUp(LTexture* texture, int pwrUp_id) {
	//gets a random position to place the powerup
	int randInd = type()%xTile.size();
	powerUpRect = {xTile[randInd]+Tile::WIDTH/5, yTile[randInd]+Tile::HEIGHT/5, texture->getWidth(), texture->getLength()};
	powerUpTex = texture;
	id = pwrUp_id;
	collider = {xTile[randInd]+Tile::WIDTH/5+WIDTH/2, yTile[randInd]+Tile::HEIGHT/5+WIDTH/2, WIDTH/2};
}

void PowerUp::render() {
	powerUpTex->render(&powerUpRect);
}

int PowerUp::getPowerUpID() {
	return id;
}

Circle& PowerUp::getCollider() {
    return collider;
}

bool Bullet::move() {
	x += VEL*cos(PI*(dir+1)/2);
	y += VEL*sin(PI*(dir+1)/2);

	SDL_Rect bullet{(int) x, (int) y, WIDTH, HEIGHT};

    if(gLevels[gLevel].tile(x, y) == gTiles[BRICK]) {
        gLevels[gLevel].hit(x, y);
        return false;
    }
    else if(gLevels[gLevel].tile(x+WIDTH, y+HEIGHT) == gTiles[BRICK]) {
        gLevels[gLevel].hit(x+WIDTH, y+HEIGHT);
        return false;
    }
    else if(gLevels[gLevel].tile(x, y) == gTiles[STEEL]) {
        return false;
    }
    else if(checkCollision(gPlayers[0].getCollider(), bullet)) {
        gPlayers[0].life--;
        return false;
    }
    else if(checkCollision(gPlayers[1].getCollider(), bullet)) {
        gPlayers[1].life--;
        return false;
    }
	return true;
}

bool Bullet::move(Uint32 t) {
	x += VEL*cos(dir%4*PI/2);
	y += VEL*sin(dir%4*PI/2);
	SDL_Rect bullet{(int) x, (int) y, WIDTH, HEIGHT};
	if(checkCollision(gPlayers[0].getCollider(), bullet)) {
        gPlayers[0].life--;
        return false;
    }
    if(checkCollision(gPlayers[1].getCollider(), bullet)) {
        gPlayers[1].life--;
        return false;
    }
    return true;
}

void Bullet::render() {
	SDL_Rect bullet{(int) x, (int) y, WIDTH, HEIGHT};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &bullet);
}

bool Bullet::blanks() {
    SDL_Rect bullet{(int) x, (int) y, WIDTH, HEIGHT};
    if(checkCollision(gPlayers[0].getCollider(), bullet)&&gPlayers[0].shieldEnable == true) {
        gPlayers[0].life++;
        return false;
    }
    if(checkCollision(gPlayers[1].getCollider(), bullet)&&gPlayers[1].shieldEnable == true) {
        gPlayers[1].life++;
        return false;
    }
}

Circle& Bomb::getCollider() {
	return collider;
}

void Bomb::render() {
    gBombTexture.render(bombPosX+Player::WIDTH-(bombPosX+Player::WIDTH)%Tile::WIDTH+Tile::WIDTH/2-gBombTexture.getWidth()/2,
        bombPosY+Player::HEIGHT-(bombPosY+Player::HEIGHT)%Tile::HEIGHT+Tile::HEIGHT/2-gBombTexture.getLength()/2);
}

void Bomb::renderExplosion() {
    gExplosionTexture.render(bombPosX+Player::WIDTH-(bombPosX+Player::WIDTH)%Tile::WIDTH+Tile::WIDTH/2-gExplosionTexture.getWidth()/2,
        bombPosY+Player::HEIGHT-(bombPosY+Player::HEIGHT)%Tile::HEIGHT+Tile::HEIGHT/2-gExplosionTexture.getLength()/2);
}

void Bomb::blowUp(int x, int y) {
    for(int i = 1; i<=scope; i++) {
        if(gLevels[gLevel].tile((x+Tile::WIDTH*i), y) == gTiles[BRICK]) {
            gLevels[gLevel].tileMap[(x+Tile::WIDTH*i)/Tile::WIDTH][y/Tile::HEIGHT]=gTiles[GRASS];//right
        }
        if(gLevels[gLevel].tile((x-Tile::WIDTH*i), y) == gTiles[BRICK]) {
            gLevels[gLevel].tileMap[(x-Tile::WIDTH*i)/Tile::WIDTH][y/Tile::HEIGHT]=gTiles[GRASS];//left
        }
        if(gLevels[gLevel].tile((x), (y-Tile::HEIGHT*i)) == gTiles[BRICK]) {
            gLevels[gLevel].tileMap[(x)/Tile::WIDTH][(y-Tile::HEIGHT*i)/Tile::HEIGHT]=gTiles[GRASS];//up
        }
        if(gLevels[gLevel].tile((x), (y+Tile::HEIGHT*i)) == gTiles[BRICK]) {
            gLevels[gLevel].tileMap[(x)/Tile::WIDTH][(y+Tile::HEIGHT*i)/Tile::HEIGHT]=gTiles[GRASS];//down
        }
    }
    for(int i = 0; i < gPlayers.size(); i++) {
    	if( (gPlayers[i].playerRect.x < x && gPlayers[i].playerRect.x > x-Tile::WIDTH*scope && (gPlayers[i].playerRect.y > y || gPlayers[i].playerRect.y < y+Tile::HEIGHT*scope)) //player on the left
    		|| (gPlayers[i].playerRect.x > x + Tile::WIDTH && gPlayers[i].playerRect.x < x+Tile::WIDTH + (Tile::WIDTH*scope) && (gPlayers[i].playerRect.y > y || gPlayers[i].playerRect.y < y+Tile::HEIGHT*scope)) //player on the right
    		|| (gPlayers[i].playerRect.y < y && gPlayers[i].playerRect.y > y-Tile::WIDTH*scope && (gPlayers[i].playerRect.x > x || gPlayers[i].playerRect.x < x+Tile::WIDTH*scope))//player on top
    		|| (gPlayers[i].playerRect.y > y + Tile::HEIGHT && gPlayers[i].playerRect.y < y+Tile::HEIGHT+(Tile::HEIGHT*scope) && (gPlayers[i].playerRect.x > x || gPlayers[i].playerRect.x < x+Tile::WIDTH*scope))//player on bottom
    		|| checkBombCollide(gPlayers[i].getCollider())//player on bomb
    		) {
    		gPlayers[i].life--;
    	}
    }
}

Enemy::Enemy() {
	int randInd = type()%xTile.size();
	posX = xTile[randInd] + WIDTH/2;
	posY = yTile[randInd] + HEIGHT/2;

	//random path
	path = type()%3;
	if(type()%2 == 1){initDir = true;}
	else{initDir = false;}

	//random velocities
	vx = type()%2-1;
	vy = type()%2-1;
	if(vx==0||vy==0) {vx = 1; vy = 1;}

	angle = 0;
	collider = {posX, posY, WIDTH/2};
	collisionReady = true;
}

void Enemy::shoot() {
	gEnemyBullets.emplace_back(posX+WIDTH/2-1, posY+HEIGHT/2-1, angle);
}

void Enemy::move(Uint32 t) {
	switch(path) {
		//Linear movement
		case 0:
			posX+=vx;
			if(posX < 0 || posX+WIDTH > SCREEN_WIDTH ) {vx = -1*vx;}
			posY+=vy;
			if(posY < 0 || posY+HEIGHT > SCREEN_HEIGHT-SCOREBOARD_HEIGHT) {vy = -1*vy;}
			shiftColliders();
			break;
		//Circular
/*		case 1:
			posX += 5*cos(PI*t/1000);
			posY += 5*sin(PI*t/1000);
			shiftColliders();
			break;*/
		//Searcher
		case 1:
			if(initDir){posX += vx;}
			else{posX -= vx;}
			if(gLevels[gLevel].tile(posX+WIDTH, posY)->getWalkability() > 0) {posY+=vy;}
			else {posX += vx;}
			if(gLevels[gLevel].tile(posX+WIDTH, posY+HEIGHT)->getWalkability() > 0) {posX-=vx;}
			else {posY+=vy;}
			if(gLevels[gLevel].tile(posX, posY+HEIGHT)->getWalkability() > 0) {posY-=vy;}
			else {posX-=vx;}
			if(gLevels[gLevel].tile(posX, posY)->getWalkability() > 0) {posX+=vx;}
			else{posY-=vy;}
			if(posX==0||posX+WIDTH==SCREEN_WIDTH){vx=-1*vx;};
			if(posY==0||posY+HEIGHT==PLAYFIELD_HEIGHT){vy=-1*vy;};

			shiftColliders();
			break;
		//Static
		case 2:
			if(frame % 10 ==0) {shoot();}
	}
	hit();
}

void Enemy::shiftColliders() {
	collider.x = posX+collider.r;
	collider.y = posY+collider.r;
}

Circle& Enemy::getCollider() {
	return collider;
}

void Enemy::hit() {
	for(int i = 0; i < gPlayers.size(); i++) {
		if(checkCollision(gPlayers[i].getCollider(), collider)) {
			if(collisionReady){
				if(!gPlayers[i].shieldEnable) {gPlayers[i].life--;}
				vx = -1*vx;
				vy = -1*vy;
			}
			collisionReady = false;	
		}
		else {collisionReady = true;}
	}
}

void Enemy::render() {
	angle++;
	gEnemyTexture.render(posX, posY, NULL, angle);
}

Score::Score(string playerName, int playerScore) {
    name = playerName;
    score = playerScore;
}

bool init() {
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}
		//Create window
		gWindow = SDL_CreateWindow("Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				 //Initialize SDL_ttf
				if(TTF_Init() == -1) {
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	//Loading success flag
	bool success = true;

	if(!gMainTexture.loadFromFile("Assets/main.png") ) {
		printf( "Unable to render main text texture!\n" );
		success = false;
	}
	if(!gPauseTexture.loadFromFile("Assets/paused.png")) {
		printf( "Unable to render pause text texture!\n" );
		success = false;
	}
	if(!gPlayerOneWins.loadFromFile("Assets/p1_wins.png")) {
		printf( "Unable to render P1 wins texture!\n" );
		success = false;
	}
	if(!gPlayerTwoWins.loadFromFile("Assets/p2_wins.png")) {
		printf( "Unable to render P2 wins texture!\n" );
		success = false;
	}

	//load font
	gFont = TTF_OpenFont("Assets/ostrich.ttf", 50);
	SDL_Color textColor = {0xD0, 0xD0, 0xD0, 0xFF};
	if(gFont == NULL) {
		printf("Failed to load ostrich font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	
	//Load sprites
	if(!gSpriteSheet.loadFromFile("Assets/terrain.png")) {
		printf("Failed to load terrain sprite sheet!\n");
		success = false;
	} else {
		for(int i = 0; i < TOTAL_TILES; ++i) {
			gTiles[i] = new Tile(i);
		}
	}
	
    //Load power up textures
	if(!gBombPowerUPTexture.loadFromFile("Assets/bomb.png")) {
		printf("Failed to load bomb texture!\n");
		success = false;
	}
	if(!gBombTexture.loadFromFile("Assets/bomb.gif")) {
		printf("Failed to load bomb texture!\n");
		success = false;
	}
	if(!gExplosionTexture.loadFromFile("Assets/explosion.png")) {
		printf("Failed to load bomb texture!\n");
		success = false;
	}
	if(!gShieldTexture.loadFromFile("Assets/shield.png")) {
		printf("Failed to load shield texture!\n");
		success = false;
	}
	if(!gLifeTexture.loadFromFile("Assets/life.png")) {
		printf("Failed to load life texture!\n");
		success = false;
	}
	if(!gBulletUpgradeTexture.loadFromFile("Assets/bulletUp.png")) {
		printf("Failed to load bulletUpgrade texture!\n");
		success = false;
	}
	if(!gSpeedUpTexture.loadFromFile("Assets/speed.png")) {
		printf("Failed to load speed texture!\n");
		success = false;
	}
	//Load players' life available image
	if(!gLifeAvailableTexture.loadFromFile("Assets/lifeAvailable.png")) {
		printf("Failed to load life available texture!\n");
		success = false;
	}
	//load enemy
	if(!gEnemyTexture.loadFromFile("Assets/enemy.png")) {
		printf("Failed to load enemy texture!\n");
		success = false;
	}
	return success;
}

bool checkBombCollide(Circle& player) {
	for(int i = 0; i < gBomb.size(); i++) {
        if(!checkCollision(player, gBomb[i].getCollider())) {
        	gBomb[i].start = true;
        }
        if(checkCollision(player, gBomb[i].getCollider()) && gBomb[i].start) {
        	return true;
        }
    }
    return false;
}

bool checkEnemyCollide(Circle& player, int* life, bool collisionReady) {
	for(int i = 0; i < gEnemies.size(); i++) {
        if(checkCollision(player, gEnemies[i].getCollider())) {
        	if(collisionReady) {*life = *life-1;}
        	collisionReady = false;
        	return true;
        }
        else {collisionReady = true;}
    }
    return false;
}

bool checkCollision(Circle& c1, Circle& c2) {
	if(sqrt(pow(c1.x-c2.x, 2)+pow(c1.y-c2.y, 2)) < c1.r+c2.r) {
		return true;
	}
	return false;
}

bool checkCollision(Circle& c1, SDL_Rect r) {
	//Closest point on collision box
    int cX, cY;
    if(c1.x < r.x) {cX = r.x;}
    else if(c1.x > r.x + r.w) {cX = r.x + r.w;}
    else{cX = c1.x;}
    //Find closest y offset
    if(c1.y < r.y) {cY = r.y;}
    else if(c1.y > r.y + r.h) {cY = r.y + r.h;}
    else{cY = c1.y;}

    //If the closest point is inside the circle
    if(sqrt(pow(cX-c1.x, 2)+pow(cY-c1.y, 2)) < c1.r) {
        return true;
    }
    return false;
}

bool checkIfEnclosed(SDL_Rect& smallerArea, SDL_Rect& largerArea) {
	if(smallerArea.x >= largerArea.x && smallerArea.x+smallerArea.w <= largerArea.x+largerArea.w
	&& smallerArea.y >= largerArea.y && smallerArea.y+smallerArea.h <= largerArea.y+largerArea.h) {
		return true;
	} else {
		return false;
	}
}

void getGrassTilesPos() {
	for(int i = Tile::WIDTH*4; i < SCREEN_WIDTH - Tile::WIDTH*4; i+=Tile::WIDTH) {
		for(int j = Tile::WIDTH*4; j < PLAYFIELD_HEIGHT-Tile::WIDTH*4; j+=Tile::HEIGHT) {
			if(gLevels[gLevel].tile(i,j) == gTiles[GRASS]) {
				xTile.push_back(i);
				yTile.push_back(j);
			}
		}
	}
}

bool sortByScore(const Score &lhs,const Score &rhs) {
    return lhs.score>rhs.score;
}

void recordScore() {
    /***for the scoring***/
    //get data from file push to data vector
    if (myfile_Read.is_open()) {
        while ( getline (myfile_Read,line) ) {
          data.push_back(line);
        }
        myfile_Read.close();
    }
    else cout << "Unable to open file";
    //get the name and score separated by a coma
    for(int i = 0; i<data.size(); i++) {
        playerName = data[i].substr(0,data[i].find(delim));
        names.push_back(playerName);

        playerScore = data[i].substr(data[i].find(delim)+1,data[i].find("\n"));
        strScore.push_back(playerScore);
    }
    //since score is still a string needs to converted to a int
    for(int i = 0; i<strScore.size();i++) {
        numScore = atoi(strScore[i].c_str());
        intScore.push_back(numScore);
    }
    for(int i = 0; i<winnerName.length(); i++) {
        winnerName[i]= toupper(winnerName[i]);
    }
    names.push_back(winnerName);
    intScore.push_back(winnerScore);

    //load Scores to highScore vector
    for(int i = 0; i<data.size()+1;i++) {
        highScore.emplace_back(names[i],intScore[i]);
    }
    //sort who is the highest
    sort(highScore.begin(),highScore.end(),sortByScore);
    myfile.open ("score.txt");
    //write to text file the new set of scores
    for(int i = 0; i<data.size()+1;i++) {
        myfile<<highScore[i].name+",";
        myfile<<highScore[i].score;
        myfile<<"\n";
    }
    myfile.close();
}

void restart() {
    SDL_RenderClear(gRenderer);
    gPlayers[0].playerRect.x = 5;
    gPlayers[0].playerRect.y = 5;
    gPlayers[0].shiftColliders();
    gPlayers[1].playerRect.x = SCREEN_WIDTH-Player::WIDTH-5;
    gPlayers[1].playerRect.y = PLAYFIELD_HEIGHT-Player::HEIGHT-5;
    gPlayers[1].shiftColliders();
    for(auto &Bullet: gBullets) {
        gBullets.erase(gBullets.begin(),gBullets.end());
    }
    for(int i = 0; i < gPlayers.size(); i++) {
        gPlayers[i].renderLifeTexture();
        gPlayers[i].life = 5;
    }
	gLevel = (gLevel+1)%LEVELS;
    reset = false;
}

void close() {
	//Free loaded images
	gPauseTexture.free();
	gMainTexture.free();
	gTimeTextTexture.free();
	gLifeAvailableTexture.free();
	gPlayerOneWins.free();
	gPlayerTwoWins.free();

	gBombPowerUPTexture.free();
	gShieldTexture.free();
	gLifeTexture.free();
	gBulletUpgradeTexture.free();
	gSpeedUpTexture.free();

	gBombTexture.free();
	gExplosionTexture.free();

	gPlayerOneTexture.free();
	gPlayerTwoTexture.free();
	gEnemyTexture.free();

	gSpriteSheet.free();
	mapReader.close();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
