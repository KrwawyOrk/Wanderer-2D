#include "Item.h"

#include "Globals.h"
#include "Player.h"
#include "SpriteManager.h"

Item::Item( Position position, itemType_t itemType )
{
	m_picked = false;
	SetPosition( position );
	SetFloatX( static_cast<float>( m_position.x * Globals::tilesize ) );
	SetFloatY( static_cast<float>( m_position.y * Globals::tilesize ) );

	switch( itemType )
	{
	case HEALTH_REGENERATION:
		m_itemType = HEALTH_REGENERATION;
		Globals::spriteManager->GetSprite( m_sprite, "health_info" );
		break;
	}	
}

Item::~Item()
{
	std::cout << "delete item" << std::endl;
}
	
void Item::OnUse( Player* player )
{
	switch( m_itemType )
	{
	case HEALTH_REGENERATION:
		player->Heal( 10 );
		break;
	}
}

void Item::OnEquip( Player* player )
{

}

void Item::OnRemove( Player* player )
{

}