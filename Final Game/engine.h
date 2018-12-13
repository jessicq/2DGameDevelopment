#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "sound.h"

class Player;
class CollisionStrategy;
class SmartSprite;
class Hud;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();

private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world;
  World world2;
  World introduction;
  World twoWorld;
  World twoWorld2;
  Viewport& viewport;

  Player* player;
  Hud& hud;
 
  std::vector<Drawable*> bosses;
  std::vector<Drawable*> levelzero;
  std::vector<Drawable*> leveltwo;
  std::vector<Drawable*> sprites;
  std::vector<SmartSprite*> smarties;
  std::vector<CollisionStrategy*> strategies;
  int currentStrat;
  int currentSprite;

  bool collision;
  bool makeVideo;
  SDLSound sound;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
};
