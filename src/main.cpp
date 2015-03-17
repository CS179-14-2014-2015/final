#pragma once

#include <stdio.h>
#include <math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;
//using namespace sf;

const int WIDTH = 1200;
const int HEIGHT = 350;
const int FPS = 60;
const int FLOOR_WIDTH = WIDTH;
const int FLOOR_HEIGHT = 30;
const int P1_START_POSX = 40;
const int P2_START_POSX = FLOOR_WIDTH - 60;
const float PLAYER_W = 25;
const float PLAYER_H = 50;
const float PLAYER_VX = 3.5;
const float SWORD_INC = PLAYER_H/5;
const float SWORD_W = 50;
const float SWORD_H = 5;
const float SWORD_ACC = 0.5;
const double GRAV = 9.8/(FPS*1.0);

sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "Sorta Jousting");
sf::Clock clock;
sf::RectangleShape shape(sf::Vector2f(FLOOR_WIDTH,FLOOR_HEIGHT));

//some references from http://www.gamefromscratch.com/page/Game-From-Scratch-CPP-Edition.aspx

class Node{
protected:
	sf::Vector2f pos, vel, origin;
	sf::Sprite spr;
	sf::Texture txt;
	string filename;
	bool loaded;
public:
	Node() : loaded(false){}
	virtual ~Node(){};

	virtual bool isLoaded() const{
		return loaded;
	}
	virtual void load(string fn){
		if(txt.loadFromFile(fn) == false){
			filename = "";
			loaded = false;
		}else{
			filename = fn;
			spr.setTexture(txt);
			loaded = true;
		}
	}
	virtual void draw(sf::RenderWindow &rw){
		if(loaded){
			rw.draw(spr);
		}
	}
	virtual void update(float t){
		setPos(getVel()*t/FPS);
	}
	virtual void setPos(float x, float y){
		if(loaded){
			pos.x = x;
			pos.y = y;
			spr.setPosition(pos);
		}
	}
	virtual void setVel(float x, float y){
		if(loaded){
			vel.x = x;
			vel.y = y;
		}
	}
	virtual sf::Vector2f getVel(){
		if(loaded){
			return vel;
		}
		return sf::Vector2f();
	}
	virtual sf::Vector2f getPos(){
		if(loaded){
			return pos;
		}
		return sf::Vector2f();
	}
	virtual sf::Sprite& getSprite(){
		return spr;
	}
};

class Player : public Node{
public:
	Player(){
		vel = sf::Vector2f(3,3);
		load("textures/player.png");
		assert(isLoaded());
		getSprite().setScale(PLAYER_W/256,PLAYER_H/512);
		getSprite().setOrigin(getSprite().getScale().x/2, getSprite().getScale().y/2);
	};
	~Player(){};

	void draw(sf::RenderWindow &rw){
		Node::draw(rw);
	}
};

class Floor : public Node{
public:
	Floor(){
		load("textures/floor.png");
		assert(isLoaded());
		getSprite().setScale(WIDTH/512.0f,FLOOR_HEIGHT/512.0f);
		setPos(0,HEIGHT - FLOOR_HEIGHT);
	}
	~Floor(){};

	void draw(sf::RenderWindow &rw){
		Node::draw(rw);
	}
};

class NodeManager{
protected:
	map<string, Node*> nodes;
	
	struct nodeDeallocator{
		void operator()(const pair<string, Node*> &p) const{
			delete p.second;
		}
	};
public:
	NodeManager(){};
	~NodeManager(){
		for_each(nodes.begin(), nodes.end(), nodeDeallocator());
	};

	void add(string name, Node* node){
		nodes.insert(pair<string, Node*>(name, node));
	};
	void remove(string name){
		map<string, Node*>::iterator results = nodes.find(name);
		if(results != nodes.end()){
			delete results->second;
			nodes.erase(results);
		}
	}
	Node* get(string name){
		map<string, Node*>::const_iterator results = nodes.find(name);
		if(results == nodes.end()) return NULL;
		return results->second;
	}
	int getSize(){
		return nodes.size();
	}
	void drawAll(sf::RenderWindow& rw){
		map<string, Node*>::const_iterator itr = nodes.begin();
		while(itr != nodes.end()){
			itr->second->draw(rw);
			itr++;
		}
	}
	void updateAll(){
		map<sstring,Node*>::const_iterator itr = nodes.begin();
		float timeDelta = window.GetFrameTime();

		while(itr != nodes.end()){
			itr->second->update(timeDelta);
			itr++;
		}
	}
};

NodeManager nodeManager;

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

*/

void collision(){
	
}

void input(){
	if(Keyboard::isKeyPressed(Keyboard::A)){
		//cout << "Player 1 Move Left";
		nodeManager.get("Player")->setVel(-p1.vx,0);
	}
	if(Keyboard::isKeyPressed(Keyboard::D)){
		//cout << "Player 1 Move Right";
		nodeManager.get("Player")->setVel(p1.vx,0);
	}
}

void animate(){

}

void logic(){
	collision();
}

void render(){
	window.clear(sf::Color::White);
	nodeManager.drawAll(window);
    window.display();
}

void gameLoop(){
	input();
	animate();
	logic();
	render();
}

void gameInit(){
	nodeManager.add("Floor", new Floor());
	nodeManager.add("Player",new Player());
	nodeManager.get("Player")->setPos(P1_START_POSX,HEIGHT-FLOOR_HEIGHT-PLAYER_H);
}

void sysInit(){
	window.setFramerateLimit(FPS);
}

void init(){
	sysInit();
	gameInit();
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
		gameLoop();
    }
    return 0;
}
