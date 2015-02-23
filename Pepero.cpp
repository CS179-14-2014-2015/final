//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>

//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 300;

const int FPS = 20;
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

//Scene textures should be initiated here
// All pictures/media/TTF should be in loadMedia()
// Don't forget to free() all textures at close()

LTexture playerSprite;
LTexture landSprite;
LTexture bg1Sprite;
LTexture bg2Sprite;
LTexture ballSprite;

const int PLAYER_ANIMATION_FRAMES = 6;
SDL_Rect gSpriteClips[ PLAYER_ANIMATION_FRAMES ];

const int BALL_ANIMATION_FRAMES = 7;
SDL_Rect ballSpriteClip[BALL_ANIMATION_FRAMES];

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

	 if( !playerSprite.loadFromFile( "Assets/player.png" ) )
	 {
	 	printf( "Failed to load player texture!\n" );
	 	success = false;
	 }

	 else
	 {
        for (int i = 0; i < 3; i++)
        {
            gSpriteClips[i].x = i*32;
            gSpriteClips[i].y = 0;
            gSpriteClips[i].w = 32;
            gSpriteClips[i].h = 32;
        }
        for (int i = 3; i < 6; i++)
        {
            gSpriteClips[i].x = (i-3)*32;
            gSpriteClips[i].y = 33;
            gSpriteClips[i].w = 32;
            gSpriteClips[i].h = 32;
        }
    }

  if( !landSprite.loadFromFile( "Assets/platform.png" ) )
	 {
	 	printf( "Failed to load land texture!\n" );
	 	success = false;
	 }

	 if( !bg1Sprite.loadFromFile( "Assets/bg1.png" ) )
	 {
	 	printf( "Failed to load background 1 texture!\n" );
	 	success = false;
	 }

	 if(!bg2Sprite.loadFromFile("Assets/bg2.png"))
	 {
    printf("Failed to load background 2 texture!\n");
    success = false;
	 }

	 if( !ballSprite.loadFromFile( "Assets/balls.png" ) )
	 {
	 	printf( "Failed to load ball texture!\n" );
	 	success = false;
	 }

	 else
	 {
        for (int i = 0; i < 7; i++)
        {
            ballSpriteClip[i].x = 0;
            ballSpriteClip[i].y = (i*32);
            ballSpriteClip[i].w = 32;
            ballSpriteClip[i].h = 32;
        }
    }
    return success;
}

