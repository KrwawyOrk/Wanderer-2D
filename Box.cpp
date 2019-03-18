#include "Box.h"

#include "Globals.h"
#include "SpriteManager.h"

Box::Box()
{
	Globals::spriteManager->GetSprite( m_sprite, "box" );	
}
	