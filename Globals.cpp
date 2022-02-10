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
	bool			fullScreen = false;
	bool			showCursor = true;
	SpriteManager*	spriteManager;
	SDL_Surface*	screen;
	SDL_Event		event;
	Map*			currentMap;
	Camera*			camera;
	float			currentTime;
	Factory*		factory;
	Player*			player;
	bool			developer;
	float			deltaTime;
	Player*			currentPlayer;
	PlayerBelt*		playerBelt;
	int				spriteAnimationSpeed = 400;

	int				tilesize = 50;
	int				resolution_x = 1440;
	int				resolution_y = 900;
	int				healthKitHeal = 30;

	void AlertMessageToConsole( std::string message )
	{
		std::cout << message << std::endl;
	}
};