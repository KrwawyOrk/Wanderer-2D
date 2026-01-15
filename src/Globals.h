#include "SDL.h"
#include <string>

class Camera;
class Factory;
class Game;
class Map;
class Player;
class PlayerBelt;
class SpriteManager;
class TextBox;

namespace Globals
{
	extern Game*			game;
	extern bool				fullScreen;
	extern bool				showCursor;
	extern SpriteManager*	spriteManager;
	extern SDL_Surface*		screen;
	extern SDL_Event		event;
	extern Map*				currentMap;
	extern Camera*			camera;
	extern float			currentTime;
	extern Factory*			factory;
	extern Player*			player;
	extern bool				developer;
	extern float			deltaTime;
	extern Player*			currentPlayer;
	extern PlayerBelt*		playerBelt;
	extern int				spriteAnimationSpeed;

	extern int				tilesize;
	extern int				resolution_x;
	extern int				resolution_y;

	extern int				healthKitHeal;

	extern TextBox*			messageLog;

	extern void				AlertMessageToConsole( std::string message );
};
