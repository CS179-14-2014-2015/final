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
const int MAP_ROWS = HEIGHT/TILE_SIZE;
const int MAP_COLUMNS = WIDTH/TILE_SIZE;
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
sf::Time frameTime;
sf::RectangleShape shape(sf::Vector2f(FLOOR_WIDTH,FLOOR_HEIGHT));

//some references from http://www.gamefromscratch.com/page/Game-From-Scratch-CPP-Edition.aspx
//and animation references from https://github.com/LaurentGomila/SFML/wiki/Source:-AnimatedSprite

class Animation{
private:
	std::vector<sf::IntRect> m_frames;
	const sf::Texture* m_texture;
public:
	Animation() : m_texture(NULL){}
	~Animation(){
		m_texture = NULL;
	}
	void addFrame(sf::IntRect rect){
		m_frames.push_back(rect);
	}
	void setSpriteSheet(const sf::Texture& texture){
		m_texture = &texture;
	}
	const sf::Texture* getSpriteSheet() const{
		return m_texture;
	}
	std::size_t getSize() const{
		return m_frames.size();
	}
	const sf::IntRect& getFrame(std::size_t n) const{
		return m_frames[n];
	}
};

class AnimatedSprite : public sf::Drawable, public sf::Transformable{
private:
	const Animation* m_animation;
    sf::Time m_frameTime;
    sf::Time m_currentTime;
    std::size_t m_currentFrame;
    bool m_isPaused;
    bool m_isLooped;
    const sf::Texture* m_texture;
    sf::Vertex m_vertices[4];
public:
	AnimatedSprite() : m_animation(NULL), m_frameTime(sf::seconds(0.001)), m_currentFrame(0), m_isPaused(true), m_isLooped(false), m_texture(NULL){}
	AnimatedSprite(sf::Time frameTime, bool paused, bool looped) : m_animation(NULL), m_frameTime(frameTime), m_currentFrame(0), m_isPaused(paused), m_isLooped(looped), m_texture(NULL){}
	~AnimatedSprite(){
		m_animation = NULL;
		m_texture = NULL;
	}

	void setAnimation(const Animation& animation){
		m_animation = &animation;
		m_texture = m_animation->getSpriteSheet();
		m_currentFrame = 0;
		setFrame(m_currentFrame, true);
	}
	void setFrameTime(sf::Time time){
		m_frameTime = time;
	}
	void play(){
		m_isPaused = false;
	}
	void play(const Animation& animation){
		if (getAnimation() != &animation)
			setAnimation(animation);
		play();
	}
	void pause(){
		m_isPaused = true;
	}
	void stop(){
		m_isPaused = true;
		m_currentFrame = 0;
		setFrame(m_currentFrame, true);
	}
	void setLooped(bool looped){
		m_isLooped = looped;
	}
	void setColor(const sf::Color& color){
		// Update the vertices' color
		m_vertices[0].color = color;
		m_vertices[1].color = color;
		m_vertices[2].color = color;
		m_vertices[3].color = color;
	}
	const Animation* getAnimation() const{
		return m_animation;
	}
	sf::FloatRect getLocalBounds() const{
		sf::IntRect rect = m_animation->getFrame(m_currentFrame);
		float width = static_cast<float>(std::abs(rect.width));
		float height = static_cast<float>(std::abs(rect.height));
		return sf::FloatRect(0.f, 0.f, width, height);
	}
	sf::FloatRect getGlobalBounds() const{
		return getTransform().transformRect(getLocalBounds());
	}
	bool isLooped() const{
		return m_isLooped;
	}
	bool isPlaying() const{
		return !m_isPaused;
	}
	sf::Time getFrameTime() const{
		return m_frameTime;
	}
	void setFrame(std::size_t newFrame, bool resetTime){
		if (m_animation){
			//calculate new vertex positions and texture coordiantes
			sf::IntRect rect = m_animation->getFrame(newFrame);

			m_vertices[0].position = sf::Vector2f(0.f, 0.f);
			m_vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
			m_vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
			m_vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

			float left = static_cast<float>(rect.left) + 0.0001f;
			float right = left + static_cast<float>(rect.width);
			float top = static_cast<float>(rect.top);
			float bottom = top + static_cast<float>(rect.height);

			m_vertices[0].texCoords = sf::Vector2f(left, top);
			m_vertices[1].texCoords = sf::Vector2f(left, bottom);
			m_vertices[2].texCoords = sf::Vector2f(right, bottom);
			m_vertices[3].texCoords = sf::Vector2f(right, top);
		}

		if (resetTime) m_currentTime = sf::Time::Zero;
	}
	void update(sf::Time deltaTime){
		// if not paused and we have a valid animation
		if (!m_isPaused && m_animation){
			// add delta time
			m_currentTime += deltaTime;
			// if current time is bigger then the frame time advance one frame
			if (m_currentTime >= m_frameTime){
				// reset time, but keep the remainder
				m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());
				// get next Frame index
				if (m_currentFrame + 1 < m_animation->getSize()) m_currentFrame++;
				else{
					// animation has ended
					m_currentFrame = 0; // reset to start
					if (!m_isLooped){
						m_isPaused = true;
					}
				}
				// set the current frame, not reseting the time
				setFrame(m_currentFrame, false);
			}
		}
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const{
		if (m_animation && m_texture){
			states.transform *= getTransform();
			states.texture = m_texture;
			target.draw(m_vertices, 4, sf::Quads, states);
		}
	}
};

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
		return spr.getGlobalBounds();
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
		sf::Rect<float> mine = getBoundingRect();
		sf::Rect<float> other = n->getBoundingRect();
		if(mine.intersects(other)){
			return true;
		}
		return false;
	}
	virtual void collide(Node* n){
		cout << "collide boy" << endl;
		//collision reaction here raffeh
		//collision reaction here raffeh
		//collision reaction here raffeh
	}
};

