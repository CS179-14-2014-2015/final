#include "iostream"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL_timer.h"
#include <vector>
#include <cmath>
#include "SDL2_rotozoom.h"
#include <SDL_image.h>
#include <list>
#include "SDL_keycode.h"
#include "SDL_keyboard.h"
#include <time.h>
#include <string>
using namespace std;
#define Bullet_No 6

const int FRAMERATE = 60;
const double FRAME_TIME = 1000/FRAMERATE;
const float PI = 3.14159265;
const int TIME_IN_SECONDS = 60;
int window_height = 640;
int window_width = 800;
int speed = 10;
int enemyspeed = 5;
int bossspeed = 5;
const int player_width = window_width/16;
const int player_height = window_height/16;
int maxEnemies = 10;
int maxBullets = 7;
int BulletSpeed = 15;

float fRand()
{
	return (float)rand()/RAND_MAX;
}
class Bullet
{

	
public:
	bool alive;
   float x, y;
   float xv, yv;
   float startx, starty;
   float damage;
   Bullet(float startx1, float starty1, float speedx, float speedy, float upgrade)
   {
	   alive = true;
	   x = startx1;
	   y = starty1;
	   startx = startx1;
	   starty = starty1;
	   xv = speedx;
	   yv = speedy;
	   damage = 1*upgrade;
		
   }
   void Draw(float playerx, float playery, SDL_Texture* tex, SDL_Renderer* ren, int size)
   {
		if(alive)
			{
				//do whatever drawing you need in here
				double delta_x = playerx - startx;
				double delta_y = playery - starty;
				SDL_Point center = {player_width/4, player_height/4};
				int angle_deg = (atan2(delta_y, delta_x)*180.0000)/PI;
				SDL_Rect enemy = {playerx-player_width/4, playery-player_height/4, size*(player_width/4) , size*(player_height/4)}; //positioning of thingy
				//SDL_RenderCopy(ren, tex, NULL, &enemy); 
				SDL_RenderCopyEx(ren, tex, NULL, &enemy, angle_deg+90, &center, SDL_FLIP_NONE); 
			}
   };
   //void Drawleft (float playerx, float playery, SDL_Texture* tex, SDL_Renderer* ren, int size
   
 

   
};


class Enemy
{
public:
	bool alive;
   float x, y;
   float xv, yv;
   float startx, starty;
   float speeds;
   float hp;
	Enemy()
	{
		x = fRand()*(window_width);
		y = fRand()*(window_height);
		
		speeds = fRand()*(enemyspeed-1)/2;
		xv = speeds;
		yv = speeds;
		hp = 2;
		alive = true;
	};
	void Draw(float playerx, float playery, SDL_Texture* tex, SDL_Renderer* ren, int size)
	{
		if(alive)
			{
				//do whatever drawing you need in here
				double delta_x = x - playerx;
				double delta_y = y - playery;
				SDL_Point center = {player_width/2, player_height/2};
				int angle_deg = (atan2(delta_y, delta_x)*180.0000)/PI;
				SDL_Rect enemy = {x - player_width/2, y - player_width/2, player_width, player_height}; //positioning of thingy
				//SDL_RenderCopy(ren, tex, NULL, &enemy); 
				SDL_RenderCopyEx(ren, tex, NULL, &enemy, angle_deg, &center, SDL_FLIP_NONE); 
			}
   };
};
class GameObject
{
	public:
	bool alive;
   float x, y;
   
   
   GameObject()
   {
	   x = fRand()*(window_width);
		y = fRand()*(window_height);
		
		
		alive = true;
   };
   void Draw(SDL_Texture* tex, SDL_Renderer* ren)
	{
		
				//do whatever drawing you need in here
			
			SDL_Rect enemy = {x - player_width/2, y - player_width/2, player_width,player_height}; //positioning of thingy
			//SDL_RenderCopy(ren, tex, NULL, &enemy); 
			SDL_RenderCopy(ren, tex, NULL, &enemy); 
			
   };
   
};

class Hero
{
	public:
	bool alive;
   float x, y;
   float hp;
   
   Hero()
   {
	   x = window_width/2;
	   y = window_height/2;
		
		hp = 60;
		alive = true;
   };
   void Draw(int mouseposx, int mouseposy, SDL_Renderer *ren, SDL_Texture *tex)
	{
	
		double delta_x = x - mouseposx;
		double delta_y = y - mouseposy;
		int angle_deg = (atan2(delta_y, delta_x)*180.0000)/PI;
		SDL_Point center = {player_width/2, player_height/2};
		SDL_Rect players = {x - player_width/2, y -player_height/2, player_width, player_height};
		SDL_RenderCopyEx(ren, tex, NULL, &players, angle_deg- 180, &center, SDL_FLIP_NONE);
	}
   
};