void close()
{
	//Free loaded images
	landSprite.free();
	playerSprite.free();
	bg1Sprite.free();
  bg2Sprite.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

class LandTile{
  public:
    LandTile(float x, float y);
    ~LandTile();
    void render();
    float posX;
    float posY;
    float width, height;
};

class LandTileGroup{
 public:
   LandTileGroup(float x, float y);
   ~LandTileGroup();
   void move();
   void render();
   std::vector<LandTile> container;

};

class Player{
  public:
    Player(float, float);
    ~Player();
    void update();
    void render();
    void move(SDL_Event &e);
    void LandTileCollision(LandTileGroup &landTiles);
    void jump();

  private:
    double posX, posY;
    double velX, velY;

    // Physics
    double Gravity = 4.75;
    double gCap = 6.6;
    double gVel = 0;
    Vector2D center;
    float width = 0;
    float height = 0;
    float velYCap = 50;

    // Rendering
    SDL_Rect* currentClip = &gSpriteClips[2];
    int direction = 1;
    bool jumping = false;
    bool run = false;
    bool running = false;
    bool colliding = true;
};

Player::Player(float x, float y){
 posX = x;
 posY = y;
 center.x = posX+width;
 center.y = posY+height;
 velX = 10;
 velY = 2;
}

Player::~Player(){}

void Player::update(){
  if (jumping == true)
    {
      gVel = 0;
      posY -= velY;
    }
  else
  {
    posY = posY + gVel;
    gVel = gVel + Gravity;
    if (gVel > gCap){
      gVel = gCap;
    }
  }
};

void Player::render(){
  if (direction == 0)
  {
    if (run == false)
    {
      currentClip = &gSpriteClips[2];
    }
    else if (run == true)
    {
      if (running == false)
      {
        currentClip = &gSpriteClips[0];
        running = true;
      }
      else
      {
        currentClip = &gSpriteClips[1];
        running = false;
      }
    }
  }
  else if (direction == 1)
  {
    if (run == false)
    {
      currentClip = &gSpriteClips[3];
    }
    else if (run == true)
    {
      if (running == false)
      {
        currentClip = &gSpriteClips[4];
        running = true;
      }
      else
      {
        currentClip = &gSpriteClips[5];
        running = false;
      }
    }
  }
  playerSprite.render(posX,posY,currentClip);
}

void Player::move(SDL_Event &e){
  velY = 0;
//Movement Code here
  if (e.type == SDL_KEYDOWN)
  {
    /* Check the SDLKey values and move change the coords */
    if (e.key.keysym.sym == SDLK_LEFT)
    {
      direction = 0;
      run = true;
      posX -= velX;
    }
    else if (e.key.keysym.sym == SDLK_RIGHT)
    {
      direction = 1;
      run = true;
      posX += velX;
    }

    if (e.key.keysym.sym == SDLK_UP)
    {
      if (jumping == false)
      {
        jump();
      }
    }
  }
  else if (e.type == SDL_KEYUP)
  {
    if (e.key.keysym.sym == SDLK_LEFT)
    {
      run = false;
    }
    else if (e.key.keysym.sym == SDLK_RIGHT)
    {
      run = false;
    }
  }
}

void Player::jump()
{
  if (velY < velYCap)
  {
    velY += 15;
  }
  jumping = true;
}

void Player::LandTileCollision(LandTileGroup &landTiles){
 for (auto &x : landTiles.container){
   //if collided

   // else
 }

}

LandTile::LandTile(float x, float y){
  posX = x;
  posY = y;
}

LandTile::~LandTile(){}

void LandTile::render(){
  landSprite.render(posX,posY);
}

LandTileGroup::LandTileGroup(float x , float y ){
 if (x == NULL || y == NULL){
   for (int i = 0; i <= SCREEN_WIDTH/landSprite.getWidth(); i++){
     container.emplace_back(landSprite.getWidth()*i, SCREEN_HEIGHT-landSprite.getHeight());
   }
 }
 else{
   for(int i = 0; i <= rand() % 5 + 2 ; i++){
     container.emplace_back(x + i*landSprite.getWidth(), y);
   }
 }
}

void LandTileGroup::move(){
 for (auto &x : container){
   x.posX -= 10;
 }

 if (container[0].posX < 0){
   int y = rand() % (SCREEN_HEIGHT - 100) + 50;
   for (int i = 0; i <= rand() % 5 + 2 ; i++){
     container[i].posX = SCREEN_WIDTH + i*landSprite.getWidth();
     container[i].posY = y;
     }
   }
}

void LandTileGroup::render(){
 for (auto &x : container){
   x.render();
 }
}

LandTileGroup::~LandTileGroup(){};

class Ball{
  public:
    Ball();
    ~Ball();
    void render();
    void move();
    float posX;
    float posY;
    float width, height;
};

Ball::Ball()
{
  width = ballSprite.getWidth();
  height = ballSprite.getHeight();
  posX = (SCREEN_WIDTH/2) - (width/2);
  posY = (SCREEN_HEIGHT/3);
}

void Ball::render()
{
  ballSprite.render(posX, posY, &ballSpriteClip[5]);
}

void Ball::move()
{
  posX += rand()%8*sin(angle);
  posY += rand()%8*sin(angle);
}

Ball::~Ball()
{

}

void levelOne(Player &player, SDL_Event &e){
    static int scrollingOffset = 0;
    static LandTileGroup land(0,0);
    static LandTileGroup randomLand(SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
    //logic
    randomLand.move();
    player.update();
    player.move(e);


    scrollingOffset-= 2;
				if( scrollingOffset < -bg1Sprite.getWidth() )
				{
					scrollingOffset = 0;
				}


 				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				bg1Sprite.render( scrollingOffset, 0 );
				bg1Sprite.render( scrollingOffset + bg1Sprite.getWidth(), 0 );


    land.render();
    randomLand.render();
    player.render();

				//Update screen
				SDL_RenderPresent( gRenderer );
}

void levelTwo(Player &player, SDL_Event &e){
  static LandTileGroup a(100,100);
  static Ball *b = new Ball();
  player.move(e);

  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( gRenderer );

  bg2Sprite.render(0,0);
  a.render();
  b->render();
  player.render();

  SDL_RenderPresent(gRenderer);
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
   srand(time(0));
			//Event handler
			SDL_Event e;
			Timer fps;

			// Game Object initialization
   Player player(100,SCREEN_HEIGHT-50);
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

        levelTwo(player, e);

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
