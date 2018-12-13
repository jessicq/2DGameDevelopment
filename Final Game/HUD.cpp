#include <SDL_image.h>
#include <algorithm>
#include <sstream>
#include <string>
#include "ioMod.h"
#include "gameData.h"
#include "HUD.h"
#include "player.h"

Hud& Hud::getInstance() {
	static Hud instance;
	return instance;
}

Hud::Hud() :
	text(Gamedata::getInstance().getXmlStr("HUD/text")),
	width(Gamedata::getInstance().getXmlInt("HUD/width")),
	height(Gamedata::getInstance().getXmlInt("HUD/height")),
	pos(Vector2f(Gamedata::getInstance().getXmlInt("HUD/position/x"),
		  	Gamedata::getInstance().getXmlInt("HUD/position/y"))),
	visible(true),
	textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/textColor/r")),
		static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/textColor/g")),
		static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/textColor/b")),
		static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/textColor/a"))}),
	backgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backgroundColor/r")),
			static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backgroundColor/g")),
			static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backgroundColor/b")),
			static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/backgroundColor/a"))}),
	outlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/outlineColor/r")),
			static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/outlineColor/g")),
			static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/outlineColor/b")),
			static_cast<Uint8>(Gamedata::getInstance().getXmlInt("HUD/outlineColor/a"))})
{ }

void Hud::draw(int active, int free, bool godMode) {
	if(isVisible()) {
		SDL_Rect rect;
		rect.x = getPosition()[0];
		rect.y = getPosition()[1];
		rect.h = getHeight();
		rect.w = getWidth();

		SDL_Renderer* renderer = IoMod::getInstance().getRenderer();
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		SDL_RenderFillRect(renderer, &rect);

		string mode;
		if(godMode) mode = "On";
		else mode = "Off";

		std::ostringstream hudDisplay;
		hudDisplay << getText();
		IoMod::getInstance().writeText(hudDisplay.str(), getPosition()[0], getPosition()[1], getTextColor());
		IoMod::getInstance().writeText("SPACE = Shoot, G = God Mode", getPosition()[0], getPosition()[1]+25, getTextColor());
		IoMod::getInstance().writeText("ActiveBullets: " + std::to_string(active), getPosition()[0], getPosition()[1]+50, getTextColor());
		IoMod::getInstance().writeText("FreeBullets: " + std::to_string(free), getPosition()[0], getPosition()[1]+75, getTextColor());
		IoMod::getInstance().writeText("God Mode: " + mode, getPosition()[0], getPosition()[1]+100, getTextColor());
	}
}

