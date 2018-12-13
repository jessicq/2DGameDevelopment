#ifndef SMARTSPRITE_H
#define SMARTSPRITE_H
#include <string>
#include "sprite.h"

class Player;

class SmartSprite : public Sprite {
public:	
	SmartSprite(const std::string&, const Player*);
  	SmartSprite(const SmartSprite&);
   	virtual ~SmartSprite() { }

	virtual void update(Uint32 ticks);

	enum MODE{NORMAL,EVADE};

	void setPlayerPos(const Vector2f& p) {playerPos = p;}
	void setPlayerWidth(int w) {playerWidth = w;}
  	void setPlayerHeight(int h) {playerHeight = h;}
	void setSafeDistance(float sd) {safeDistance=sd;}
	void setCurrentMode(MODE m) {modeType=m;}

private:
	//enum MODE{NORMAL, EVADE};	

	Vector2f playerPos;
	int playerWidth;
	int playerHeight;
	MODE modeType;

	//int playerWidth;
	//int playerHeight;

	float safeDistance;

	void turnRight();
	void turnLeft();
	void goUp();
	void goDown();
};

#endif
