#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

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

sf::RenderWindow window (sf::VideoMode(WIDTH,HEIGHT), "Sorta Jousting");
sf::RectangleShape shape(sf::Vector2f(FLOOR_WIDTH,FLOOR_HEIGHT));


struct player{
	sf::RectangleShape rect;
	sf::RectangleShape sword;
	bool animatingDown; // for swords
	bool animatingUp;// for swords
	double vx,vy;
	double cx,cy;
	player(){
		rect.setSize(sf::Vector2f(PLAYER_W,PLAYER_H));
		rect.setPosition(P1_START_POSX,HEIGHT - FLOOR_HEIGHT - PLAYER_H);
		rect.setFillColor(sf::Color::Blue);
		sword.setSize(sf::Vector2f(SWORD_W,SWORD_H));
		sword.setPosition(P1_START_POSX,HEIGHT - FLOOR_HEIGHT - SWORD_INC);
		rect.setPosition(P1_START_POSX,HEIGHT - FLOOR_HEIGHT - PLAYER_H);
		sword.setFillColor(sf::Color::Yellow);
		animatingDown = animatingUp =false;
		vx = PLAYER_VX;
		vy = 0;
	};
	player(float cx, float cy){
	
	};
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
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		//cout << "Player 1 Sword Up";
		moveSwordUp(&p1);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		//cout << "Player 1 Move Left";
		p1.rect.move(-p1.vx,0);
		p1.sword.move(-p1.vx,0);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		//cout << "Player 1 Sword Down";
		moveSwordDown(&p1);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		//cout << "Player 1 Move Right";
		p1.rect.move(p1.vx,0);
		p1.sword.move(p1.vx,0);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
		//cout << "Player 1 Sword Thrust";
		//animate thrust
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::G)){
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

void render(){
	    window.clear();
		window.draw(p1.rect);
		window.draw(p1.sword);
        window.draw(shape);
        window.display();
};

void init(){
	window.setFramerateLimit(FPS);
	shape.setPosition(0,HEIGHT - FLOOR_HEIGHT);
	shape.setFillColor(sf::Color::White);
}

int main()
{
	init();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {	
            if (event.type == sf::Event::Closed)
                window.close();
        }
		handleInput();
		animate();
		collide();
		render();

    }

    return 0;
}