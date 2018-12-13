#include <cmath>
#include <random>
#include <functional>
#include "gameData.h"
#include "renderContext.h"
#include "player.h"
#include "smartSprite.h"

SmartSprite::SmartSprite(const std::string& s, const Player* p) :
	Sprite(s),
	playerPos(p->getPosition()),
	playerWidth(p->getScaledWidth()),
	playerHeight(p->getScaledHeight()),
	modeType(NORMAL),
	safeDistance(Gamedata::getInstance().getXmlFloat(s+"/safeDistance"))
{ }

SmartSprite::SmartSprite(const SmartSprite& s) :
	Sprite(s),
	playerPos(s.playerPos),
	playerWidth(s.playerWidth),
	playerHeight(s.playerHeight),
	modeType(s.modeType),
	safeDistance(s.safeDistance)
{ }

float distance(float x1, float y1, float x2, float y2) {
	float x = x1 - x2;
	float y = y1 - y2;
	return hypot(x, y);
}

void SmartSprite::turnRight() {
	setVelocityX(-abs(getVelocityX())); 
}
void SmartSprite::turnLeft() {
	setVelocityX(fabs(getVelocityX()));
}
void SmartSprite::goUp() {
	setVelocityY(-fabs(getVelocityY()));
}
void SmartSprite::goDown() {
	setVelocityY(fabs(getVelocityY()));
}

void SmartSprite::update(Uint32 ticks) {
        Sprite::update(ticks);
        float x = getX() + getImage()->getWidth()/2;
        float y = getY() + getImage()->getHeight()/2;
        float sx = playerPos[0] + playerWidth/2;
        float sy = playerPos[1] + playerHeight/2;
        float distanceToSprite = ::distance(x, y, sx, sy);

	if(modeType == NORMAL) {
		if(distanceToSprite < safeDistance) modeType = EVADE;
	}
	else if(modeType == EVADE) {
		if(distanceToSprite > safeDistance) modeType = NORMAL;
		else {
			if(x < sx) turnLeft();
			if(x > sx) turnRight();
			if(y < sy) goUp();
			if(y > sy) goDown();
		}
	}
}

