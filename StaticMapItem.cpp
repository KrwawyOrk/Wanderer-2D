#include "StaticMapItem.h"

#include "Globals.h"
#include "SpriteManager.h"

StaticMapItem::StaticMapItem( Position& position, std::string name, bool walkable, std::string spriteName )
{
	m_position.x = position.x;
	m_position.y = position.y;
	flposition_x = static_cast<float>( m_position.x * Globals::tilesize );
	flposition_y = static_cast<float>( m_position.y * Globals::tilesize );
	m_name = name;
	m_walkable = walkable;

	Globals::spriteManager->GetSprite( m_sprite, spriteName );
}
