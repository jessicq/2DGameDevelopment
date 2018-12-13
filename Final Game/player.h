#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include "twoWayMultiSprite.h"
#include "smartSprite.h"
#include "bullet.h"
#include "collisionStrategy.h"

class ExplodingTwoWayMultiSprite;

class Player : public TwoWayMultiSprite {
	public:
		Player(const std::string&);
		Player(const Player&);
		virtual ~Player() { }
	
		virtual void draw() const;
		virtual void update(Uint32 ticks);

		void collided() { collision = true; }
		void missed() { collision = false; }
		Player& operator=(const Player&);

		void stop();
		void right();
		void left();
		void up();
		void down();

		void attach(SmartSprite* o) { observers.push_back(o); };
		void detach(SmartSprite* o);
 		void notify();

		virtual void explode();

		int getFreeBullets() { return freeBullets.size(); }
		int getActiveBullets() { return activeBullets.size(); }
 		std::list<Bullet> getBullets() { return activeBullets; }
		void shoot();
   		//bool collidedWith(const Drawable* obj);

		//god mode
		bool isGod() { return godMode; }
		void setGodMode(bool gm) { godMode = gm; } 
	private:
		std::list<SmartSprite*> observers;
		bool collision;
		ExplodingTwoWayMultiSprite* explosion;
		Vector2f initialVelocity;
		
		//Bullets
		std::string bulletName;
		std::list<Bullet> activeBullets;
		std::list<Bullet> freeBullets;
		//CollisionStrategy* strategy;	 	

		float minSpeed;
		float bulletInterval;
		float timeSinceLastFrame;

		enum Facing{RIGHT, LEFT};
		Facing facing;

		//god mode
		bool godMode;
};

#endif
		
