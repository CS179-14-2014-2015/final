#pragma once

#include <stdio.h>
#include <math.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;
//using namespace sf;

const int WIDTH = 1200;
const int HEIGHT = 400;
const int FPS = 60;
const float TFPS = 100.0f/FPS;
const int FLOOR_WIDTH = WIDTH;
const int FLOOR_HEIGHT = 30;
const int P1_START_POSX = 40;
const int P2_START_POSX = WIDTH - 60;
const int TILE_SIZE = 25;
const float PLAYER_W = 25;
const float PLAYER_H = 50;
const float PLAYER_VX = 3.5;
const float SWORD_INC = PLAYER_H/5;
const float SWORD_W = 50;
const float SWORD_H = 5;
const float SWORD_ACC = 0.5;
const double GRAV = 9.8/(FPS*1.0);

sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "Sorta Jousting");
sf::Clock gameClock;
sf::Time startTime;
sf::RectangleShape shape(sf::Vector2f(FLOOR_WIDTH,FLOOR_HEIGHT));

//some references from http://www.gamefromscratch.com/page/Game-From-Scratch-CPP-Edition.aspx

class Node{
protected:
	sf::Vector2f pos, vel, origin;
	sf::Sprite spr;
	sf::Texture txt;
	string filename;
	bool loaded;
	bool cstatic;
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
			txt.setSmooth(true);
			spr.setTexture(txt);
			loaded = true;
		}
	}
	virtual void draw(sf::RenderWindow &rw){
		if(loaded){
			rw.draw(spr);
		}
	}
	virtual void update(){
		if(loaded){
			setPos(pos.x + vel.x*TFPS, pos.y + vel.y*TFPS);
		}
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
	virtual float getHeight() const{
		return spr.getScale().y;
	}
	virtual float getWidth() const{
		return spr.getScale().x;
	}
	virtual sf::Rect<float> getBoundingRect() const{
		sf::Vector2f size = spr.getScale();
		
		return sf::Rect<float>(
			pos.x - size.x/2,
			pos.y - size.y/2,
			pos.x + size.x/2,
			pos.y + size.y/2
			);
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
	virtual bool isColliding(Node* n){
		//collision test here raffeh
		//collision test here raffeh
		//collision test here raffeh
		return false;
	}
	virtual void collide(Node* n){
		//collision reaction here raffeh
		//collision reaction here raffeh
		//collision reaction here raffeh
	}
};

class Player : public Node{
public:
	Player(){
		load("textures/player.png");
		assert(isLoaded());
		getSprite().setScale(PLAYER_W/256,PLAYER_H/512);
		getSprite().setOrigin(getSprite().getScale().x/2, getSprite().getScale().y/2);
	};
	~Player(){};

	void draw(sf::RenderWindow& rw){
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
		cstatic = true;
	}
	~Floor(){};

	void draw(sf::RenderWindow& rw){
		Node::draw(rw);
	}
};

class Wall : public Node{
public:
	enum Wall_Type{
		Top, Left, Right
	};
	Wall(){};
	Wall(Wall_Type wt){
		switch(wt){
		case Top:
			spr.setScale(WIDTH*1.0f,5.0f);
			setPos(0,0);
			break;
		case Left:
			spr.setScale(5.0f,HEIGHT*1.0f);
			setPos(0,0);
			break;
		case Right:
			spr.setScale(5.0f,HEIGHT*1.0f);
			setPos(WIDTH-5.0f,0);
			break;
		default:
			break;
		}
		cstatic = true;
	};
	~Wall(){};

	void draw(sf::RenderWindow& rw){
		Node::draw(rw);
	}
};

class Tile : public Node{
	Tile(){
		//put tile stuff here uncomment 2 lines below if may texture na
		//load("textures/floor.png");
		//assert(isLoaded());
	}
	Tile(int x, int y){
		pos.x = x;
		pos.y = y;
		//put tile stuff here uncomment 2 lines below if may texture na
		//load("textures/floor.png");
		//assert(isLoaded());
	}
	~Tile(){};
	
	void draw(sf::RenderWindow& rw){
		Node::draw(rw);
	}
};

class NodeManager{
protected:
	map<string, Node*> nodes;
	map<Node*, Node*> colls;

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
		map<string,Node*>::const_iterator itr = nodes.begin();

		while(itr != nodes.end()){
			itr->second->update();
			itr++;
		}
	}
	void collideAll(){
		map<string,Node*>::const_iterator objx = nodes.begin();
		map<string,Node*>::const_iterator objy = nodes.begin();
		objy++;
		while(objx != nodes.end()){
			if(objx == objy) continue;
			while(objy != nodes.end()){
				if(objx->second->isColliding(objy->second)) objx->second->collide(objy->second);
				objy++;
			}
			objx++;
		}
	}
};

NodeManager nodeManager;

void collision(){
	nodeManager.collideAll();
}

void input(){
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		nodeManager.get("Player")->setVel(-3,0);
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		nodeManager.get("Player")->setVel(3,0);
	}else{
		nodeManager.get("Player")->setVel(0,0);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		nodeManager.get("Player2")->setVel(-3,0);
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		nodeManager.get("Player2")->setVel(3,0);
	}else{
		nodeManager.get("Player2")->setVel(0,0);
	}
}

void animate(){

}

void logic(){
	nodeManager.updateAll();
	collision();
}

void render(){
	window.clear(sf::Color::White);
	nodeManager.drawAll(window);
    window.display();
}

void gameLoop(){
	startTime = gameClock.getElapsedTime();
	input();
	animate();
	logic();
	render();
}

void gameInit(){
	nodeManager.add("WallTop",new Wall(Wall::Wall_Type::Top));
	nodeManager.add("WallLeft",new Wall(Wall::Wall_Type::Left));
	nodeManager.add("WallRight",new Wall(Wall::Wall_Type::Right));
	nodeManager.add("Floor", new Floor());
	nodeManager.add("Player",new Player());
	nodeManager.add("Player2",new Player());
	nodeManager.get("Player")->setPos(P1_START_POSX,HEIGHT-FLOOR_HEIGHT-PLAYER_H);
	nodeManager.get("Player2")->setPos(P2_START_POSX,HEIGHT-FLOOR_HEIGHT-PLAYER_H);
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




