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
const int P1_START_POSX = 100;
const int P2_START_POSX = WIDTH - 60;
const int TILE_SIZE = 25;
const int MAP_ROWS = HEIGHT/TILE_SIZE;
const int MAP_COLUMNS = WIDTH/TILE_SIZE + 1;
const int FPS = 60;
const float PLAYER_VY = 5;
const int PLAYER_W = 25;
const int PLAYER_H = 50;
const float TFPS = 100.0f/FPS;
const float PLAYER_VX = 3.5;
const double GRAV = 9.8/(FPS*1.0);
//const int FLOOR_WIDTH = WIDTH;
//const int FLOOR_HEIGHT = 30;
//const float SWORD_INC = PLAYER_H/5;
//const float SWORD_W = 50;
//const float SWORD_H = 5;
//const float SWORD_ACC = 0.5;

int map_grid[MAP_ROWS][MAP_COLUMNS];

sf::RenderWindow window(sf::VideoMode(WIDTH-1,HEIGHT-1), "Sorta Jousting");
sf::Clock gameClock;
sf::Time frameTime;

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
	AnimatedSprite() : m_animation(NULL), m_frameTime(sf::seconds(0.05f)), m_currentFrame(0), m_isPaused(true), m_isLooped(false), m_texture(NULL){}
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

class TextureLoader{
protected:
	map<string, sf::Texture*> textures;
public:
	TextureLoader(){};
	~TextureLoader(){
		map<string, sf::Texture*>::iterator itr = textures.begin();
		delete itr->second;
		textures.erase(itr);
	};

	sf::Texture* getTexture(string n){
		sf::Texture* temp = NULL;
		map<string, sf::Texture*>::const_iterator results = textures.find(n);
		if(results != textures.end()){
			temp = results->second;
		}else{
			temp = new sf::Texture();
			temp->loadFromFile(n);
			temp->setSmooth(true);
			textures.insert(pair<string, sf::Texture*>(n,temp));
			return temp;
		}
		return temp;
	}
};

TextureLoader tl;

class Node{
protected:
	sf::Vector2f vel, acc;
	sf::Vector2i col;
	sf::Sprite spr;
	sf::Texture txt;
	AnimatedSprite aspr;
	string filename;
	short type;
	bool loaded;
	bool cstatic;
public:
	Node() : loaded(false){}
	virtual ~Node(){};

	virtual bool isLoaded() const{
		return loaded;
	}
	virtual void load(string fn){
		spr.setTexture(*tl.getTexture(fn));
		loaded = true;
	}
	virtual void draw(sf::RenderWindow &rw){
		if(loaded){
			rw.draw(spr);
		}
	}
	virtual void update(){
	}
	virtual void move(){
	}
	virtual void setPos(float x, float y){
		if(loaded){
			spr.setPosition(x,y);
		}
	}
	virtual void setVel(float x, float y){
		if(loaded){
			vel.x = x;
			vel.y = y;
		}
	}
	virtual void setAcc(float x, float y){
		if(loaded){
			acc.x = x;
			acc.y = y;
		}
	}
	virtual void setCol(int x, int y){
		if(loaded){
			col.x = x;
			col.y = y;
		}
	}
	virtual float getHeight() const{
		return spr.getGlobalBounds().height;
	}
	virtual float getWidth() const{
		return spr.getGlobalBounds().width;
	}
	virtual sf::Rect<float> getBoundingRect() const{
		return spr.getGlobalBounds();
	}
	virtual sf::Vector2f getPos(){
		if(loaded){
			return spr.getPosition();
		}
		return sf::Vector2f();
	}
	virtual sf::Vector2f getVel(){
		if(loaded){
			return vel;
		}
		return sf::Vector2f();
	}
	virtual sf::Vector2f getAcc(){
		if(loaded){
			return acc;
		}
		return sf::Vector2f();
	}
	virtual sf::Vector2i getCol(){
		if(loaded){
			return col;
		}
		return sf::Vector2i();
	}
	virtual sf::Sprite& getSprite(){
		return spr;
	}
	virtual AnimatedSprite& getAnimatedSprite(){
		return aspr;
	}
	virtual void setAnim(int n){
		return;
	}
	virtual bool isColliding(Node* n, sf::Vector2i axis){
		sf::Rect<float> mine = getBoundingRect();
		sf::Rect<float> other = n->getBoundingRect();

		//axis checking
		mine.left *= axis.x;
		mine.width *= axis.x;
		mine.top *= axis.y;
		mine.height *= axis.y;
		other.left *= axis.x;
		other.width *= axis.x;
		other.top *= axis.y;
		other.height *= axis.y;

		if(mine.intersects(other)){
			return true;
		}
		return false;
	}
	virtual void collide(Node* n, sf::Vector2i axis){};
};