class Boss
{
public:
	bool alive;
   float x, y;
   float xv, yv;
   float startx, starty;
   float speeds;
   float hp;
	Boss(float difficulty, float speedinc)
	{
		x = fRand()*(window_width);
		y = fRand()*(window_height);
		
		speeds = speedinc + bossspeed;
		xv = speeds;
		yv = speeds;
		hp = 50 * difficulty;
		alive = true;
	};
	void Draw(float playerx, float playery, SDL_Texture* tex, SDL_Renderer* ren, int size)
	{
		if(alive)
			{
				//do whatever drawing you need in here
				double delta_x = x - playerx;
				double delta_y = y - playery;
				SDL_Point center = {player_width/2, player_height/2};
				int angle_deg = (atan2(delta_y, delta_x)*180.0000)/PI;
				SDL_Rect enemy = {x - 98/2, y - 131/2, 98, 131}; //positioning of thingy
				//SDL_RenderCopy(ren, tex, NULL, &enemy); 
				SDL_RenderCopyEx(ren, tex, NULL, &enemy, angle_deg, &center, SDL_FLIP_NONE); 
			}
   };
};
list<GameObject*> bombs;
list<Enemy*> enemies;
list<GameObject*> weapons;
list<GameObject*> bigweapons;
list<Boss*> boss;
Hero goodguy;




struct bullet
{
	int positionx, positiony, speedx, speedy, directionx, directiony;
};

list<Bullet*> bullets;


struct enemy {
	float x, y, vx, vy, speeds;
};
//vector<enemy> enemies (maxEnemies);
//struct hero{
//	int x, y, vy, roto, startx, starty, centerx, centery;
//};
//hero player;
//void generateBullet(int mouseposx, int mouseposy, int count, SDL_Renderer *ren, SDL_Surface *surf)
//{
//	int distance = 0;
//	for (int x = 0; x< maxBullets; x++)
//	{
//	
//	
//	bullets[x].directionx = mouseposx;
//	bullets[x].directiony = mouseposy;
//	bullets[x].speedx = speed*2;
//	bullets[x].speedy = speed*2;
//	bullets[x].positionx = player.startx;
//	bullets[x].positiony = player.starty + distance;
//	distance += 50;
//	//bullets.push_back(toShoot);
//	}
//}
//void generateHero()
//{
//	player.x = player_width/2;
//	player.y = player_height/2;
//	player.vy = 5;
//	player.roto = 1.5;
//	player.startx = window_width/2;
//	player.starty = window_height/2;
//	player.centerx = player.startx - (player_width/2);
//	player.centery = player.starty - (player_height/2);
//	
//}

//void generateEnemy()
//{
//	
//	for (int i = 0; i < maxEnemies; i++)
//	{
//		//cout << fRand() << " ";
//		enemies[i].x = fRand()*(window_width);
//		enemies[i].y = fRand()*(window_height);
//		enemies[i].vx = 1;
//		enemies[i].vy = 1;
//		enemies[i].speeds = fRand()*(enemyspeed-2)/2;
//		if (enemies[i].speeds <= enemyspeed/10)
//		{
//			enemies[i].speeds = 5;
//		}
//		//cout << bullets[i].x << " " << bullets[i].y << endl;
//	}
//}
//void drawEnemies(SDL_Renderer *ren, SDL_Texture *tex)
//{
//	for (int i = 0; i < maxEnemies; i++)
//	{
//		double delta_x = enemies[i].x - player.startx;
//		double delta_y = enemies[i].y - player.starty;
//		int angle_deg = (atan2(delta_y, delta_x)*180.0000)/PI;
//		SDL_Point center = {player_width/4, player_height/4};
//		SDL_Rect enemy = {enemies[i].x, enemies[i].y, player_width/2, player_height/2}; //positioning of thingy
//		SDL_RenderCopyEx(ren, tex, NULL, &enemy, angle_deg - 90, &center, SDL_FLIP_NONE); //texturization of thingy
//	}
//	
//}
double dotproduct( float vec1x, float vec1y, float vec2x, float vec2y)
{
	return ((vec1x*vec2x)+(vec2y*vec1y));
}
double maximize(float hhx, float hhy, float hwx, float hwy, float projaxisx, float projaxisy)
{
	double a = dotproduct(hhx, hhy, projaxisx, projaxisy) + dotproduct(hwx, hwy, projaxisx, projaxisy);
	double b = -dotproduct(hhx, hhy, projaxisx, projaxisy) - dotproduct(hwx, hwy, projaxisx, projaxisy);
	if (b >= a)
	{
		a = b;
	}

	b = -dotproduct(hhx, hhy, projaxisx, projaxisy) + dotproduct(hwx, hwy, projaxisx, projaxisy);
	if (b >=a)
	{
		a = b;
	}

	b = +dotproduct(hhx, hhy, projaxisx, projaxisy) - dotproduct(hwx, hwy, projaxisx, projaxisy);
	if (b>=a)
	{
		a = b;
	}
	return a;
}

