//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

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
	Vector2D(float X = 0, float Y = 0)
	{
		x = X;
		y = Y;
	};
	~Vector2D() {} ;

	float x, y;

	Vector2D operator*(float scalar) const
	{
		return Vector2D(x * scalar, y * scalar);
	}


	Vector2D operator+(const Vector2D &vect) const
	{
		return Vector2D(x + vect.x, y + vect.y);
	}

	Vector2D operator-(const Vector2D &vect) const
	{
		return Vector2D(x - vect.x, y - vect.y);
	}

	void rotate(float angle)
	{
		float xt = (x * cosf(angle)) - (y * sinf(angle));
		float yt = (y * cosf(angle)) + (x * sinf(angle));
		x = xt;
		y = yt;
	}

	float crossproduct(const Vector2D &vect2) const
	{
		return (this->x * vect2.y) - (this->y * vect2.x);
	}

	float magnitude()
	{
		return sqrtf(x * x +y * y);
	}

	void normalise()
	{
		float mag = sqrtf(x* x + y * y);
		this->x = x / mag;
		this->y = y / mag;
	}

	// return dot product
	float dotproduct(const Vector2D &vect) const
	{
		return (x * vect.x) + (y * vect.y);
	}

	void setXY(float X, float Y){
  x = X;
  y = Y;
	}
};

class Timer{
   public:
     Timer();
     void start();
     void stop();
     void pause();
     void unpause();
     int get_ticks();
     bool is_started();
     bool is_paused();
   private:
     int startTicks;
     int pausedTicks;
     bool paused;
     bool started;
};

Timer::Timer(){
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start(){
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}

void Timer::stop(){
    started = false;
    paused = false;
}

void Timer::pause(){
    if( ( started == true ) && ( paused == false ) ){
    paused = true;
    pausedTicks = SDL_GetTicks() - startTicks;
        }
}

void Timer::unpause(){
    if( paused == true ){
    paused = false;
    startTicks = SDL_GetTicks() - pausedTicks;
    pausedTicks = 0;
    }
}

int Timer::get_ticks(){
    if( started == true ){
        if( paused == true ){
            return pausedTicks;}
        else{
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}

bool Timer::is_started(){return started;}
bool Timer::is_paused(){return paused;}



//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	// Sample Load
	// if( !gBGTexture.loadFromFile( "bg.png" ) )
	// {
	// 	printf( "Failed to load background texture!\n" );
	// 	success = false;
	// }

	return success;
}

void close()
{
	//Free loaded images
	// Sample : gBGTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


//Scene textures should be initiated here
// All pictures/media/TTF should be in loadMedia()
// Don't forget to free() all textures at close()

const int PLAYER_ANIMATION_FRAMES = 3;
SDL_Rect gSpriteClips[ PLAYER_ANIMATION_FRAMES ];



class Player{
 public:
 Player(float, float);
 ~Player();
 void update();
 void render();
 void move(SDL_Event &e);
 void LandTileCollision();

 private:
  double posX, posY;
  // Physics
  double Gravity = 4.75;
  double gCap = 6.6;
  double gVel = 0;
  Vector2D center;
  float width = 0;
  float height = 0;
  // Rendering
  SDL_Rect* currentClip = &gSpriteClips[1];
  bool spriteFlag = false;

};


class LandTile{
  public:
    LandTile(float x, float y);
    ~LandTile();
    void render();

  private:
    float posX, posY;
    float width, height;
};

class LandTileGroup{
 public:
   LandTileGroup(std::vector<float> &x, std::vector<float> &y);
   ~LandTileGroup();
   void render();
   std::vector<LandTile> container;
};

Player::Player(float x, float y){
 posX = x;
 posY = y;
 center.x = posX+width;
 center.y = posY+height;
}

Player::~Player(){}

void Player::update(){
 posY = posY + gVel;
 gVel = gVel + Gravity;
 if (gVel > gCap){
  gVel = gCap;
 }
};

void Player::render(){
//  if ()
//   currentClip = &gSpriteClips[1];
//  else if (angle < 0 && spriteFlag){
//   currentClip = &gSpriteClips[0];
//   spriteFlag = false;
//  }
//  else {
//   currentClip = &gSpriteClips[2];
//   spriteFlag = true;
//  }

 }

void Player::move(SDL_Event &e){

//Movement Code here


}


LandTile::LandTile(float x, float y){
  posX = x;
  posY = y;
}

LandTile::~LandTile(){}

void LandTile::render(){

}

LandTileGroup::LandTileGroup(std::vector<float> &x, std::vector<float> &y){
 if (x.empty() || y.empty()){
   for (int i = 0; i <= SCREEN_WIDTH/2; i++){
     container.emplace_back(50*i, 50);
   }
 }
 else{
   for(int i = 0; i <= x.size(); i++){
     container.emplace_back(x[i], y[i]);
   }
 }
}

void levelOne(){

 				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Update screen
				SDL_RenderPresent( gRenderer );
}

void levelTwo(){

}

void levelThree(){

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
			Timer fps;

			// Game Object initialization


			//While application is running
			while( !quit )
			{
    fps.start();
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}


    levelOne();

				//FPS Cap
				if( fps.get_ticks() < 1000 / FPS ){
       				SDL_Delay( ( 1000 / FPS ) - fps.get_ticks() );
       			}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
