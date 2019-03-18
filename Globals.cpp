#include "Globals.h"

#include "Camera.h"
#include "Factory.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "SpriteManager.h"

namespace Globals
{
	Game*			game;
	bool			fullScreen = true;
	SpriteManager*	spriteManager;
	SDL_Surface*	screen;
	SDL_Event		event;
	Map*			currentMap;
	Camera*			camera;
	int				currentTime;
	Factory*		factory;
	Player*			player;
	bool			developer;
	float			deltaTime;
	Player*			currentPlayer;

	int				tilesize = 50;
	int				resolution_x = 1440;
	int				resolution_y = 900;
	int				healthKitHeal = 30;
};