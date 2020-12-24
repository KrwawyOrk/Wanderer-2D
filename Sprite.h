#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include <string>

class Position;
class SpriteManager;

class Sprite
{
public:
	Sprite();

	virtual void Draw( SDL_Surface* screen, int x, int y );
	virtual void Draw( SDL_Surface* screen, Position& position );

	//use this function for animations... :-)
	virtual void DrawAnimatedOnMapPosition( SDL_Surface* screen, Position& position );
	virtual void DrawAnimatedOnScreenPosition( SDL_Surface* screen, int x, int y );

	virtual void Draw( SDL_Surface* screen, int x, int y, SDL_Rect* clip );

	SDL_Surface* GetSDLSurface( void ) const { return m_surface; }
	void SetSDLSurface( SDL_Surface* surface ) { m_surface = surface; }

	std::string GetSpriteName( void ) const { return m_name; }

private:
	SDL_Surface* m_surface;
	std::string m_name;

	friend class SpriteManager;
};

#endif
