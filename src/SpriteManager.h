#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <string>
#include <map>

#include "SDL.h"

class GSMapEditor;
class Sprite;

class SpriteManager
{
public:
	SpriteManager()
	{
		m_spritesLoadedSuccessfully = false;
		LoadSprites();
	}

	bool GetSprite( Sprite& sprite, std::string spriteName );
	bool SpritesLoadedSuccessfully( void ) const { return m_spritesLoadedSuccessfully; }
	SDL_Surface* LoadSurface( std::string path );

	std::map<std::string, SDL_Surface*>& GetSDLSurfaceMap( void ) { return m_SDLSurfaceMap; }

	struct SpriteInfo
	{
		std::string m_name;
		std::string m_path;
	};

private:
	void LoadSprites( void );

	bool m_spritesLoadedSuccessfully;
	std::map<std::string, SDL_Surface*> m_SDLSurfaceMap;
};

#endif
