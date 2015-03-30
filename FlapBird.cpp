//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>
#include <SDL2_gfxPrimitives.h>
#include <vector>
#include <algorithm>
//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 300;

const int FPS = 30;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;


//Texture wrapper class from LazyFoo
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


class Vector2D
{
    public:
        Vector2D( double X = 0, double Y = 0 )
        {
            x = X;
            y = Y;
        };
        ~Vector2D()
        {
        };

        double x, y;

        Vector2D operator*(double scalar) const
        {
            return Vector2D( x * scalar, y * scalar );
        }

        Vector2D operator+( const Vector2D &vect ) const
        {
            return Vector2D( x + vect.x, y + vect.y );
        }

        Vector2D operator-(const Vector2D &vect) const
        {
            return Vector2D( x - vect.x, y - vect.y );
        }

        void rotate( float angle )
        {
            double xt = (x * cosf(angle)) - (y * sinf(angle));
            double yt = (y * cosf(angle)) + (x * sinf(angle));
            x = xt;
            y = yt;
        }

        double crossproduct(const Vector2D &vect2) const
        {
            return (this->x * vect2.y) - (this->y * vect2.x);
        }

        double magnitude()
        {
            return sqrtf( x * x + y * y );
        }

        void normalise()
        {
            double mag = sqrtf( x * x + y * y );
            this->x = x / mag;
            this->y = y / mag;
        }

        // return dot product
        float dotproduct(const Vector2D &vect) const
        {
            return (x * vect.x) + (y * vect.y);
        }

        void setXY( double X, double Y )
        {
            x = X;
            y = Y;
        }
};


class Timer
{
    public:
        Timer( double pX, double pY );
        void start();
        void stop();
        void pause();
        void unpause();
        int get_ticks();
        bool is_started();
        bool is_paused();
        void update();
        void render();
        double posX, posY;
        int display;

    private:
        int startTicks;
        int pausedTicks;
        bool paused;
        bool started;
};

TTF_Font *gFont = nullptr;

//Scene textures
LTexture gBGTexture;
LTexture gBirdSpriteSheetTexture;
LTexture gPipeUpTexture;
LTexture gPipeDownTexture;
LTexture gTextTexture;
LTexture gScoreTexture;
const int BIRD_ANIMATION_FRAMES = 3;
SDL_Rect gSpriteClips[ BIRD_ANIMATION_FRAMES ];



class Pipes
{
    public:
        Pipes(int i);
        void update();
        void render();
        Vector2D Up, Down, centerUp, centerDown;
        int width = gPipeDownTexture.getWidth();
        int height = gPipeDownTexture.getHeight();
};


class Bird
{
    public:
        Bird( double pX, double pY );
        ~Bird();
        void update();
        void render();
        void fly();
        void calculateOBB();
        void OBBCollision( std::vector<Pipes> &pipes );
        Vector2D center;
        short width = gBirdSpriteSheetTexture.getWidth() / BIRD_ANIMATION_FRAMES;
        short height = gBirdSpriteSheetTexture.getHeight();
        double posX, posY;

        bool hasCollided = false;

    private:
        std::vector<short> OBBx{ 0, 0, 0, 0 };
        std::vector<short> OBBy{ 0, 0, 0, 0 };
        // Physics
        double Gravity = 1;
        double gVel = 0;
        double gCap = 12;
        double fCap = 80;
        double fVel = -70;
        double fImpulse = 50;
        double angle = 0;

