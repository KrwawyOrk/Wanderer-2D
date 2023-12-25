#include "Item.h"

#include "Camera.h"
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

	case EXPERIENCE_BOOK:
		m_itemType = EXPERIENCE_BOOK;
		Globals::spriteManager->GetSprite( m_sprite, "experience_book" );
		break;

	case AMMO_PACK:
		m_itemType = AMMO_PACK;
		Globals::spriteManager->GetSprite( m_sprite, "ammo_pack" );
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
		player->Heal( player->GetSkills().m_medicine * 10 );
		break;

	case EXPERIENCE_BOOK:
		player->GiveExperiencePoints( 200 );
		break;

	case AMMO_PACK:
		player->GiveAmmunition( 5 );
		break;
	}
}

void Item::OnEquip( Player* player )
{

}

void Item::OnRemove( Player* player )
{

}