class Player : public Node{
protected:
	vector<Animation> anims;
	vector<sf::Texture> txts;
	Animation* currAnim;
	bool face;
public:
	Player(){
		aspr = AnimatedSprite(sf::seconds(0.05f),true,false);
		load("textures/idleplayer.png");
		load("textures/walkplayer.png");
		load("textures/jumpplayer.png");

		for(int i = 0; i < 3; i++){
			anims.push_back(*new Animation);
			anims[i].setSpriteSheet(txts[i]);
			for(int j = 0; j < 30; j++){
				anims[i].addFrame(sf::IntRect((j%5)*25,(j/5)*50,25,50));
			}
		}
		setCol(1,1);
		setAcc(0,GRAV);
		type = 1;
		cstatic = false;
		currAnim = &anims[0];
		assert(isLoaded());
		getAnimatedSprite().setOrigin(12.5, 25);
	};
	~Player(){};

	void update(){
		if(loaded){
			if(col.y){
				vel+= acc;
			}
			aspr.play(*currAnim);
			aspr.update(frameTime);
		}
	}
	void move(){
		if(loaded && abs(getVel().x+getVel().y) > 0.0001){

			aspr.move(col.x*vel.x*TFPS, col.y*vel.y*TFPS);
		}
	}
	void setAnim(int n){
		if(n > anims.size() || n < 0) return;
		currAnim = &anims[n];
	}
	virtual void draw(sf::RenderWindow &rw){
		if(loaded){
			rw.draw(aspr);
		}
	}
	virtual bool isLoaded() const{
		return loaded;
	}
	virtual void load(string fn){
		txts.push_back(*tl.getTexture(fn));
		loaded = true;
	}
	virtual void setPos(float x, float y){
		if(loaded){
			aspr.setPosition(x,y);
		}
	}
	virtual float getHeight() const{
		return aspr.getGlobalBounds().height;
	}
	virtual float getWidth() const{
		return aspr.getGlobalBounds().width;
	}
	virtual sf::Rect<float> getBoundingRect() const{
		return aspr.getGlobalBounds();
	}
	virtual sf::Vector2f getPos(){
		if(loaded){
			return aspr.getPosition();
		}
		return sf::Vector2f();
	}
	virtual bool isColliding(Node* n, sf::Vector2i axis){
		sf::Rect<float> mine = getBoundingRect();
		sf::Rect<float> other = n->getBoundingRect();

		//pre-collision
		mine.left += getVel().x;
		mine.top += getVel().y;

		//axis checking
		mine.left *= axis.x;
		mine.top *= axis.y;
		other.left *= axis.x;
		other.top *= axis.y;

		if(mine.intersects(other)){
			return true;
		}
		return false;
	}
	virtual void collide(Node* n, sf::Vector2i axis){
		sf::Rect<float> mine = getBoundingRect();
		sf::Rect<float> other = n->getBoundingRect();
		sf::Rect<float> inter = sf::Rect<float>();

		//axis checking
		mine.left *= axis.x;
		mine.top *= axis.y;
		other.left *= axis.x;
		other.top *= axis.y;

		if(mine.intersects(other, inter)){
			if(getPos().y > inter.top){
				getAnimatedSprite().move(0,axis.y*((other.top + other.height) - inter.top));
				setVel(getVel().x,0);
			}else if(getPos().y <= inter.top){
				getAnimatedSprite().move(0,axis.y*(other.top - (inter.top + inter.height)));
				setVel(getVel().x,0);
			}
			if(getPos().x > inter.left){
				getAnimatedSprite().move(axis.x*((other.left + other.width) - inter.left),0);
				setVel(0,getVel().y);
			}else if(getPos().x <= inter.left){
				getAnimatedSprite().move(axis.x*(other.left - (inter.left + inter.width)),0);
				setVel(0,getVel().y);
			}
		}
	}
};

class Tile : public Node{
public:
	Tile(int x = 0, int y = 0){
		load("textures/floor.png");
		assert(isLoaded());
		setPos(x,y);
		type = 2;
		spr.setOrigin(12.5,12.5);
	}
	~Tile(){};

	void collide(Node* n){}
	void draw(sf::RenderWindow& rw){
		Node::draw(rw);
	}
};

vector<Node> tiles;


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
	/*void collideAll(){
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
	}*/

	void moveAll(){
		map<string,Node*>::const_iterator itr = nodes.begin();

		while(itr != nodes.end()){
			itr->second->move();
			itr++;
		}
	}
};

NodeManager nodeManager;