        //OBB
        SDL_Rect* currentClip = &gSpriteClips[1];
        bool spriteFlag = false;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

float randomRange( float Min, float Max )
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

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
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
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
		gWindow = SDL_CreateWindow( "Fwappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				//Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
			}
		}
	}
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

    //Open the font
    gFont = TTF_OpenFont( "FlappyBirdy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 255, 0, 0 };
        if( !gTextTexture.loadFromRenderedText( "Collided", textColor ) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
        if( !gScoreTexture.loadFromRenderedText( "0", textColor ) )
        {
            printf( "Failed to render score texture!\n" );
            success = false;
        }
    }

	if( !gBGTexture.loadFromFile( "bg.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	if( !gBirdSpriteSheetTexture.loadFromFile( "birdsprite.png" ) )
    {
        printf( "Failed to load bird sprite sheet texture!\n" );
		success = false;
	}

	if( !gPipeUpTexture.loadFromFile( "PipeUp.png" ) )
    {
        printf( "Failed to load bird sprite sheet texture!\n" );
		success = false;
	}
	if( !gPipeDownTexture.loadFromFile( "PipeDown.png" ) )
	{
        printf( "Failed to load bird sprite sheet texture!\n" );
		success = false;
	}
	else
	{
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w =  26;
        gSpriteClips[ 0 ].h =  18;

        gSpriteClips[ 1 ].x =  26;
        gSpriteClips[ 1 ].y =   0;
        gSpriteClips[ 1 ].w =  26;
        gSpriteClips[ 1 ].h =  18;

        gSpriteClips[ 2 ].x =  52;
        gSpriteClips[ 2 ].y =   0;
        gSpriteClips[ 2 ].w =  26;
        gSpriteClips[ 2 ].h =  18;
	}
	return success;
}

void close()
{
	//Free loaded images
	gBirdSpriteSheetTexture.free();
	gBGTexture.free();
	gPipeDownTexture.free();
	gPipeUpTexture.free();
	gTextTexture.free();
	gScoreTexture.free();

	//Free global font
    TTF_CloseFont( gFont );
    gFont = nullptr;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

Timer::Timer( double pX, double pY )
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
    posX = pX;
    posY = pY;
    display = 0;
}

void Timer::start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    started = false;
    paused = false;
}

void Timer::pause()
{
    if( ( started == true ) && ( paused == false ) )
    {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    if( paused == true )
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    if( started == true )
    {
        if( paused == true )
        {
            return pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}

bool Timer::is_started()
{
    return started;
}
bool Timer::is_paused()
{
    return paused;
}

void Timer::update()
{
    if( started && !paused )
    {
        display += 1;
    }
    //display = display + get_ticks();
}

void Timer::render()
{
    /*
    std::ostringstream ss;
    ss << display;
    gFont = TTF_OpenFont( "FlappyBirdy.ttf", 28 );
    SDL_Color color = { 255, 0, 0 };
    std::string dstring = ss.str().c_str();
    //std::cout << dstring << "\n";
    gScoreTexture.loadFromRenderedText( dstring, color );
    //gScoreTexture.loadFromRenderedText( std::to_string( display ), color );
    gScoreTexture.render( 15, 15 );
    */
}

Bird::Bird( double pX, double pY )
{
    posX = pX;
    posY = pY;
    center.x = posX+width/2;
    center.y = posY+height/2;
}

Bird::~Bird()
{
}

void Bird::update()
{
    posY = posY + gVel;
    gVel = gVel + Gravity;
    angle = angle + 8;
    if( gVel > gCap )
    {
        gVel = gCap;
    }
    if( angle > 60 )
    {
        angle = 60;
    }
    calculateOBB();
};

void Bird::render()
{
    if( angle > 0 )
    {
        currentClip = &gSpriteClips[1];
    }
    else if( angle < 0 && spriteFlag )
    {
        currentClip = &gSpriteClips[0];
        spriteFlag = false;
    }
    else
    {
        currentClip = &gSpriteClips[2];
        spriteFlag = true;
    }
    short arrx[4],arry[4];
    std::copy( OBBx.begin(), OBBx.end(), arrx );
    std::copy( OBBy.begin(), OBBy.end(), arry );
    polygonRGBA( gRenderer, arrx, arry, 4, 255, 0, 0, 255 );
    if( hasCollided )
    {
        gTextTexture.render( 100, 100 );
    }
    gBirdSpriteSheetTexture.render( posX, posY, currentClip, angle );
 }

void Bird::fly()
{
    posY += fVel;
    gVel = 0;
    angle = -20;
    calculateOBB();
}

void Bird::calculateOBB()
{
    center.x = posX + width/2;
    center.y = posY + height/2;
    double cosA = cos(angle * M_PI/180);
    double sinA = sin(angle * M_PI/180);
    short ULx  =  center.x + ( width / 2 )  * cosA - ( height / 2 ) * sinA;
    short ULy  =  center.y + ( height / 2 ) * cosA + ( width / 2 )  * sinA;
    short URx  =  center.x - ( width / 2 )  * cosA - ( height / 2 ) * sinA;
    short URy  =  center.y + ( height / 2 ) * cosA - ( width / 2 )  * sinA;
    short BLx  =  center.x + ( width / 2 )  * cosA + ( height / 2 ) * sinA;
    short BLy  =  center.y - ( height / 2 ) * cosA + ( width / 2 )  * sinA;
    short BRx  =  center.x - ( width / 2 )  * cosA + ( height / 2 ) * sinA;
    short BRy  =  center.y - ( height / 2 ) * cosA - ( width / 2 )  * sinA;

    OBBx = { ULx, URx, BRx, BLx };
    OBBy = { ULy,URy,BRy,BLy };
}


Pipes::Pipes( int i )
{
    Down.setXY( SCREEN_WIDTH + 80 * i, randomRange( -140,-(SCREEN_HEIGHT * 1/3) ) );
    Up.setXY( Down.x, randomRange( SCREEN_HEIGHT * 3/5,SCREEN_HEIGHT * 6/8 ) );
    centerDown.setXY( Down.x + width/2, Down.y + height/2 );
    centerUp.setXY( Up.x + width/2, Up.y + height/2 );
};

void Pipes::render()
{
    gPipeDownTexture.render( (int) Down.x, (int)Down.y );
    gPipeUpTexture.render( (int) Up.x, (int)Up.y );
};

void Pipes::update()
{
    Up.x -= 5;
    Down.x -= 5;
    centerDown.setXY( Down.x + width/2, Down.y + height/2 );
    centerUp.setXY( Up.x + width/2, Up.y + height/2 );
    if( Up.x <= -20 )
    {
        Up.x = SCREEN_WIDTH;
        Down.x = SCREEN_WIDTH;
    }
};

double calcI( double width, double height, Vector2D axis )
{
    double result;
    Vector2D compare0, compare1, compare2, compare3;
    compare0 =  axis * ( height + width);
    compare1 =  axis * (-height + width);
    compare2 =  axis * ( height - width);
    compare3 =  axis * (-height - width);
    result = std::max( compare0.magnitude(), compare1.magnitude() );
    result = std::max( result, compare2.magnitude() );
    result = std::max( result, compare3.magnitude() );
    return result;
}

void Bird::OBBCollision( std::vector<Pipes> &pipes )
{
    // Check whether Bird is in upper or lower half of screen
    Vector2D AB,axis;
    double Ia, Ib, distance;
    hasCollided = false;
    // PipeDown collision detection
    if( center.y < SCREEN_HEIGHT/2 )
    {
        for( auto &pipe : pipes )
        {
            AB = pipe.Down - center;
            // 1st axis
            axis = AB;
            distance = axis.dotproduct( AB );
            Ia = calcI( height/2, width/2, axis );
            Ib = calcI( pipe.width/2, pipe.height/2, axis );
            if( std::abs( distance ) < ( Ia + Ib ) )
            {
                hasCollided = true;
            }
        }
    }
    else
    {
        for( auto &pipe : pipes )
        {
            AB = pipe.Up - center;
            // 1st axis
            axis = AB;
            distance = axis.dotproduct( AB );
            Ia = calcI( height/2, width/2,axis );
            Ib = calcI( pipe.width/2, pipe.height/2, axis );
            if( std::abs( distance ) < ( Ia + Ib ) )
            {
                hasCollided = true;
            }
        }
    }
}
//      printf("%f", Ia);
//      printf("\n");
//      printf("%f", Ib);
//      printf("\n");
//      printf("%f", distance);
//      printf("\n");
//      printf("------------------");
//      printf("\n");

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
            Bird bird = Bird( 100, 100 );
            Bird bird2 = Bird( 150, 100 );
            Timer fps = Timer( 0, 0 );
            Timer score = Timer( 15, 15 );
            Timer score2 = Timer( 15, 45 );
            int scrollingOffset = 0;

            std::vector<Pipes> pipes;
            for( int i = 0; i < 10; i++ )
            {
                pipes.emplace_back(i);
            }
            //While application is running
			while( !quit )
			{
                fps.start();
                score.start();
                score2.start();

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
                    {
                        if( e.key.keysym.sym == SDLK_SPACE)
                        {
                            bird.fly();
                        }
                        if( e.key.keysym.sym == SDLK_UP )
                        {
                            bird2.fly();
                        }
					}
				}
                bird.update();
                bird2.update();
                for( auto &pipe : pipes )
                {
                    pipe.update();
                }

                bird.OBBCollision( pipes );
                bird2.OBBCollision( pipes );
                if( bird.hasCollided && !score.is_paused() )
                {
                    score.pause();
                }
                else if( score.is_paused() )
                {
                    score.unpause();
                }
                score.update();

                if( bird2.hasCollided && !score2.is_paused() )
                {
                    score2.pause();
                }
                else if( score2.is_paused() )
                {
                    score2.unpause();
                }
                score2.update();

                //Scroll background
				scrollingOffset -= 2;
				if( scrollingOffset < -gBGTexture.getWidth() )
				{
					scrollingOffset = 0;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
                //Render background
				gBGTexture.render( scrollingOffset, 0 );
				gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0 );
                for( auto &pipe : pipes )
                {
                    pipe.render();
                }
				bird.render();
				bird2.render();
				score.render();
				score2.render();

				std::cout << "Player 1: " << score.display << "     Player 2: " << score2.display << "\n";

				//Update screen
				SDL_RenderPresent( gRenderer );
				if( fps.get_ticks() < 1000 / FPS )
                {
                    SDL_Delay( ( 1000 / FPS ) - fps.get_ticks() );
                }
			}
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