class Player : public Node{
protected:
	vector<Animation> anims;
	vector<sf::Texture> txts;
	AnimatedSprite spr;
	Animation* currAnim;
	bool face;
public:
	Player() : spr(sf::seconds(0.05),true,false) {
		load("textures/idleplayer.png");
		load("textures/walkplayer.png");
		for(int i = 0; i < 2; i++){
			anims.push_back(*new Animation);
			anims[i].setSpriteSheet(txts[i]);
			for(int j = 0; j < 30; j++){
				anims[i].addFrame(sf::IntRect((j%5)*25,(j/5)*50,25,50));
			}
		}

		currAnim = &anims[0];
		assert(isLoaded());
		getAnimatedSprite().setOrigin(12.4998, 25);
	};
	~Player(){};

	void update(){
		if(loaded){
			setPos(pos.x + vel.x*TFPS, pos.y + vel.y*TFPS);
		}
		//cout << getAnimatedSprite().getOrigin().x << " " << getAnimatedSprite().getOrigin().y << endl;
		spr.play(*currAnim);
		spr.update(frameTime);
	}
	void setAnim(int n){
		if(n > anims.size()) return;
		currAnim = &anims[n];
	}
	virtual void draw(sf::RenderWindow &rw){
		if(loaded){
			rw.draw(spr);
		}
	}
	virtual AnimatedSprite& getAnimatedSprite(){
		return spr;
	}
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
			loaded = true;
			txts.push_back(txt);
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
		return spr.getGlobalBounds();
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
	virtual bool isColliding(Node* n){
		sf::Rect<float> mine = getBoundingRect();
		sf::Rect<float> other = n->getBoundingRect();
		//cout << "x : " << mine.left << " y: " << mine.top << " w: " << mine.width << " h: " << mine.height << endl;
		//cout << "x : " << other.left << " y: " << other.top << " w: " << other.width << " h: " << other.height << endl;
		if(mine.intersects(other)){
			return true;
		}
		return false;
	}
	virtual void collide(Node* n){
		cout << "boy collide player" << endl;
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
				if(objx->second->isColliding(objy->second)){
					objx->second->collide(objy->second);
				}
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
		((Player*)nodeManager.get("Player"))->getAnimatedSprite().setScale(-1,1);
		((Player*)nodeManager.get("Player"))->setAnim(1);
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		nodeManager.get("Player")->setVel(3,0);
		((Player*)nodeManager.get("Player"))->getAnimatedSprite().setScale(1,1);
		((Player*)nodeManager.get("Player"))->setAnim(1);
	}else{
		nodeManager.get("Player")->setVel(0,0);
		((Player*)nodeManager.get("Player"))->setAnim(0);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		nodeManager.get("Player2")->setVel(-3,0);
		((Player*)nodeManager.get("Player2"))->getAnimatedSprite().setScale(-1,1);
		((Player*)nodeManager.get("Player2"))->setAnim(1);
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		nodeManager.get("Player2")->setVel(3,0);
		((Player*)nodeManager.get("Player2"))->getAnimatedSprite().setScale(1,1);
		((Player*)nodeManager.get("Player2"))->setAnim(1);
	}else{
		nodeManager.get("Player2")->setVel(0,0);
		((Player*)nodeManager.get("Player2"))->setAnim(0);
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
	frameTime = gameClock.restart();
	input();
	animate();
	logic();
	render();
}

void gameInit(){
	//nodeManager.add("WallTop",new Wall(Wall::Wall_Type::Top));
	//nodeManager.add("WallLeft",new Wall(Wall::Wall_Type::Left));
	//nodeManager.add("WallRight",new Wall(Wall::Wall_Type::Right));
	//nodeManager.add("Floor", new Floor());
	nodeManager.add("Player",new Player());
	nodeManager.add("Player2",new Player());
	((Player*)nodeManager.get("Player2"))->getAnimatedSprite().setScale(-1,1);
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




