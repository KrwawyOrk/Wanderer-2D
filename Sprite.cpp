#include "Sprite.h"

#include "Camera.h"
#include "Position.h"
#include "Globals.h"

#include "SDL_image.h"
#include <iostream>

Sprite::Sprite()
{
	m_surface = NULL;
}

void Sprite::Draw( SDL_Surface* screen, int x, int y )
{
	if( m_surface )
	{
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;

		SDL_BlitSurface( m_surface, NULL, screen, &rect );
	}
}

void Sprite::Draw( SDL_Surface* screen, Position& position )
{
	if( m_surface )
	{
		SDL_Rect rect;

		rect.x = position.x * Globals::tilesize - Globals::camera->GetCameraX();
		rect.y = position.y * Globals::tilesize - Globals::camera->GetCameraY() - m_surface->h + Globals::tilesize;

		SDL_BlitSurface( m_surface, NULL, screen, &rect );
	}
}

void Sprite::Draw( SDL_Surface* screen, int x, int y, SDL_Rect& healthPercent )
{
	if( m_surface )
	{
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;

		SDL_BlitSurface( m_surface, &healthPercent, screen, &rect );
	}
}
