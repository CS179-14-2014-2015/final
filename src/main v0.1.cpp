/*
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;
using namespace sf;
const int WIDTH = 1200;
const int HEIGHT = 350;
const int FPS = 60;
const int FLOOR_WIDTH = WIDTH;
const int FLOOR_HEIGHT = 30;
const int P1_START_POSX = 40;
const int P2_START_POSX = FLOOR_WIDTH - 60;
const float PLAYER_W = 10;
const float PLAYER_H = 50;
const float PLAYER_VX = 3.5;
const float SWORD_INC = PLAYER_H/5;
const float SWORD_W = 50;
const float SWORD_H = 5;
const float SWORD_ACC = 0.5;
const double GRAV = 9.8/(FPS*1.0);

RenderWindow window (VideoMode(WIDTH,HEIGHT), "Sorta Jousting");
RectangleShape shape(Vector2f(FLOOR_WIDTH,FLOOR_HEIGHT));


class Node{
	private:
		Vector2f pos, vel, origin;
	public:
		Node(){};
		Node(double x, double y){
			pos = new Vector2f(x,y);
		};
		~Node(){
			delete pos;
		}
}


/*
struct player{
	RectangleShape rect;
	RectangleShape sword;
	bool animatingDown; // for swords
	bool animatingUp;// for swords
	double vx,vy;
	double cx,cy;
	player(){
		rect.setSize(Vector2f(PLAYER_W,PLAYER_H));
		rect.setPosition(P1_START_POSX,HEIGHT - FLOOR_HEIGHT - PLAYER_H);
		rect.setFillColor(Color::Blue);
		sword.setSize(Vector2f(SWORD_W,SWORD_H));
		sword.setPosition(P1_START_POSX,HEIGHT - FLOOR_HEIGHT - SWORD_INC);
		rect.setPosition(P1_START_POSX,HEIGHT - FLOOR_HEIGHT - PLAYER_H);
		sword.setFillColor(Color::Yellow);
		animatingDown = animatingUp =false;
		vx = PLAYER_VX;
		vy = 0;
	};
	player(float cx, float cy){};

};

void moveSwordDown(player* p){
	if(p->animatingDown)
		return;
	else{
		p->sword.move(0,SWORD_INC);
	}
};

void moveSwordUp(player* p){
	if(p->animatingUp)
		return;
	else{
		p->sword.move(0,-SWORD_INC);
	}
};

player p1;
player p2;

void handleInput(){
	if(Keyboard::isKeyPressed(Keyboard::W)){
		//cout << "Player 1 Sword Up";
		moveSwordUp(&p1);
	}
	if(Keyboard::isKeyPressed(Keyboard::A)){
		//cout << "Player 1 Move Left";
		p1.rect.move(-p1.vx,0);
		p1.sword.move(-p1.vx,0);
	}
	if(Keyboard::isKeyPressed(Keyboard::S)){
		//cout << "Player 1 Sword Down";
		moveSwordDown(&p1);
	}
	if(Keyboard::isKeyPressed(Keyboard::D)){
		//cout << "Player 1 Move Right";
		p1.rect.move(p1.vx,0);
		p1.sword.move(p1.vx,0);
	}
	if(Keyboard::isKeyPressed(Keyboard::F)){
		//cout << "Player 1 Sword Thrust";
		//animate thrust
	}
	if(Keyboard::isKeyPressed(Keyboard::G)){
		//cout << "Player 1 Jump";
		if(p1.rect.getPosition().y < HEIGHT - FLOOR_HEIGHT - PLAYER_H){
		}
		else{
			p1.vy = -5;
		}
	}
};

void animate(){
	if(p1.rect.getPosition().y <= HEIGHT - FLOOR_HEIGHT - PLAYER_H){
		p1.vy += GRAV;
		p1.rect.move(0,p1.vy);
		p1.sword.move(0,p1.vy);
	}

};

void collideWalls(){
	if(p1.rect.getPosition().x <=0){
		p1.rect.setPosition(0,p1.rect.getPosition().y);
	};
	if(p1.rect.getPosition().y > HEIGHT - FLOOR_HEIGHT - PLAYER_H){
		p1.rect.setPosition(p1.rect.getPosition().x,HEIGHT - FLOOR_HEIGHT - PLAYER_H);
		p1.vy = 0;
	};
	if(p1.rect.getPosition().x + PLAYER_W >= WIDTH){
		p1.rect.setPosition( WIDTH - PLAYER_W , p1.rect.getPosition().y);
	}

};

void collide(){
	collideWalls();
}



void collision(){
	
}

void input(){

}

void animate(){

}

void logic(){
	collision();
}

void render(){
	    window.clear();
		window.draw(p1.rect);
		window.draw(p1.sword);
        window.draw(shape);
        window.display();
}

void gameLoop(){
	input();
	animate();
	logic();
	render();
}


void init(){
	window.setFramerateLimit(FPS);
}

int main()
{
	init();

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {	
            if (event.type == sf::Event::Closed)
                window.close();
        }
		gameLoop();
    }
    return 0;
}
*/