void collideTiles(){
	Node* p = nodeManager.get("Player");
	p->setCol(1,1);
	sf::Vector2f p_pos = p->getPos();
	sf::Vector2f p_size = sf::Vector2f(p->getWidth(),p->getHeight());
	for(int i = 0; i < tiles.size(); i++){
		sf::Vector2f t_pos = tiles[i].getPos();
		if(t_pos.x >= p_pos.x - p_size.x/2 && t_pos.x <= p_pos.x + p_size.x/2){
			if(p->isColliding(&tiles[i], sf::Vector2i(0,1))){
				p->collide(&tiles[i], sf::Vector2i(0,1));
				p->setCol(1,0);
			}
		}
		if(t_pos.y >= p_pos.y - p_size.y/2 && t_pos.y <= p_pos.y + p_size.y/2){
			if(p->isColliding(&tiles[i], sf::Vector2i(1,0))){
				p->collide(&tiles[i], sf::Vector2i(1,0));
				p->setCol(0,1);
			}
		}
	}

	Node* p2 = nodeManager.get("Player2"); 
	p_pos = p2->getPos();
	p_size = sf::Vector2f(p2->getWidth(),p2->getHeight());
	for(int i = 0; i < tiles.size(); i++){
		sf::Vector2f t_pos = tiles[i].getPos();
		if(t_pos.x >= p_pos.x - p_size.x/2 && t_pos.x <= p_pos.x + p_size.x/2){
			if(p2->isColliding(&tiles[i], sf::Vector2i(0,1))){
				p2->collide(&tiles[i], sf::Vector2i(0,1));
				p2->setCol(1,0);
			}
		}
		if(t_pos.y >= p_pos.y - p_size.y/2 && t_pos.y <= p_pos.y + p_size.y/2){
			if(p2->isColliding(&tiles[i], sf::Vector2i(1,0))){
				p2->collide(&tiles[i], sf::Vector2i(1,0));
				p2->setCol(0,1);
			}
		}
	}

}

void collision(){
	collideTiles();
}

void input(){
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		nodeManager.get("Player")->setVel(-PLAYER_VX,nodeManager.get("Player")->getVel().y);
		nodeManager.get("Player")->getAnimatedSprite().setScale(-1,1);
		nodeManager.get("Player")->setAnim(1);
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		nodeManager.get("Player")->setVel(PLAYER_VX,nodeManager.get("Player")->getVel().y);
		nodeManager.get("Player")->getAnimatedSprite().setScale(1,1);
		nodeManager.get("Player")->setAnim(1);
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		if(nodeManager.get("Player")->getCol().y == 0)nodeManager.get("Player")->setVel(nodeManager.get("Player")->getVel().x,-PLAYER_VY);
		nodeManager.get("Player")->setAnim(2);
	}else{
		if(!nodeManager.get("Player")->getCol().y){
			nodeManager.get("Player")->setVel(0,0);
		}
		nodeManager.get("Player")->setAnim(0);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		nodeManager.get("Player2")->setVel(-PLAYER_VX,nodeManager.get("Player2")->getVel().y);
		nodeManager.get("Player2")->getAnimatedSprite().setScale(-1,1);
		nodeManager.get("Player2")->setAnim(1);
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		nodeManager.get("Player2")->setVel(PLAYER_VX,nodeManager.get("Player2")->getVel().y);
		nodeManager.get("Player2")->getAnimatedSprite().setScale(1,1);
		nodeManager.get("Player2")->setAnim(1);
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
		if(nodeManager.get("Player2")->getCol().y == 0)nodeManager.get("Player2")->setVel(nodeManager.get("Player2")->getVel().x,-PLAYER_VY);
		nodeManager.get("Player2")->setAnim(2);
	}else{
		if(!nodeManager.get("Player2")->getCol().y){
			nodeManager.get("Player2")->setVel(0,0);
		}
		nodeManager.get("Player2")->setAnim(0);
	}
}

void animate(){

}

void logic(){
	nodeManager.updateAll();
	collision();
	nodeManager.moveAll();
}

void render(){
	window.clear(sf::Color::White);
	nodeManager.drawAll(window);
	for(int i = 0; i < tiles.size(); i++){
		tiles[i].draw(window);
	}
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

	nodeManager.add("Player",new Player());
	nodeManager.add("Player2",new Player());
	nodeManager.get("Player2")->getAnimatedSprite().setScale(-1,1);
	nodeManager.get("Player")->setPos(P1_START_POSX,HEIGHT-PLAYER_H/2.0-25);
	nodeManager.get("Player2")->setPos(P2_START_POSX,HEIGHT-PLAYER_H/2.0-25);

	for(int i = 0; i < MAP_ROWS; i++){
		for(int j = 0; j <  MAP_COLUMNS - 1; j++){
			if(i == 0 || i == MAP_ROWS-1 || j == 0 || j == MAP_COLUMNS-2) map_grid[i][j] =1;
			else map_grid[i][j] = 0;
		}
	}
	for(int i = 0; i < 5; i++){
		map_grid[12][i] = 1;
	}

	for(int i = 0; i < MAP_ROWS; i++){
		for(int j = 0; j < MAP_COLUMNS - 1; j++){
			if(map_grid[i][j] == 1){
				tiles.push_back(*new Tile(12.5+j*TILE_SIZE,12.5+i*TILE_SIZE));
			}
			else{
			}
		}
	}


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




