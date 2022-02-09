#include "Tile.h"

#include "Globals.h"
#include "SpriteManager.h"

Tile::Tile( Position& position, std::string spriteName )
{
	m_position.x = position.x;
	m_position.y = position.y;
	m_protectionZone = false;
	m_fogofwar.SetPosition( m_position );

	Globals::spriteManager->GetSprite( m_sprite, spriteName );
	Globals::spriteManager->GetSprite( m_protectionZoneSprite, "brush" ); 
}

void Tile::Think( void )
{
	m_fogofwar.Think();
}

void Tile::Draw( void )
{
	m_sprite.Draw( Globals::screen, m_position );

	if( m_protectionZone )
	{
		m_protectionZoneSprite.Draw( Globals::screen, m_position );
	}
}
