#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include <vector>
#include "sprite.h"
#include "multisprite.h"
#include "twoWayMultiSprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "smartSprite.h"
#include "HUD.h"

int level = 1, monsterCounter = 0, once = 0, music_once = 0;
bool alive = true;

Engine::~Engine() { 
  for(SmartSprite* next : smarties) {
    delete next;
  }
 
  for(auto nextone : sprites) {
     delete nextone;
  }

  for(auto nextzero : levelzero) {
     delete nextzero;
  }

  for(auto more : leveltwo) {
    delete more;
  }

  for(unsigned int i=0; i<strategies.size(); i++) {
     delete strategies[i];
  }

  for(auto b : bosses) {
     delete b;
  }  

  delete player;
  //delete healthBar;

  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world2("icemounds", Gamedata::getInstance().getXmlInt("icemounds/factor") ),
  introduction("happyland", Gamedata::getInstance().getXmlInt("happyland/factor") ),
  twoWorld("hellfire", Gamedata::getInstance().getXmlInt("hellfire/factor") ),
  twoWorld2("rocky", Gamedata::getInstance().getXmlInt("rocky/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Swinub")),
  hud(Hud::getInstance()),
  bosses(),
  levelzero(),
  leveltwo(),
  sprites(),
  smarties(),
  strategies(),
  currentStrat(0),
  currentSprite(0),
  collision(false),
  makeVideo(false),
  sound()
{
   sprites.emplace_back(new MultiSprite("EatingSwinub"));
   sprites.emplace_back(new MultiSprite("Gulpin"));
   sprites.emplace_back(new MultiSprite("Swablu"));
   sprites.emplace_back(new MultiSprite("Inkay"));
   sprites.emplace_back(new MultiSprite("Goomy"));
   sprites.emplace_back(new MultiSprite("Portal"));

   leveltwo.emplace_back(new MultiSprite("DeckardCain"));
   leveltwo.emplace_back(new MultiSprite("Mephisto"));
   
   int spiderAmt = Gamedata::getInstance().getXmlInt("spiderAmt");
   for(int i=0; i<spiderAmt; i++) {
     levelzero.emplace_back(new TwoWayMultiSprite("Spider")); 
   }

   int impAmt = Gamedata::getInstance().getXmlInt("impAmt");
   for(int i=0; i<impAmt; i++) {
     levelzero.emplace_back(new TwoWayMultiSprite("Imp"));
   }
   bosses.emplace_back(new MultiSprite("Diablo"));
   bosses.emplace_back(new MultiSprite("Baal"));
   bosses.emplace_back(new MultiSprite("Tyrael"));

   int limit = Gamedata::getInstance().getXmlInt("skullAmt");
   smarties.reserve(limit);
   for(int j = 0; j < limit; j++) {
     smarties.push_back(new SmartSprite("Skull", player));
     player->attach(smarties[j]);
   }

   strategies.push_back(new PerPixelCollisionStrategy); 
   Viewport::getInstance().setObjectToTrack(player);

   std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  if(level == 1) {
    introduction.draw();
  
    //sprites.size() - 1 
    if ( sprites.size() > 0 ) {
      for(unsigned int oners = 0; oners < 5; oners++) { 
	      sprites[oners]->draw();
      }
    }

    if(clock.getTicks() > 8000 && once == 0) {
       std::cout << "6000 passed" << std::endl;
       once++;
    }
    else if(clock.getTicks() < 8000 && once == 0) {
      std::stringstream intro;
      intro << "It's a beautiful day, until...";
      io.writeText(intro.str(), 200, 250, {255, 255, 0, 255});
    }

    if(level==1 && once == 1) sprites[5]-> draw(); //draw portal
    if(clock.getTicks() >= 11000) {
      once--;
      level++; 
    } //go to level two
  }

  if(level == 2) {
    twoWorld.draw();
    twoWorld2.draw();

    leveltwo[0]->draw();
    player->draw();
                                   
    if(clock.getTicks() < 20000) {
      std::stringstream cain;
      cain << "Deckard Cain: Diablo and his brothers must be stopped!";
      io.writeText(cain.str(), 20, 250, {255, 255, 0, 255});
    }

    if(clock.getTicks() >= 20000 && once == 0) {
      std::cout << "20000 passed" << std::endl;
      once++;
    }

    if(once == 1) {
      leveltwo[1]->draw();          

      if(clock.getTicks() >= 21000 && clock.getTicks() <= 26000) {
	std::stringstream meph;
        meph << "Cain: Mephisto! I'll teleport you away - stay alive till then and be evasive!";
        io.writeText(meph.str(), 20, 250, {255, 255, 0, 255});
      }

      for(const SmartSprite* next : smarties) {
        next->draw();
      }
    }

    unsigned int mephistoTime = Gamedata::getInstance().getXmlInt("mephistoTime");
    if(clock.getTicks() >= mephistoTime) {
      level = 0;
      once = 0;
    }
    
    hud.draw(player->getActiveBullets(), player->getFreeBullets(), player->isGod());
  }

  if(level == 0) {
    
    world.draw();
    world2.draw();

    for(const auto* nextone : levelzero) {
      nextone->draw();
    }  
   
    int diablo = Gamedata::getInstance().getXmlInt("diabloSpawn");
    int baal = Gamedata::getInstance().getXmlInt("baalSpawn");
    int tyrael = Gamedata::getInstance().getXmlInt("tyraelSpawn");
 
    if(monsterCounter >= diablo) {
      bosses[0]->draw();
      once = 1;
    }
    if(monsterCounter >= baal) {
      bosses[1]->draw();
      once = 2;
    }
    if(monsterCounter >= tyrael) {
      bosses[2]->draw();
      once = 3;

      std::stringstream ty;
      ty << "Tyrael: Congratulations - you have stopped Diablo! You will return home soon.";
      io.writeText(ty.str(), 20, 250, {255, 255, 0, 255}); 
    }
    if(monsterCounter == 0) {
      std::stringstream objective;
      objective << "Objective: Kill as much as you can until you are saved.";
      io.writeText(objective.str(), 20, 250, {255, 255, 0, 255});
    }

    player->draw();
    hud.draw(player->getActiveBullets(), player->getFreeBullets(), player->isGod());
  }

  std::stringstream sgsm;
  sgsm << "Jessica Qiu\n";
  io.writeText(sgsm.str(), 20, 450, {255, 255, 0, 255});

  if(hud.isVisible()) {
    std::stringstream fpsStream;
    fpsStream << "fps: " << clock.getFps();
    io.writeText(fpsStream.str(), 170, 450, {255, 255, 0, 255}); 
  }

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  collision = false;
 
  if(level == 2) {
    for(SmartSprite* d : smarties) {
      if(strategies[currentStrat]->execute(*player, *d)) {
	if(player->isGod() == false) {
	  collision = true;
	  alive = false;
	}
      }
    }

     for(auto itr : leveltwo) {
       for(Sprite detectBullets : player->getBullets() ) {
         if(strategies[currentStrat]->execute(detectBullets, *itr)) {
           //collision = true;
	   static_cast<MultiSprite*>(itr)->explode();
        }
      }
    }
  }

  if(level == 0) {
    for(auto *dMulti : levelzero) {
      if(strategies[currentStrat]->execute(*player, *dMulti)) {
        if(player->isGod() == false) {
	  collision = true;
          alive = false;
        }
      }
    }

    for(auto *bossPtr : bosses) {
      if(strategies[currentStrat]->execute(*player, *bossPtr)) {
	if(player->isGod() == false) {
	  collision = true;
	  alive = false;
	}
      }	
     }
  
    for(auto ptr : levelzero) {
       for(Sprite bulletPtr : player->getBullets() ) {
         if(strategies[currentStrat]->execute(bulletPtr, *ptr)) {
           static_cast<TwoWayMultiSprite*>(ptr)->explode();
           monsterCounter++;
         }
       }
    }
  }

  if(collision && (player->isGod() == false)) {
    //player->collided();
    player->explode();
    sound[0];
  }
  else {
    player->missed();
    collision = false;
  }
}

void Engine::update(Uint32 ticks) {
  if(!alive) {
    clock.pause();
  } 

  /* Start of Introduction */
  if(level == 1) {
     for(auto *oners : sprites) {
	oners->update(ticks);
     }
     sound[4];
  } 
 
  /* Start of Level 1 */
  if(level == 2) {
    if(once != 1) sound[5];
    static bool firstTimeInLevel2 = true;
    if ( firstTimeInLevel2 ) {
      firstTimeInLevel2 = false;
      std::vector<Drawable*>::iterator it = sprites.begin();
      while ( it != sprites.end() ) {
        delete *it;
        it = sprites.erase(it);
      }
      firstTimeInLevel2 = false;
    }
    checkForCollisions();
    player->update(ticks);

    for(auto *twoers : leveltwo) {
       twoers->update(ticks);
    }
  
    for(SmartSprite *next : smarties) {
       next->update(ticks);
    }

    if(once == 1 && music_once == 0){
      sound[1];
      music_once++;
    }

    twoWorld.update();
    twoWorld2.update();
  }

  /* Start of the Last Level */
  if(level == 0) {
    checkForCollisions();
    if(once < 1 && music_once != 3) sound[5];

    static bool firstTimeinLevel0 = true;
    if(firstTimeinLevel0) {
      music_once = 0;
      firstTimeinLevel0 = false;
      std::vector<Drawable*>::iterator it = leveltwo.begin();
      while(it != leveltwo.begin() ) {
	delete *it;
	it = leveltwo.erase(it);
      }

      /*std::vector<SmartSprite*>::iterator smart = smarties.begin();
      while( smart != smarties.end() ) {
        delete *smart;
      smart = smarties.erase(smart); 
      } */

      firstTimeinLevel0 = false;
    }

    player->update(ticks);
    
    for(auto* nextone : levelzero) {
      nextone->update(ticks);
    }

    if(once == 1 && music_once == 0) { //Spawn Diablo
      sound[2]; 
      music_once++;
    }

    if(once == 2 && music_once == 1) { //Spawn Baal
      sound[3];
      music_once++;
    }

    if(once == 3 && music_once == 2) { //Spawn Tyrael
      sound[6];
      music_once++;
    }

    for(auto bitr : bosses) {
      bitr->update(ticks);
    }

    world.update();
    world2.update();
  }

  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % smarties.size();
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(sprites[0]); 
  }
  else {
    Viewport::getInstance().setObjectToTrack(sprites[1]); 
  }
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  bool restart = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }

	if ( keystate[SDL_SCANCODE_SPACE] ) {
	  player->shoot();
	} 

        if ( keystate[SDL_SCANCODE_T] ) {
          //switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
	if(keystate[SDL_SCANCODE_F1]) {
	  hud.setVisibility(!hud.isVisible());
        }
	if(keystate[SDL_SCANCODE_G]) {
	  if(!player->isGod()) {
	    player->setGodMode(true);
	  }
	  else player->setGodMode(false);
 	} 
	if(keystate[SDL_SCANCODE_R] && level != 1) {
	  done = true;
          restart = true;
          alive = true;
  	  music_once = 0;
          monsterCounter = 0;
   	  once = 0;
  	  collision = false;
   	  clock.unpause();
          break;
	}
    }

   }//issue would be here
    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();

      if(keystate[SDL_SCANCODE_A]) {
	player->left();
      }
      if(keystate[SDL_SCANCODE_D]) {
	player->right();
      }
      if(keystate[SDL_SCANCODE_W]) {
	player->up();
      }
      if(keystate[SDL_SCANCODE_S]) {
	player->down();
      }

      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  
  return restart;
}
