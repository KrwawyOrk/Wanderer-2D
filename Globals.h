#include "SDL.h"

class Camera;
class Factory;
class Game;
class Map;
class Player;
class SpriteManager;

namespace Globals
{
	extern Game*			game;
	extern bool				fullScreen;
	extern SpriteManager*	spriteManager;
	extern SDL_Surface*		screen;
	extern SDL_Event		event;
	extern Map*				currentMap;
	extern Camera*			camera;
	extern int				currentTime;
	extern Factory*			factory;
	extern Player*			player;
	extern bool				developer;
	extern float			deltaTime;
	extern Player*			currentPlayer;

	extern int				tilesize;
	extern int				resolution_x;
	extern int				resolution_y;

	extern int				healthKitHeal;
};
