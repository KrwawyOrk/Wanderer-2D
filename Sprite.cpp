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

void Sprite::DrawAnimatedOnMapPosition( SDL_Surface* screen, Position& position, int animationSpeed )
{
	if( m_surface )
	{
		SDL_Rect dst;
		dst.x = position.x * Globals::tilesize - Globals::camera->GetCameraX();
		dst.y = position.y * Globals::tilesize - Globals::camera->GetCameraY() - m_surface->h + Globals::tilesize;

		SDL_Rect src;
		src.x = Globals::tilesize * int(((SDL_GetTicks() / animationSpeed) % ( m_surface->w / Globals::tilesize ) ));
		src.y = 0;
		src.w = Globals::tilesize;
		src.h = m_surface->h;

		SDL_BlitSurface( m_surface, &src, screen, &dst );
	}
}

void Sprite::DrawAnimatedOnScreenPosition( SDL_Surface* screen, int x, int y, int animationSpeed )
{
	if( m_surface )
	{
		SDL_Rect dst;
		dst.x = x;
		dst.y = y;

		SDL_Rect src;
		src.x = Globals::tilesize * int(((SDL_GetTicks() / animationSpeed ) % ( m_surface->w / Globals::tilesize ) ));
		src.y = 0;
		src.w = Globals::tilesize;
		src.h = m_surface->h;

		SDL_BlitSurface( m_surface, &src, screen, &dst );
	}
}

void Sprite::Draw( SDL_Surface* screen, int x, int y, SDL_Rect* clip = NULL )
{
	if( m_surface )
	{
		SDL_Rect rect;

		rect.x = x;
		rect.y = y;

		SDL_BlitSurface( m_surface, clip, screen, &rect );
	}
}
