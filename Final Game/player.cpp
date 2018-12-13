#include "player.h"
#include "smartSprite.h"
#include "explodingTwoWayMultiSprite.h"

Player::Player( const std::string& name) :
  TwoWayMultiSprite(name),
  observers(),
  collision(false),
  explosion(nullptr),
  initialVelocity(getVelocity()),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  activeBullets(),
  freeBullets(),
  //strategy(new PerPixelCollisionStrategy), //changed
  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0),
  facing(RIGHT),
  godMode(false)
{ }

Player::Player(const Player& s) :
  TwoWayMultiSprite(s),
  observers(s.observers),
  collision(s.collision),
  explosion(s.explosion),
  initialVelocity(s.getVelocity()),

  //bullet stuff below
  bulletName(s.bulletName),
  activeBullets(s.activeBullets),
  freeBullets(s.freeBullets),
  //strategy(s.strategy), //changed
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  facing(s.facing),
  godMode(s.godMode)
  { }

Player& Player::operator=(const Player& s) {
  TwoWayMultiSprite::operator=(s);
  collision = s.collision;
  explosion = s.explosion;
  initialVelocity = s.initialVelocity;
  godMode = s.godMode;
  return *this;
}

/*bool Player::collidedWith(const Drawable* obj) {
  std::list<Bullet>::iterator ptr = activeBullets.begin();
  while(ptr != activeBullets.end()) {
    if(strategy->execute(*ptr, *obj) ) {
      freeBullets.push_back(*ptr);
      ptr = activeBullets.erase(ptr);
      return true;
    }
  ++ptr;
  }
  return false;
} */

void Player::stop() { 
  setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    turnRight();

    facing = RIGHT;
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    turnLeft();

    facing = LEFT;
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::detach(SmartSprite *o) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while(ptr != observers.end()) {
    if(*ptr == o) {
        ptr = observers.erase(ptr);
        return;
    }
    ++ptr;
  }
}

void Player::notify() {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while(ptr != observers.end()) {
    (*ptr)->setPlayerPos(getPosition());
    ++ptr;
  }
}

//added this for explosion
void Player::explode() {
  if(!explosion) explosion = new ExplodingTwoWayMultiSprite(*this);
} 

void Player::draw() const {
  if(explosion) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());

  //Bullet stuff below
  for(const Bullet& bullet : activeBullets) {
    bullet.draw();
  }
}

void Player::shoot() {
  if(timeSinceLastFrame < bulletInterval) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;

  Bullet bullet(bulletName);
  switch(facing) {
     case RIGHT:
       bullet.setPosition(getPosition() + Vector2f(deltaX, deltaY) );
       bullet.setVelocity(getVelocity() + Vector2f(minSpeed, 0) );
       break;
     case LEFT:
       bullet.setPosition(getPosition() + Vector2f(deltaX, deltaY) );
       bullet.setVelocity(getVelocity() + Vector2f(-minSpeed, 0) );
       break;
  }

  if(!freeBullets.empty()) {
     bullet = freeBullets.back();
     bullet.reset();

     switch(facing) {
	case RIGHT:
	  bullet.setPosition(getPosition() + Vector2f(deltaX, deltaY) );
     	  bullet.setVelocity(getVelocity() + Vector2f(minSpeed, 0) );
	  break;
	case LEFT:
	  bullet.setPosition(getPosition() + Vector2f(deltaX, deltaY) );
    	  bullet.setVelocity(getVelocity() + Vector2f(-minSpeed, 0) );
	  break;
     }

     activeBullets.push_back(bullet);
     freeBullets.pop_back();
  }
  else {
     activeBullets.push_back(bullet);
  }

  timeSinceLastFrame = 0;
}

void Player::update(Uint32 ticks) {

  timeSinceLastFrame += ticks;
  std::list<Bullet>::iterator it = activeBullets.begin();
  while(it != activeBullets.end()) {
    it->update(ticks);
    if(it->goneTooFar()) {
      freeBullets.push_back(*it);
      it = activeBullets.erase(it);
    }
    else {
      it->update(ticks);
      ++it;
    }
  } 

  if(explosion) {
     explosion->update(ticks);
     if(explosion->chunkCount() == 0) {
	delete explosion;
	explosion = NULL;
     }
     return;
  }

  advanceFrame(ticks);

  if ( !collision ) TwoWayMultiSprite::update(ticks);

  /*if(collision) {
     explosion->update(ticks);
  } */

  notify();
  
  stop();
}

