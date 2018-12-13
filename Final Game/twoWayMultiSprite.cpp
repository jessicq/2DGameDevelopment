#include "twoWayMultiSprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "explodingTwoWayMultiSprite.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if(timeSinceLastFrame > frameInterval) {
    	   currentFrame = (currentFrame+1) % numberOfFrames;
	   timeSinceLastFrame = 0;
	}
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")+100, 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")+100), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  imagesRight(ImageFactory::getInstance().getImages(name) ),
  imagesLeft(ImageFactory::getInstance().getImages(name + "Left") ),
  images(imagesRight),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
  Drawable(s), 
  imagesRight(s.imagesRight),
  imagesLeft(s.imagesLeft),
  images(s.imagesRight),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  worldWidth( s.worldWidth),
  worldHeight( s.worldHeight)
  { }

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  explosion = (s.explosion);
  currentFrame = (s.currentFrame);
  numberOfFrames = (s.numberOfFrames);
  frameInterval = (s.frameInterval);
  timeSinceLastFrame = (s.timeSinceLastFrame);
  worldWidth = (s.worldWidth);
  worldHeight = (s.worldHeight);
  return *this;
}

TwoWayMultiSprite::~TwoWayMultiSprite() { if(explosion)  delete explosion; }

void TwoWayMultiSprite::explode() {
   setPosition(Vector2f(-150, -150));
   if(!explosion) explosion = new ExplodingTwoWayMultiSprite(*this);
}

void TwoWayMultiSprite::draw() const { 
  if(explosion) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwoWayMultiSprite::turnRight() {
  images = imagesRight;
}

void TwoWayMultiSprite::turnLeft() {
  images = imagesLeft;
}

void TwoWayMultiSprite::update(Uint32 ticks) { 
  if(explosion) {
    explosion->update(ticks);
  
    if(explosion->chunkCount() == 0 ) {
       delete explosion;
       explosion = NULL;
    }
    
    return;
  } 

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images = imagesRight;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    images = imagesLeft;
  }  

}
