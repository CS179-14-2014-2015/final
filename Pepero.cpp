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

const int FPS = 30;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;


//Texture wrapper class from LazyFoo
class LTexture{
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

class Vector2D{

public:
	Vector2D(double X = 0, double Y = 0)
	{
		x = X;
		y = Y;
	};
	~Vector2D() {} ;

	double x, y;

	Vector2D operator*(double scalar) const
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

	void rotate(double angle)
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
		double mag = sqrtf(x* x + y * y);
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

double distanceSquared(int x1, int y1, int x2, int y2);

LTexture::LTexture(){
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture(){
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path ){
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
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor ){
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

void LTexture::free(){
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ){
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending ){
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha ){
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ){
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

int LTexture::getWidth(){
	return mWidth;
}

int LTexture::getHeight(){
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
LTexture boulderSprite;
LTexture portalSprite;

const int PLAYER_ANIMATION_FRAMES = 6;
SDL_Rect gSpriteClips[ PLAYER_ANIMATION_FRAMES ];

const int BALL_ANIMATION_FRAMES = 7;
SDL_Rect ballSpriteClip[BALL_ANIMATION_FRAMES];

bool init(){
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

bool loadMedia(){
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

  if( !portalSprite.loadFromFile( "Assets/wormhole.png" ) )
	 {
	 	printf( "Failed to load portal texture!\n" );
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

  if( !boulderSprite.loadFromFile( "Assets/Boulder.png" ) )
	 {
	 	printf( "Failed to load boulder texture!\n" );
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

void close(){
	//Free loaded images
	landSprite.free();
	playerSprite.free();
	bg1Sprite.free();
 bg2Sprite.free();
 ballSprite.free();
 boulderSprite.free();
 portalSprite.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

double distanceSquared( int x1, int y1, int x2, int y2 ){
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

class LandTile{
  public:
    LandTile(double x, double y);
    ~LandTile();
    void render();
    Vector2D position, dimension;
};

class LandTileGroup{
 public:
   LandTileGroup(float x, float y);
   ~LandTileGroup();
   void move();
   void moveLand();
   void render();
   std::vector<LandTile> container;
};

struct Circle{
  int x,y;
  int r;
};

class Boulder{
  public:
    Boulder();
    void render();
    Circle collider;
    Vector2D position, dimension;
    ~Boulder();

  private:
    int animationClip = 0;
};

class Ball{
  public:
    Ball();
    Circle collider;
    ~Ball();
    void render();
    void move();
    void moveBernoulli();
    Vector2D position, dimension;

  private:
    int velX = 1;
};

class Portal{
  public:
    Portal();
    Vector2D position, dimension;
    Circle collider;
    void render();
    ~Portal();

  private:
    int angle = 0;
};

class Player{
  public:
    Player(double, double);
    ~Player();
    void update();
    void render();
    void move(SDL_Event &e);
    void LandTileCollision(LandTileGroup &landTiles);
    void boulderCollision(Boulder &);
    void ballCollision(Ball &);
    void portalCollision(Portal &);
    int currentLevel = 1;
    bool portalOn = false;

    Vector2D position;

  private:
    Vector2D velocity,dimension;

    // Physics
    double Gravity = 2;
    double gCap = 4;
    double gVel = 0;
    double jumpHeight = 80;

    // AABB
    SDL_Rect collider, result;
    bool collidedTile = false;
    bool collidedBoulder = false;

    // Rendering
    SDL_Rect* currentClip = &gSpriteClips[2];
    int direction = 1;

    // control flags
    bool run = false;
    bool running = false;
    bool notJumped = true;
    int jumpCounter = 0;
    bool onTile = false;
    bool notSkipped = true;
};

Player::Player(double x, double y){
 position.x = x;
 position.y = y;
 velocity.x = 5;
 velocity.y = 50;
 dimension.x = playerSprite.getWidth()/3;
 dimension.y = playerSprite.getHeight()/2;

}

Player::~Player(){}

void Player::update(){
    position.y = position.y + gVel;
    gVel = gVel + Gravity;
    if (gVel > gCap){
      gVel = gCap;
    }
    if (position.y > SCREEN_HEIGHT - 50){
      position.y = SCREEN_HEIGHT - 50;
      jumpCounter = 0;
    }
}

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
    playerSprite.render(position.x,position.y,currentClip);
}

void Player::move(SDL_Event &e){

//Movement Code here
  if (e.type == SDL_KEYDOWN)
  {
    /* Check the SDLKey values and move change the coords */
    if (e.key.keysym.sym == SDLK_LEFT)
    {
      direction = 0;
      run = true;
      if (position.x <= 0)
      {
        position.x = 0;
      }
      else
      {
        position.x -= velocity.x;
      }
    }
    if (e.key.keysym.sym == SDLK_RIGHT)
    {
      direction = 1;
      run = true;
      if (position.x >= SCREEN_WIDTH - dimension.x)
      {
        position.x = SCREEN_WIDTH - dimension.x;
      }
      else
      {
        position.x += velocity.x;
      }
   }

   if (e.key.keysym.sym == SDLK_UP && notJumped && (jumpCounter < 2))
   {
       position.y -= jumpHeight;
       notJumped = false;
       jumpCounter++;
   }

   if (e.key.keysym.sym == SDLK_DOWN && notSkipped && onTile){
     position.y += 20;
     notSkipped = false;
     onTile = false;
     notJumped = false;
   }
  }
  else if (e.type == SDL_KEYUP)
  {
    if (e.key.keysym.sym == SDLK_LEFT)
    {
      run = false;
    }
    if (e.key.keysym.sym == SDLK_RIGHT)
    {
      run = false;
    }
    if (e.key.keysym.sym == SDLK_UP)
      notJumped = true;
      run = false;

    if (e.key.keysym.sym == SDLK_DOWN)
      notSkipped = true;
      run = false;
  }
}

void Player::LandTileCollision(LandTileGroup &landTiles){


  SDL_Rect tileCollider;
  // calculate player collider
  collider.x = position.x;
  collider.y = position.y;
  collider.w = dimension.x;
  collider.h = dimension.y;

  for(LandTile &tile : landTiles.container){
   tileCollider.x = tile.position.x;
   tileCollider.y = tile.position.y;
   tileCollider.w = tile.dimension.x;
   tileCollider.h = tile.dimension.y;

   collidedTile = SDL_IntersectRect(&collider, &tileCollider, &result);

   if (collidedTile){
     if (result.w < result.h){
       if (position.x < tile.position.x)
           position.x -= (result.w);
       else if (position.x > tile.position.x)
           position.x += result.w ;
       }
     if (result.h < result.w){
        if (position.y < tile.position.y){
           position.y -= (result.h) ;
           onTile = true;
           jumpCounter = 0;
           }
        }
     break;
     }
  }
}

//Algorithm from LazyFoo
void Player::boulderCollision(Boulder &boulder){

  int x, y;

  if (boulder.collider.x < position.x)
  {
      x = position.x;
  }
  else if (boulder.collider.x > position.x + dimension.x)
  {
      x = position.x + dimension.x;
  }
  else
  {
    x = boulder.collider.x;
  }

  if (boulder.collider.y < position.y)
  {
    y = position.y;
  }
  else if (boulder.collider.y > position.y + dimension.y)
  {
    y = position.y + dimension.y;
  }
  else
  {
    y = boulder.collider.y;
  }

  if( distanceSquared( boulder.collider.x, boulder.collider.y, x, y ) < boulder.collider.r * boulder.collider.r )
    {
        position.x = SCREEN_WIDTH/2;
        position.y = SCREEN_HEIGHT/2;
        onTile = false;
     }
}

void Player::ballCollision(Ball& ball){
  //closest point on collision box
  int x, y;

  if (ball.collider.x < position.x)
  {
      x = position.x;
  }
  else if (ball.collider.x > position.x + dimension.x)
  {
      x = position.x + dimension.x;
  }
  else
  {
    x = ball.collider.x;
  }

  if (ball.collider.y < position.y)
  {
    y = position.y;
  }
  else if (ball.collider.y > position.y + dimension.y)
  {
    y = position.y + dimension.y;
  }
  else
  {
    y = ball.collider.y;
  }

  if( distanceSquared( ball.collider.x, ball.collider.y, x, y ) < ball.collider.r * ball.collider.r )
    {
        ball.position.x = SCREEN_WIDTH + 100;
        ball.position.y = SCREEN_HEIGHT + 100;
        ball.collider.x = ball.position.x + ball.dimension.x;
        ball.collider.y = ball.position.y + ball.dimension.y;
        portalOn = true;
    }
}

void Player::portalCollision(Portal &portal){
  int x, y;

  if (portal.collider.x < position.x)
  {
      x = position.x;
  }
  else if (portal.collider.x > position.x + dimension.x)
  {
      x = position.x + dimension.x;
  }
  else
  {
    x = portal.collider.x;
  }

  if (portal.collider.y < position.y)
  {
    y = position.y;
  }
  else if (portal.collider.y > position.y + dimension.y)
  {
    y = position.y + dimension.y;
  }
  else
  {
    y = portal.collider.y;
  }

  if( distanceSquared( portal.collider.x, portal.collider.y, x, y ) < portal.collider.r * portal.collider.r )
    {
        portalOn = false;
        currentLevel += 1;
        position.x = 100;
        position.y = 50;
    }
}

LandTile::LandTile(double x, double y){
  position.x = x;
  position.y = y;
  dimension.x = landSprite.getWidth();
  dimension.y = landSprite.getHeight();
}

LandTile::~LandTile(){}

void LandTile::render(){
  landSprite.render(position.x,position.y);
}

LandTileGroup::LandTileGroup(float x , float y ){
 if (x == 0 || y == 0){
   for (int i = 0; i <= SCREEN_WIDTH/landSprite.getWidth(); i++){
     container.emplace_back(landSprite.getWidth()*i, SCREEN_HEIGHT-landSprite.getHeight());
   }
 }
 else{
   for(int i = 0; i <= rand() % 3 + 2 ; i++){
     container.emplace_back(x + i*landSprite.getWidth(), y);
   }
 }
}

void LandTileGroup::move(){
 for (auto &tile : container){
   tile.position.x -= 1;
   if (tile.position.x < 0 - tile.dimension.x)
      tile.position.x = SCREEN_WIDTH;
 }
}

void LandTileGroup::moveLand(){
 int movement = -1;
 for (auto &tile : container){
   tile.position.x += movement;
   if(tile.position.x < 0 - tile.dimension.x)
     tile.position.x = SCREEN_WIDTH;
 }
}

void LandTileGroup::render(){
 for (auto &tile : container){
   tile.render();
 }
}

LandTileGroup::~LandTileGroup(){};

Boulder::Boulder(){
  dimension.x = boulderSprite.getWidth();
  dimension.y = boulderSprite.getHeight();
  position.x = 0;
  position.y = SCREEN_HEIGHT - dimension.y - landSprite.getHeight();
  //-4 because of extra 2 pixels on both sides in png file
  collider.r = dimension.x/2 - 4;
  collider.x = position.x + collider.r;
  collider.y = position.y + collider.r;
}

void Boulder::render(){
   if (animationClip == 360)
      animationClip = 0;

   boulderSprite.render(position.x,position.y, nullptr, animationClip);

   animationClip++;
}

Boulder::~Boulder(){}

Ball::Ball()
{
  dimension.x = ballSprite.getWidth();
  dimension.y = ballSprite.getHeight()/7;
  position.x = (SCREEN_WIDTH/2) - (dimension.x/2);
  position.y = (SCREEN_HEIGHT/3);
  collider.r = dimension.x/2;
  collider.x = position.x + collider.r;
  collider.y = position.y + collider.r;
}

void Ball::render()
{
  ballSprite.render(position.x, position.y, &ballSpriteClip[5]);
}

void Ball::move()
{
 if (position.x > SCREEN_WIDTH-dimension.x || position.x < 0){
    velX *= -1;
 }
  position.x += velX;
  position.y = sin(0.08*(position.x) + 1)*50 + SCREEN_HEIGHT/3;

  collider.x = position.x + collider.r;
  collider.y = position.y + collider.r;
}

Ball::~Ball()
{
}

void Ball::moveBernoulli(){
   static double step = 0.1;
   static double framestep = 0.01;
    if (framestep > (2 * M_PI))
       step *= -1;

    double x = 100 * pow(2, 0.5)  * cos(framestep) + SCREEN_WIDTH/2;
    double y = 100 * pow(2, 0.5)  * sin(framestep) * cos(framestep) + SCREEN_HEIGHT/3;


    position.x = x;
    position.y = y;
    collider.x = position.x + collider.r;
    collider.y = position.y + collider.r;
    framestep += step;
}

Portal::Portal(){
   dimension.x = portalSprite.getWidth();
   dimension.y = portalSprite.getHeight();
   position.x = SCREEN_WIDTH - dimension.x;
   position.y = 0;
   angle = 0;
   collider.r = dimension.x/4;
   collider.x = position.x + 2*collider.r;
   collider.y = position.y + 2*collider.r;
}

void Portal::render(){
  if (angle > 360)
     angle = 0;
  portalSprite.render(position.x, position.y, nullptr, angle);
  angle++;
}

Portal::~Portal(){
}

void levelTwo(Player &player, SDL_Event &e){
    static int scrollingOffset = 0;
    static LandTileGroup land(0,0);
    static std::vector<LandTileGroup> randomLand;
    static Boulder boulder;
    static Ball ball;
    static Portal portal;

    if (randomLand.empty()){
      for (int i = 0; i < 4; i++)
       randomLand.emplace_back(SCREEN_WIDTH + rand() % 300 , i * 60 + 32);
    }

    // Movement
    land.moveLand();
    for (auto &tiles : randomLand){
      tiles.move();
    }
    player.position.x -= 1; // Level specific
    player.update();
    player.move(e);
    player.ballCollision(ball);
    player.portalCollision(portal);

    if (!player.portalOn)
    {
      ball.moveBernoulli();
    }
    else
    {
      ball.position.x = -100;
    }

    // Collision detection - resolution
    for (auto &tiles : randomLand){
      player.LandTileCollision(tiles);
    }
    player.boulderCollision(boulder);

    scrollingOffset-= 1;
				if( scrollingOffset < -bg1Sprite.getWidth() )
				{
					scrollingOffset = 0;
				}

 				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				bg1Sprite.render( scrollingOffset, 0 );
				bg1Sprite.render( scrollingOffset + bg1Sprite.getWidth(), 0 );

    // Rendering
    land.render();

    for (auto &tiles : randomLand){
      tiles.render();
    }

    boulder.render();
    ball.render();
    player.render();

    if (player.portalOn == true)
    {
      portal.render();
    }

			//Update screen
      SDL_RenderPresent( gRenderer );
}

void levelOne(Player &player, SDL_Event &e){
  static LandTileGroup a(SCREEN_WIDTH-100,200);
  static Ball ball;
  static Portal portal;
  player.update();
  player.move(e);
  ball.move();
  player.LandTileCollision(a);
  player.ballCollision(ball);
  player.portalCollision(portal);

  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( gRenderer );

  bg2Sprite.render(0,0);
  a.render();
  ball.render();
  player.render();

  if (player.portalOn == true)
  {
    portal.render();
  }

  SDL_RenderPresent(gRenderer);
}

void gameOver(){

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

        switch(player.currentLevel){
         case 1:
          levelOne(player, e);
          break;
         case 2:
          levelTwo(player, e);
          break;
         case 3:
          gameOver();
          break;
        }

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