//void drawPlayer(int mouseposx, int mouseposy, SDL_Renderer *ren, SDL_Texture *tex)
//{
//	
//	double delta_x = player.startx - mouseposx;
//	double delta_y = player.starty - mouseposy;
//	int angle_deg = (atan2(delta_y, delta_x)*180.0000)/PI;
//	SDL_Point center = {player.x, player.y};
//	SDL_Rect players = {player.startx - player.x, player.starty - player.y, player_width, player_height};
//	SDL_RenderCopyEx(ren, tex, NULL, &players, angle_deg- 180, &center, SDL_FLIP_NONE);
//}
SDL_Texture *loadTexture(SDL_Renderer *ren,string path)
{
	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
		string i;
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		cin >> i;
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(ren, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

	return newTexture;
}

void collisionOnWindow()
{
	if (goodguy.y + player_height/2 >= window_height)
	{
		goodguy.y = window_height - player_height/2;
	}
	if (goodguy.x + player_width/2 >= window_width)
	{
		goodguy.x = window_width - player_width/2;
	}
	if (goodguy.x - player_width/2 <= 0)
	{
		goodguy.x = player_width/2;
	}
	if (goodguy.y - player_height/2 <= 0)
	{
		goodguy.y = player_height/2;
	}
}


int advanceBulletsx(int playerx, int playery, int mouseposx, int mouseposy, int speedx, int speedy)
{
	
		float xDistance = mouseposx-playerx;
		float yDistance = mouseposy-playery;
		float hypotenuse = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
		//float angle = atan2(xDistance, yDistance)/PI;
		
			int bulletposx = speedx*(xDistance/hypotenuse);
			int bulletposy = speedy*(yDistance/hypotenuse);
		return bulletposx;
		
}
int advanceBulletsy(int playerx, int playery, int mouseposx, int mouseposy, int speedx, int speedy)
{
	
		float xDistance = mouseposx-playerx;
		float yDistance = mouseposy-playery;
		float hypotenuse = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
		

			int bulletposx = speedx*(xDistance/hypotenuse);
			int bulletposy = speedy*(yDistance/hypotenuse);
		return bulletposy;
		//bullets[i].positiony -= bullets[i].speedy;
	
}



int advanceEnemiesx(int playerposx, int playerposy, int enemiesx, int enemiesy, int speedx, int speedy)
{
	 
	
		float xDistance = playerposx-enemiesx;
		float yDistance = playerposy-enemiesy;
		float hypotenuse = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
		

			int enemyposx = speedx*(xDistance/hypotenuse);
			int enemyposy = speedy*(yDistance/hypotenuse);
			return enemyposx;
		
		
	
	
}
int advanceEnemiesy(int playerposx, int playerposy, int enemiesx, int enemiesy, int speedx, int speedy)
{
	 
	
		float xDistance = playerposx-enemiesx;
		float yDistance = playerposy-enemiesy ;
		float hypotenuse = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
		

			int enemyposx = speedx*(xDistance/hypotenuse);
			int enemyposy = speedy*(yDistance/hypotenuse);
			return enemyposy;
		
		
	
	
} 
void advanceEnemies(int playerposx, int playerposy)
{
	for(list<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
		{
			float xDistance = playerposx-(*it)->x;
			float yDistance = playerposy-(*it)->y;
			float hypotenuse = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
		

			(*it)->xv = (*it)->speeds*(xDistance/hypotenuse);
			(*it)->yv = (*it)->speeds*(yDistance/hypotenuse);
			/*(*it)->xv = advanceBulletsx(player.startx, player.starty, mx, my, (*it)->xv, (*it)->yv);
			(*it)->yv = advanceBulletsy(player.startx, player.starty, mx, my, (*it)->xv, (*it)->yv);*/
			/*(*it)->x += advanceBulletsx(player.startx, player.starty, m(*it)->speeds;
			(*it)->y += (*it)->speeds;*/
			
			(*it)->x += (*it)->xv;
			(*it)->y += (*it)->yv;
			//(*it)->Draw((*it)->x, (*it)->y, bullet, ren);
			//if(((*it)->x) >= window_width || ((*it)->x) <= 0 || ((*it)->y) >= window_height || ((*it)->y) <= 0 )
			//{
			//	delete (*it);
			//	enemies.erase(it++); //remove from the list and take next
			//	
			//}
			
		
			
				++it; //take next object
				
			
		
			
		}
}
void advanceBoss(int playerposx, int playerposy)
{
	for(list<Boss*>::iterator it = boss.begin(); it != boss.end();)
	{
		float xDistance = playerposx-(*it)->x;
		float yDistance = playerposy-(*it)->y;
		float hypotenuse = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
		

		(*it)->xv = (*it)->speeds*(xDistance/hypotenuse);
		(*it)->yv = (*it)->speeds*(yDistance/hypotenuse);
		/*(*it)->xv = advanceBulletsx(player.startx, player.starty, mx, my, (*it)->xv, (*it)->yv);
		(*it)->yv = advanceBulletsy(player.startx, player.starty, mx, my, (*it)->xv, (*it)->yv);*/
		/*(*it)->x += advanceBulletsx(player.startx, player.starty, m(*it)->speeds;
		(*it)->y += (*it)->speeds;*/
			
		(*it)->x += (*it)->xv;
		(*it)->y += (*it)->yv;
		//(*it)->Draw((*it)->x, (*it)->y, bullet, ren);
		//if(((*it)->x) >= window_width || ((*it)->x) <= 0 || ((*it)->y) >= window_height || ((*it)->y) <= 0 )
		//{
		//	delete (*it);
		//	enemies.erase(it++); //remove from the list and take next
		//	
		//}
			
		
			
			++it; //take next object
				
			
		
			
	}
}
void checkCollision()
{
	//wall bounce
	
		list <Enemy*>::iterator it;
		list<Enemy*>::iterator ie;
		for( it = enemies.begin(); it != enemies.end();)
		{
			if ((*it)->x < player_width/4)
			{
				(*it)->xv *= -1.00000;
				(*it)->x = player_width/4;
			}
			if ((*it)->x > window_width - player_width/4)
			{
				(*it)->xv *= -1.00000;
				(*it)->x = window_width - player_width/4;
			}
			if ((*it)->y < player_height/4)
			{
				(*it)->yv *= -1.00000;
				(*it)->y = player_height/4;
			}
			if ((*it)->y > window_height - player_height/4)
			{
				(*it)->yv *= -1.00000;
				(*it)->y = window_height - player_height/4;
			}
			//list<Enemy>& listentry = *it;
			for( ie = enemies.begin(); ie != enemies.end(); )
			{
				if (ie != it)
				{
				const float dx = (*it)->x - (*ie)->x,dy = (*it)->y - (*ie)->y;
				const float mag2 = dx * dx + dy * dy;
				const float minD = 2*(player_width/2);

					if ( mag2 <= minD * minD ) 
					{
						const float mag = sqrt(mag2);
						const float ndx = dx/mag, ndy = dy/mag;

						(*it)->x = (minD + 0.00000) * ndx + (*ie)->x;
						(*it)->y = (minD + 0.00000) * ndy + (*ie)->y;
			
						const float proj = -2 * (ndx * (*it)->xv + ndy * (*it)->yv);
						const float proj2 = -2 * (ndx * (*ie)->xv + ndy * (*ie)->yv);
			
						(*it)->xv += proj * ndx;
						(*it)->yv += proj * ndy;
						(*ie)->xv += proj2 * ndx;
						(*ie)->yv += proj2 * ndy;
					}
				}
				++ie;
				
			}
			++it;
			
			
		}
	
}
void checkBossCollision()
{
	//wall bounce
	
		list <Boss*>::iterator it;
		list<Boss*>::iterator ie;
		for( it = boss.begin(); it != boss.end();)
		{
			//if ((*it)->x < 98)
			//{
			//	(*it)->xv *= -1.00000;
			//	(*it)->x = player_width/4;
			//}
			//if ((*it)->x > window_width - 98)
			//{
			//	(*it)->xv *= -1.00000;
			//	(*it)->x = window_width - 98;
			//}
			//if ((*it)->y < 131)
			//{
			//	(*it)->yv *= -1.00000;
			//	(*it)->y = player_height/4;
			//}
			//if ((*it)->y > window_height - 131)
			//{
			//	(*it)->yv *= -1.00000;
			//	(*it)->y = window_height - 131;
			//}
			////list<Enemy>& listentry = *it;
			for( ie = boss.begin(); ie != boss.end(); )
			{
				if (ie != it)
				{
				const float dx = (*it)->x - (*ie)->x,dy = (*it)->y - (*ie)->y;
				const float mag2 = dx * dx + dy * dy;
				const float minD = 2*(98);

					if ( mag2 <= minD * minD ) 
					{
						const float mag = sqrt(mag2);
						const float ndx = dx/mag, ndy = dy/mag;

						(*it)->x = (minD + 0.00000) * ndx + (*ie)->x;
						(*it)->y = (minD + 0.00000) * ndy + (*ie)->y;
			
						const float proj = -2 * (ndx * (*it)->xv + ndy * (*it)->yv);
						const float proj2 = -2 * (ndx * (*ie)->xv + ndy * (*ie)->yv);
			
						(*it)->xv += proj * ndx;
						(*it)->yv += proj * ndy;
						(*ie)->xv += proj2 * ndx;
						(*ie)->yv += proj2 * ndy;
					}
				}
				++ie;
				
			}
			++it;
			
			
		}
	
}
bool collisionOnEnemy()
{
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
	{
			const float dx = (*it)->x - goodguy.x,dy = (*it)->y - goodguy.y;
			const float mag2 = dx * dx + dy * dy;
			const float minD = player_width/2 + player_width/2;

			if ( mag2 <= minD * minD ) 
			{
				goodguy.hp  -= 1;
				//(*ie)->alive = false;
								
						return true;
						
			}
			++it;
	}
	return false;
}

bool collisionOnBoss()
{
	for (list<Boss*>::iterator it = boss.begin(); it != boss.end();)
	{
			const float dx = (*it)->x - goodguy.x,dy = (*it)->y - goodguy.y;
			const float mag2 = dx * dx + dy * dy;
			const float minD = 60 + player_width/2;

			if ( mag2 <= minD * minD ) 
			{
				goodguy.hp  -= 1;
				//(*ie)->alive = false;
						
				return true;
						
						
			}
			++it;
	}
	return false;
}
void checkObjectCollision(list<GameObject*> wew)
{
		list<GameObject*>::iterator it;
	
		for (it = wew.begin(); it != wew.end();)
		{
			const float dx = (*it)->x - goodguy.x,dy = (*it)->y - goodguy.y;
			const float mag2 = dx * dx + dy * dy;
			const float minD = player_width/2 + player_width/2;

			if ( mag2 <= minD * minD ) 
			{
				(*it)->alive = false;
				//(*ie)->alive = false;
								
						
						
			}
			
			++it;
			
		}
		
	
}
void checkBossCollision(int size)
{
	list<Boss*>::iterator it;
	list<Bullet*>::iterator ie;
	for (ie = bullets.begin(); ie!= bullets.end();)
	{
		for (it = boss.begin(); it != boss.end();)
		{
			const float dx = (*it)->x - (*ie)->x,dy = (*it)->y - (*ie)->y;
			const float mag2 = dx * dx + dy * dy;
			const float minD = 60 + player_width/4*size;

			if ( mag2 <= minD * minD ) 
			{
				(*it)->hp -= 1 * size;
				cout << "Boss life: " << (*it)->hp << "\n";
				(*ie)->alive = false;
								
						
						
			}
			
			++it;
			
		}
		++ie;
	}
}
void checkEnemyCollision(int size)
{
	list<Enemy*>::iterator it;
	list<Bullet*>::iterator ie;
	for (ie = bullets.begin(); ie!= bullets.end();)
	{
		for (it = enemies.begin(); it != enemies.end();)
		{
			const float dx = (*it)->x - (*ie)->x,dy = (*it)->y - (*ie)->y;
			const float mag2 = dx * dx + dy * dy;
			const float minD = player_width/2 + player_width/4*size;

			if ( mag2 <= minD * minD ) 
			{
				(*it)->alive = false;
				(*ie)->alive = false;
								
						
						
			}
			
			++it;
			
		}
		++ie;
	}
}

//vector<bullet> bullets (maxBullets);
//http://wiki.libsdl.org/APIByCategory

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int mx, my;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Run this", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 640, SDL_WINDOW_SHOWN);
	if (win == nullptr){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_Surface *screenSurface = SDL_GetWindowSurface(win);
	SDL_Texture *ship = loadTexture(ren, "C:\\Users\\Marty\\Documents\\Visual Studio 2010\\Projects\\SDL4\\SDL4\\swattdss.png");
	SDL_Texture *bullet = loadTexture(ren, "C:\\Users\\Marty\\Documents\\Visual Studio 2010\\Projects\\SDL4\\SDL4\\raiden.png");
	SDL_Texture *upgradeweapon = loadTexture(ren, "C:\\Users\\Marty\\Documents\\Visual Studio 2010\\Projects\\SDL4\\SDL4\\upgradeweapon.png");
	SDL_Texture *bigweapon = loadTexture(ren, "C:\\Users\\Marty\\Documents\\Visual Studio 2010\\Projects\\SDL4\\SDL4\\bigweapon.png");
	/*SDL_Texture 
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, ship);*/
	//SDL_FreeSurface(ship);
	
	/*SDL_Surface *damShip = SDL_LoadBMP("raiden.bmp");
	SDL_Texture *damTex = SDL_CreateTextureFromSurface(ren, damShip);
	SDL_FreeSurface(damShip);*/
	//generateHero();
	//generateEnemy();
	int fps;
	SDL_PumpEvents();
	int last_bullet_time = 0;
	int bullet_interval = 500;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	bool running = true;
	double t =0 ;
	double dt = 0.1;
	int bulletcount = 0;
	int last_bombspawn_time = 0;
	int bomb_interval = 7500;
	int lastspawntime = 0;
	int spawn_interval = 1100;
	int upgrade_interval = 15000;
	int last_upgrade_time = 0;
	int killcount = 0;
	int enemiescount = 10;
	int diff = 1;
	int size = 1;
	int upgradebig_interval = 30000;
	int last_big_time = 0;
	int bosssize = 0;
	int bosscount = 0;
	int last_damage_time = 0;
	int damage_interval = 3300;
	float speedinc = 1;
	float boss_interval = 50;
	bool warned = false;
	bool reallyhard = false;
	bool reallyreallyhard = false;
	//generateBullet(window_width/2, window_height, bulletcount, ren, screenSurface);
	while (running) {
		
		
		int frameTimeStart = SDL_GetTicks();
		if (ship == NULL)
		{
			return 1;
		}
		int frame_time_start = SDL_GetTicks();
		t+=dt;
		SDL_Event ev;
		const auto start = SDL_GetTicks();
		SDL_GetMouseState(&mx, &my);
		
		/*while(SDL_PollEvent(&ev) != 0) 
		{*/
			SDL_SetRenderDrawColor(ren,0,0,0,0);
		
		if (SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
			{
				running = false;
			}
			if (ev.type == SDL_MOUSEBUTTONDOWN)
            {
                /* If the left button was pressed. */
                if (ev.button.button == SDL_BUTTON_LEFT)
				{
					
				}

			}

		}
		if (SDL_GetTicks() >= 1000)
		{
		/*if (keys[SDLK_ESCAPE])
		{
			running = false;
		}*/
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			running = false;
		}
		if (keys[SDL_SCANCODE_A] ||keys[SDL_SCANCODE_LEFT])
		{
			goodguy.x -= speed;
		}
		if (keys[SDL_SCANCODE_W] ||keys[SDL_SCANCODE_UP])
		{
			goodguy.y -= speed;
		}
		if (keys[SDL_SCANCODE_D] ||keys[SDL_SCANCODE_RIGHT])
		{
			goodguy.x += speed;
		}
		if (keys[SDL_SCANCODE_S] ||keys[SDL_SCANCODE_DOWN])
		{
			goodguy.y += speed;
		}

		SDL_RenderClear(ren);
		if (keys[SDL_SCANCODE_SPACE])
		{
			
		}

		//SDL_FillRect(screenSurface, NULL, 0xFFFFFFFF);
		if(last_bullet_time + bullet_interval <= SDL_GetTicks())
			{
				last_bullet_time = SDL_GetTicks();
				Bullet* toShoot = new Bullet(goodguy.x, goodguy.y, advanceBulletsx(goodguy.x,goodguy.y, mx, my, speed, speed), advanceBulletsy(goodguy.x, goodguy.y, mx, my, speed, speed), size);
				
				
				bullets.push_back(toShoot);
				Bullet* toShoot1 = new Bullet(goodguy.x, goodguy.y, advanceBulletsx(goodguy.x,goodguy.y, mx, my  , speed, speed) , advanceBulletsy(goodguy.x, goodguy.y, mx, my, speed, speed) , size);
				bullets.push_back(toShoot1);
				Bullet* toShoot2 = new Bullet(goodguy.x, goodguy.y, advanceBulletsx(goodguy.x,goodguy.y, mx , my, speed, speed) , advanceBulletsy(goodguy.x, goodguy.y, mx, my , speed, speed) , size);
				bullets.push_back(toShoot2);
					//filledCircleColor(ren, mx, my, 100, 0xFA00FFFF); 
					
			}
		if (upgrade_interval + last_upgrade_time <= SDL_GetTicks() && bullet_interval >= 0)
		{
			last_upgrade_time  = SDL_GetTicks();
			GameObject* weapon = new GameObject();
			weapons.push_back(weapon);
		}
		if (bomb_interval + last_bombspawn_time <= SDL_GetTicks())
		{
			last_bombspawn_time = SDL_GetTicks();
			GameObject* bomb = new GameObject();
			bombs.push_back(bomb);
			
		}
		if (upgradebig_interval + last_big_time <= SDL_GetTicks() && size <= 3)
		{
			last_big_time = SDL_GetTicks();
			
			GameObject* big = new GameObject();
			bigweapons.push_back(big);

		}
		if (enemies.size() <= enemiescount && lastspawntime + spawn_interval <= SDL_GetTicks())
		{
			lastspawntime = SDL_GetTicks();
				Enemy* toSpawn = new Enemy();
				enemies.push_back(toSpawn);
				if(diff >=3)
				{
					Enemy* toSpawn1 = new Enemy();
					enemies.push_back(toSpawn1);
					if (spawn_interval <= 500)
					{
						Enemy* toSpawn2 = new Enemy();
						enemies.push_back(toSpawn2);
					}
				}
		}
		if (killcount >= enemiescount && bosscount<1)
		{
			spawn_interval /= 1.2;
			enemiescount += enemiescount;
		}
		if (killcount == (boss_interval*diff) - 10 && warned == false)
		{
			cout << "BOSS IMMINENT" << "\n";
			cout << "Next Boss HP is " << 50*diff << "\n";
			cout << "Next Boss Speed is " << speedinc*bossspeed << "\n";
			warned = true;
		}
		if (killcount >= boss_interval*diff /*&& bosscount < 1*/)
		{
			enemyspeed += 1.7*diff;
			Boss* bosses = new Boss(diff, speedinc);
			boss.push_back(bosses);
			if(bossspeed + speedinc < 9)
			{
				speedinc += 0.4;

			}
			diff+=1.5;
			warned = false;
			bosscount++;
			

		}
		if (diff > 3 && reallyhard == false)
		{
			reallyhard = true;
			damage_interval -= 1000;
			spawn_interval -= 100;
			cout << "HELL MODE ACTIVATED" << "\n";
			Boss* bosses = new Boss(diff, speedinc);
			boss.push_back(bosses);
			bomb_interval -= 1500;
			upgrade_interval -= 1800;
			//boss_interval -= 20;
		}
		if (diff > 5 && reallyreallyhard == false)
		{
			reallyreallyhard = true;
			damage_interval -= 1000;
			spawn_interval -=100;
			bomb_interval -= 1500;
			upgrade_interval -= 5000;
			cout << "SUDDEN DEATH ACTIVATED" << "\n";
			Boss* bosses = new Boss(diff, speedinc);
			boss.push_back(bosses);
			Boss* bosses1 = new Boss(diff, speedinc);
			boss.push_back(bosses1);
			//boss_interval -= 0;
		}
		if (goodguy.hp <= 0)
		{
			SDL_Quit();
		}
		advanceBoss(goodguy.x, goodguy.y);
		advanceEnemies(goodguy.x, goodguy.y);
		checkObjectCollision(bombs);
		checkObjectCollision(weapons);
		checkObjectCollision(bigweapons);
		
		checkEnemyCollision(size);
		//checkBossCollision();
		checkBossCollision(size);
		if (last_damage_time + damage_interval <= SDL_GetTicks() && (collisionOnBoss() == true || collisionOnEnemy() == true))
		{
			last_damage_time = SDL_GetTicks();
			
			cout << "Health: "<< goodguy.hp << "\n";
		}
		
		checkCollision();
		for(list<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
		{
			
			
			(*it)->Draw((*it)->x, (*it)->y, bullet, ren, 1);
			if ((*it)->alive == false)
			{
				delete (*it);
				enemies.erase(it++);
				killcount++;
				cout << "Kill Count: "<< killcount << "\n";

			}
			else
			{
				++it;
			}
			
			
		
			
		}
		for(list<Boss*>::iterator it = boss.begin(); it != boss.end();)
		{
			
			
			(*it)->Draw((*it)->x, (*it)->y, bullet, ren, 1);
			if ((*it)->hp <= 0)
			{
				delete (*it);
				boss.erase(it++);
				killcount++;
				cout << "Kill Count: "<< killcount << "\n";

			}
			else
			{
				++it;
			}
			
			
		
			
		}
		for(list<GameObject*>::iterator it = bombs.begin(); it != bombs.end();)
		{
			(*it)->Draw(ship, ren);
			if ((*it)->alive == false)
			{
				for(list<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
				{
					enemies.erase(it++);
					killcount++;
					cout << "Kill Count: "<< killcount << "\n";
					if (killcount == (boss_interval*diff) - 10 && warned == false)
					{
						cout << "BOSS IMMINENT" << "\n";
						cout << "Next Boss HP is " << 50*diff << "\n";
						cout << "Next Boss Speed is " << speedinc+bossspeed << "\n";
						warned = true;
					}
				}
				
				for (list<Boss*>::iterator it = boss.begin(); it != boss.end();)
				{
					(*it)->hp -= 50;
					cout << "Boss life: " << (*it)->hp << "\n";
					++it;
				}
				delete (*it);
				bombs.erase(it++);
			}
			else
			{
				++it;
			}
			
		}
		for(list<GameObject*>::iterator it = weapons.begin(); it != weapons.end();)
		{
			(*it)->Draw(upgradeweapon, ren);
			if ((*it)->alive == false)
			{
				bullet_interval /= 1.15;
				delete (*it);
				weapons.erase(it++);
				cout << "BulletSpeed: "<< bullet_interval << "\n";
			}
			else
			{
				++it;
			}
			
		}
		
		for(list<GameObject*>::iterator it = bigweapons.begin(); it != bigweapons.end();)
		{
			(*it)->Draw(bigweapon, ren);
			if ((*it)->alive == false)
			{
				size += 1;
				delete (*it);
				bigweapons.erase(it++);
				cout << "size: "<< size<< "\n";
			}
			else
			{
				++it;
			}
			
		}
		for(list<Bullet*>::iterator it = bullets.begin(); it != bullets.end();)
		{
			/*(*it)->xv = advanceBulletsx(player.startx, player.starty, mx, my, (*it)->xv, (*it)->yv);
			(*it)->yv = advanceBulletsy(player.startx, player.starty, mx, my, (*it)->xv, (*it)->yv);*/
			
			(*it)->x += (*it)->xv;
			(*it)->y += (*it)->yv;
			//cout<< (*it)->x << " " << (*it)->y << "\n";
			(*it)->Draw((*it)->x, (*it)->y, bullet, ren, size);
			if(((*it)->x) >= window_width || ((*it)->x) <= 0 || ((*it)->y) >= window_height || ((*it)->y) <= 0 || (*it)->alive == false )
			{
				delete (*it);
				bullets.erase(it++); //remove from the list and take next
				
			}
		
			else
			{
				++it;
			}
		}
		
		
			
			double wew = 10;
			
			collisionOnWindow();
		
			
			//drawPlayer(mx, my, ren, ship);
		
			goodguy.Draw(mx, my, ren, ship);
			
			SDL_RenderPresent(ren);
			/*if (SDL_GetTicks() < 1000/FRAMERATE)
			{
				  SDL_Delay( ( 1000 / FRAMERATE) - SDL_GetTicks() );
			}*/
			if ((FRAME_TIME - (SDL_GetTicks() - frameTimeStart)) > 0)
			{
				SDL_Delay(FRAME_TIME - (SDL_GetTicks() - frameTimeStart));
			}
			
		
		//SDL_Delay(2000);
	}
	}

	//SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
