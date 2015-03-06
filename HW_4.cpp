/*This source code copyrighted by Lazy Foo' Productions (2004-2014)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <array>
#include "SDL2_gfxPrimitives.h"
#include <math.h>
//#include <SDL_image.h>
//Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 650;
int bossrad = 225;
const double PI = 3.1415926535897932384626433832795;
const int Bullet_RADIUS = 10;
int bulletCount = 0;
int bulletCount2 = 0;
int alienCount = 0;
SDL_Renderer* renderer = nullptr;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//The Bullet that will move around on the screen
class Bullet;
//The FIGHTER that will move around on the screen
class FIGHTER
{
    public:
		//The dimensions of the FIGHTER
		static const int FIGHTER_WIDTH = 50;
		static const int FIGHTER_HEIGHT = 50;

		//Maximum axis velocity of the FIGHTER
		static const int FIGHTER_VEL = 3;
		int hp = 10;
		//Initializes the variables
		FIGHTER();

		//Takes key presses and adjusts the FIGHTER's velocity
		void handleEvent( SDL_Event& e );

		//Moves the FIGHTER and checks collision
		void move();

		//Shows the FIGHTER on the screen
		void render();

		int yPos();

		int xPos();

		int gotHit();

		int getHp();
    private:
		//The X and Y offsets of the FIGHTER
		int mPosX, mPosY;

		//The velocity of the FIGHTER
		int mVelX, mVelY;

		//FIGHTER's collision box
		SDL_Rect mCollider;

		long shootTimer;

		long lastHit;
};

class Bullet
{
    public:

		//The X and Y offsets of the FIGHTER
		int mPosX, mPosY;



		//Initializes the variables
		Bullet(int x, int y, int speed, float angle, bool rotate, bool east);


		//Moves the Bullet and checks collision
		void move();

		//Shows the FIGHTER on the screen
		void render();

		int xPos();

		int yPos();


    private:

		bool changeDir;

		int ms;

		float dir;

		long changePat;
		//FIGHTER's collision box

		int changed = 0;

		bool rot;
};

class ALIEN
{
    public:
		//The dimensions of the FIGHTER
		static const int ALIEN_WIDTH = 50;
		static const int ALIEN_HEIGHT = 50;

		//Maximum axis velocity of the FIGHTER
		static const int ALIEN_VEL = 2;


		//Initializes the variables
		ALIEN(int x, int y, int type);

		//Moves the FIGHTER and checks collision
		void move(int xPos, int yPos);

		//Shows the FIGHTER on the screen
		void render();

		int xPos();

		int yPos();

		int gotHit();

    private:
		//The X and Y offsets of the FIGHTER
		int mPosX, mPosY;

		//The velocity of the FIGHTER
		int mVelX, mVelY;

		//FIGHTER's collision box
		SDL_Rect mCollider;

		long shootTimer;

		int aType;

		int hp = 5;

		bool right;


};

class BOSS
{
    public:
		//The dimensions of the FIGHTER
		static const int ALIEN_WIDTH = 125;
		static const int ALIEN_HEIGHT = 125;

		//Maximum axis velocity of the FIGHTER
		static const int ALIEN_VEL = 0;


		//Initializes the variables
		BOSS(int x, int y);

		//Moves the FIGHTER and checks collision
		void move();

		//Shows the FIGHTER on the screen
		void render();

		int xPos();

		int yPos();

		int gotHit();

		int getHp();

    private:
		//The X and Y offsets of the FIGHTER
		int mPosX, mPosY;

		//The velocity of the FIGHTER
		int mVelX, mVelY;

		//FIGHTER's collision box
		SDL_Rect mCollider;

		long shootTimer;

		bool right = false;

		int hp = 40;

		long changeTimer;

		int pattern;
		long menaceTime;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Box collision detector
//void checkCollision( Bullet Bul[] );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gFIGHTERTexture;
LTexture gALIENTexture;
LTexture gBOSSTexture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	//SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}


FIGHTER::FIGHTER()
{
    //Initialize the offsets
    mPosX = SCREEN_WIDTH/2;
    mPosY = SCREEN_HEIGHT*3/4;

	//Set collision box dimension
	mCollider.w = gFIGHTERTexture.getWidth();
	mCollider.h = gFIGHTERTexture.getHeight();

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    shootTimer = SDL_GetTicks();

    lastHit = SDL_GetTicks();
}

ALIEN::ALIEN(int x, int y, int type)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

	//Set collision box dimension
	mCollider.w = gALIENTexture.getWidth();
	mCollider.h = gALIENTexture.getHeight();

    //Initialize the velocity
    mVelX = 1;
    mVelY = 0;

    shootTimer = SDL_GetTicks();

    aType = type;
    right = false;
}

BOSS::BOSS(int x, int y)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

	//Set collision box dimension
	mCollider.w = gBOSSTexture.getWidth();
	mCollider.h = gBOSSTexture.getHeight();

    //Initialize the velocity
    mVelX = 1;
    mVelY = 0;

    shootTimer = SDL_GetTicks();
    changeTimer = SDL_GetTicks();
    menaceTime = SDL_GetTicks();
    pattern = 0;
}

Bullet::Bullet(int x, int y, int speed, float angle, bool rotate, bool east )
{
	dir = angle;
	mPosX = x;
	mPosY = y;
	changeDir = rotate;
	ms = speed;
	changePat = SDL_GetTicks();
	rot = east;
}

Bullet* abul[50];
Bullet* ebul[1000];


void ALIEN::move(int xPos, int yPos)
{
	if(right == true)
	{
		mPosX += mVelX;
		if(mPosX > SCREEN_WIDTH-150)
		{
			mPosX -= mVelX;
			right = false;
		}

	}
	else
	{
		mPosX -= mVelX;
		if(mPosX < 50)
		{
			mPosX += mVelX;
			right = true;
		}
	}
	if( shootTimer +1250< SDL_GetTicks() )
	{
		if(aType == 1)
		{
			ebul[bulletCount2++] = new Bullet(mPosX+ALIEN_WIDTH/2, mPosY+ALIEN_HEIGHT, 4, 90*PI/180, false, false);
			ebul[bulletCount2++] = new Bullet(mPosX+ALIEN_WIDTH/2, mPosY+ALIEN_HEIGHT, 4, 65*PI/180, false, false);
			ebul[bulletCount2++] = new Bullet(mPosX+ALIEN_WIDTH/2, mPosY+ALIEN_HEIGHT, 4, 115*PI/180, false , false);
		}
		else
		if(aType == 2)
		{
			double angle = atan( abs(mPosX+25-xPos+25) /(double)abs(mPosY+25-yPos+25) )*180/PI;
			if( yPos+25 > mPosY+ALIEN_HEIGHT/2 )
			{
				std::cout << angle;
				std::cout << "\n";
				if( xPos+25 > mPosX+ALIEN_WIDTH/2)
				{
					ebul[bulletCount2++] = new Bullet(mPosX+ALIEN_WIDTH/2, mPosY+ALIEN_HEIGHT, 4, (angle+270)*PI/180, false, false );
				}
				else
				{
					ebul[bulletCount2++] = new Bullet(mPosX+ALIEN_WIDTH/2, mPosY+ALIEN_HEIGHT, 4, (angle+180)*PI/180, false, false);
				}

			}
			else
			{

				std::cout << angle;
				std::cout << "\n";
				if( xPos+25 < mPosX+ALIEN_HEIGHT/2 )
				{
					ebul[bulletCount2++] = new Bullet(mPosX+ALIEN_WIDTH/2, mPosY+ALIEN_HEIGHT, 4, (angle+45)*PI/180, false, false);
				}
				else
				{
					ebul[bulletCount2++] = new Bullet(mPosX+ALIEN_WIDTH/2, mPosY+ALIEN_HEIGHT, 4, (angle)*PI/180, false, false);
				}

			}

		}
		shootTimer = SDL_GetTicks();
	}
}

void BOSS::move()
{
	if( menaceTime + 750 < SDL_GetTicks())
	{
		if(right == true)
		{
			mPosX += mVelX;
			if(mPosX > SCREEN_WIDTH-300)
			{
				mPosX -= mVelX;
				right = false;
			}

		}
		else
		{
			mPosX -= mVelX;
			if(mPosX < 50)
			{
				mPosX += mVelX;
				right = true;
			}
		}
		if( shootTimer +750< SDL_GetTicks() )
		{
			if(changeTimer +7500 < SDL_GetTicks() )
			{
				changeTimer = SDL_GetTicks();
				pattern++;
				if(pattern == 3)
				{
					pattern = 0;
				}
			}
			if( pattern == 0 )
			{
				for(int i = 0; i< 24; i++)
				{
					ebul[bulletCount2++] = new Bullet(mPosX+225/2, mPosY+225/2, 4, (i*15)*PI/180, true, true);
				}
			}
			else
			if(pattern == 1)
			{
				for(int i = 0; i< 24; i++)
				{
					ebul[bulletCount2++] = new Bullet(mPosX+225/2, mPosY+225/2, 4, (i*15)*PI/180, true, false);
				}
			}
			else
			{
				for(int i = 0; i<4; i++)
				{
					ebul[bulletCount2++] = new Bullet(mPosX+225/2, mPosY+225/2, 4, i*15*PI/180, true, true);
				}

				for(int i = 0; i< 3; i++)
				{
					ebul[bulletCount2++] = new Bullet(mPosX+225/2, mPosY+225/2, 4, (  180-(i*15))*PI/180, true, false);
				}
				ebul[bulletCount2++] = new Bullet((mPosX+225/2)+25, mPosY+225/2, 5, 85, false, false);
				ebul[bulletCount2++] = new Bullet((mPosX+225/2)-25, mPosY+225/2, 5, 75, false, false);
			}

			shootTimer = SDL_GetTicks();
		}
	}
}

void FIGHTER::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= FIGHTER_VEL; break;
            case SDLK_DOWN: mVelY += FIGHTER_VEL; break;
            case SDLK_LEFT: mVelX -= FIGHTER_VEL; break;
            case SDLK_RIGHT: mVelX += FIGHTER_VEL; break;
            case ' ':
            	if( SDL_GetTicks() > shootTimer+300 )
            	{
            		abul[bulletCount++] = new Bullet(mPosX+FIGHTER_WIDTH/2, mPosY, 5, 270*PI/180, false, false);
            	  	shootTimer = SDL_GetTicks();
            	}
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += FIGHTER_VEL; break;
            case SDLK_DOWN: mVelY -= FIGHTER_VEL; break;
            case SDLK_LEFT: mVelX += FIGHTER_VEL; break;
            case SDLK_RIGHT: mVelX -= FIGHTER_VEL; break;
            case ' ':
            	if( SDL_GetTicks() > shootTimer+300 )
            	{
            		abul[bulletCount++] = new Bullet(mPosX+FIGHTER_WIDTH/2, mPosY, 5, 270*PI/180, false , false);
            		shootTimer = SDL_GetTicks();
            	}
        }
    }
}

void FIGHTER::move()
{
    //Move the FIGHTER left or right
    mPosX += mVelX;
	mCollider.x = mPosX;

    //If the FIGHTER collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + FIGHTER_WIDTH > SCREEN_WIDTH-50 ) )
    {
        //Move back
        mPosX -= mVelX;
		mCollider.x = mPosX;
    }

    //Move the FIGHTER up or down
    mPosY += mVelY;
	mCollider.y = mPosY;

    //If the FIGHTER collided or went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + FIGHTER_HEIGHT > SCREEN_HEIGHT )  )
    {
        //Move back
        mPosY -= mVelY;
		mCollider.y = mPosY;
    }
}

void Bullet::move()
{

	mPosX += (int) (ms*cos(dir) );
	mPosY += (int) (ms*sin(dir) );
	if( changeDir == true && changePat+20 < SDL_GetTicks() && changed < 100)
	{
		if( rot == false)
		{
			dir -=(1*PI/180);
		}
		else
		{
			dir +=(1*PI/180);
		}
		changePat = SDL_GetTicks();
		changed++;
	}
}

void ALIEN::render()
{
	gALIENTexture.render(mPosX, mPosY);
}

void BOSS::render()
{
	gBOSSTexture.render(mPosX, mPosY);
}


void FIGHTER::render()
{
    //Show the FIGHTER
	gFIGHTERTexture.render( mPosX, mPosY );
}

int FIGHTER::xPos()
{
	return mPosX;
}

int FIGHTER::yPos()
{
	return mPosY;
}

int FIGHTER::gotHit()
{
	if(lastHit + 250 < SDL_GetTicks() )
	{
		hp--;
		lastHit = SDL_GetTicks();
	}
	return hp;
}

int FIGHTER::getHp()
{
	return hp;
}

int ALIEN::xPos()
{
	return mPosX;
}

int ALIEN::yPos()
{
	return mPosY;
}

int ALIEN::gotHit()
{
	hp--;
	return hp;
}

int BOSS::xPos()
{
	return mPosX;
}

int BOSS::yPos()
{
	return mPosY;
}

int BOSS::gotHit()
{
	if( menaceTime + 750 < SDL_GetTicks())
	{
		hp--;
	}
	return hp;

}

int BOSS::getHp()
{
	return hp;
}

void Bullet::render()
{
	filledCircleRGBA(gRenderer, mPosX, mPosY, Bullet_RADIUS, 0xFF,0x0,0x0,0xFF);
}

int Bullet::xPos()
{
	return mPosX;
}

int Bullet::yPos()
{
	return mPosY;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Last Boss", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );


			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load press texture
	if( !gFIGHTERTexture.loadFromFile( "src/MiG-51S.bmp" ) )
	{
		printf( "Failed to load FIGHTER texture!\n" );
		success = false;
	}
	if( !gALIENTexture.loadFromFile( "src/alien1.bmp" ) )
	{
		printf( "Failed to load FIGHTER texture!\n" );
		success = false;
	}
	if( !gBOSSTexture.loadFromFile( "src/spacestation.bmp" ) )
	{
		printf( "Failed to load FIGHTER texture!\n" );
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	gFIGHTERTexture.free();
	gALIENTexture.free();
	gBOSSTexture.free();
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The FIGHTER that will be moving around on the screen
			FIGHTER fight;

			ALIEN* aliens[100];
			for(int i = 0; i< 5; i++)
			{
				aliens[alienCount++] = new ALIEN( SCREEN_WIDTH-150-(100*i), 70*i, 1);
			}
			int hpLeft = 10;
			bool boss = false;
			BOSS* station;
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the FIGHTER
					fight.handleEvent( e );

				}
				//Move the FIGHTER and check collision
				fight.move();
				for(int i = 0; i<alienCount; i++)
				{
					aliens[i]->move(fight.xPos(), fight.yPos());
				}

				if( boss )
				{
					station->move();
				}
				for(int i = 0; i<bulletCount; i++)
				{
					abul[i]->move();
					if(abul[i]->xPos()+Bullet_RADIUS < 0 || abul[i]->xPos() > SCREEN_WIDTH-50 ||
					   abul[i]->yPos()+Bullet_RADIUS < 0 || abul[i]->yPos() > SCREEN_HEIGHT
					  )
					{
						for(int p = i; p<bulletCount-1; p++)
						{
							abul[p] = abul[p+1];
						}
						i--;
						bulletCount--;
					}
					else
					if( boss == false)
					{
						for(int k = 0; k<alienCount; k++)
						{
							if( sqrt(  pow( (double) ( (aliens[k]->xPos()+25)-abul[i]->xPos() ), 2.0 ) + pow( (double) ( (aliens[k]->yPos()+25)-abul[i]->yPos() ), 2.0 ) ) < 30)
							{
								for(int p = i; p<bulletCount-1; p++)
								{
									abul[p] = abul[p+1];
								}
								i--;
								bulletCount--;
								hpLeft = aliens[k]->gotHit();
								if(hpLeft == 0)
								{
									if(alienCount==1)
									{
										alienCount--;
										boss = true;
										station = new BOSS( ( ( (SCREEN_WIDTH-50)/2 )-(225/2)), SCREEN_HEIGHT/15 );
										break;
									}
									else
									{
										for(int o = k; o<alienCount-1;o++)
										{
											aliens[o] = aliens[o+1];
										}
										alienCount--;

									}
								}
								break;
							}
						}
					}
					else
					{
						if( sqrt(  pow( (double) ( (station->xPos()+100)-abul[i]->xPos() ), 2.0 ) + pow( (double) ( (station->yPos()+100)-abul[i]->yPos() ), 2.0 ) ) < 60)
						{
							for(int p = i; p<bulletCount-1; p++)
							{
								abul[p] = abul[p+1];
							}
							i--;
							bulletCount--;
							hpLeft = station->gotHit();
							if(hpLeft == 0)
							{
								quit = true;
							}
						}
					}
				}

				for(int i = 0; i<bulletCount2; i++)
				{
					ebul[i]->move();
					if( sqrt(  pow( (double) ( (fight.xPos()+25)-ebul[i]->xPos() ), 2.0 ) + pow( (double) ( (fight.yPos()+25)-ebul[i]->yPos() ), 2.0 ) ) < 30)
					{
						for(int p = i; p<bulletCount2-1; p++)
						{
							ebul[p] = ebul[p+1];
						}
						i--;
						bulletCount2--;
						hpLeft = fight.gotHit();
						if(hpLeft == 0)
						{
							quit = true;
						}

					}
					else
					if(ebul[i]->xPos()+Bullet_RADIUS < 0 || ebul[i]->xPos() > SCREEN_WIDTH-50 ||
					   ebul[i]->yPos()+Bullet_RADIUS < 0 || ebul[i]->yPos() > SCREEN_HEIGHT
					  )
					{
						for(int p = i; p<bulletCount2-1; p++)
						{
							ebul[p] = ebul[p+1];
						}
						i--;
						bulletCount2--;
					}


				}
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render wall
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				//Render FIGHTER
				boxRGBA(gRenderer, 0, 0, SCREEN_WIDTH-50, SCREEN_HEIGHT, 0x0,0x0,0x0,0xFF);
				boxRGBA(gRenderer, SCREEN_WIDTH-40, SCREEN_HEIGHT-(20*fight.getHp()), SCREEN_WIDTH-25, SCREEN_HEIGHT, 0x0,0x0,0xFF,0xFF);

				fight.render();
				if(boss)
				{
					boxRGBA(gRenderer, SCREEN_WIDTH-40, 0, SCREEN_WIDTH-25, 0+(station->getHp()*10), 0xFF,0x0,0x0,0xFF);
					station->render();

				}

				for(int i = 0; i<alienCount;i++)
				{
					aliens[i]->render();
				}

				for(int i = 0; i<bulletCount; i++)
				{
					abul[i]->render();
				}

				for(int i = 0; i<bulletCount2; i++)
				{
					ebul[i]->render();
				}
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
