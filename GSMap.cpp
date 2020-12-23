#include "GSMap.h"

#include "Globals.h"
#include "SpriteManager.h"

GSMap::GSMap(void)
{
	Globals::spriteManager->GetSprite( m_map, "worldmap" );
}


GSMap::~GSMap(void)
{
}

void GSMap::Draw( void )
{
	m_map.Draw( Globals::screen, 400, 400 );
}


