#include "StaticMapItem.h"

#include "Camera.h"
#include "Globals.h"
#include "SpriteManager.h"

StaticMapItem::StaticMapItem( Position position, std::string name, bool walkable, std::string spriteName = "unnamed" )
{
	m_position.x = position.x;
	m_position.y = position.y;
	flposition_x = static_cast<float>( m_position.x * Globals::tilesize );
	flposition_y = static_cast<float>( m_position.y * Globals::tilesize );
	m_name = name;
	m_walkable = walkable;
	m_container = NULL;

	Globals::spriteManager->GetSprite( m_sprite, spriteName );
}

void StaticMapItem::Draw( void )
{
	int spriteWidth = m_sprite.GetSDLSurface()->w;
	int spriteHeigh = m_sprite.GetSDLSurface()->h;

	int draw_x = static_cast<int>( flposition_x - Globals::camera->GetCameraX() );
	int draw_y = static_cast<int>( flposition_y - Globals::camera->GetCameraY() - m_sprite.GetSDLSurface()->h + Globals::tilesize );
	
	if( spriteWidth == Globals::tilesize )
	{
		m_sprite.Draw( Globals::screen, draw_x, draw_y );
	}

	else if( spriteWidth > Globals::tilesize )
	{
		SDL_Rect src;
		src.x = Globals::tilesize * int(((SDL_GetTicks() / Globals::spriteAnimationSpeed) % ( spriteWidth / Globals::tilesize ) ));
		src.y = 0;
		src.w = Globals::tilesize;
		src.h = spriteHeigh;

		m_sprite.Draw( Globals::screen, draw_x, draw_y, &src );
	}
}

bool StaticMapItem::HasContainer( void )
{
	if( m_container )
	{
		return true;
	}

	else
	{
		return false;
	}
}

void StaticMapItem::SetContainer( Container* container )
{
	if( container && m_container == NULL )
	{
		m_container = container;
	}
}