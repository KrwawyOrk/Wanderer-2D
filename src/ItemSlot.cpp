#include "ItemSlot.h"

#include "Item.h"
#include "Player.h"
#include "Sprite.h"
#include "SpriteManager.h"

ItemSlot::ItemSlot( int x, int y, itemSlotType::itemSlotType_t itemSlotType ) : Button( x, y, "itemslot" )
{
	m_item = NULL;
	m_selected = false;
	m_itemSlotType = itemSlotType;
	Globals::spriteManager->GetSprite( m_selectedSprite, "itemslotselected" );
}

void ItemSlot::SetItem( Item* item )
{
	m_item = item;
}

void ItemSlot::RemoveItem( void )
{
	delete m_item;
	m_item = NULL;
}

void ItemSlot::Think( void )
{
	if( ButtonHover() )
	{
		m_selected = true;
	}

	else
	{
		m_selected = false;
	}

	if( ButtonClicked() )
	{
		if( m_item )
		{
			Player* player = Globals::player;
			std::vector<Item*> &items = player->GetItems();
			
			if( m_itemSlotType == itemSlotType::INVENTORY_SLOT )
			{
				m_item->OnUse( player );

				for( std::vector<Item*>::iterator it = items.begin() ; it != items.end() ; )
				{
					Item* item = ( *it );
					if( item == GetItemSlot() )
					{
						it = items.erase( it );

						delete m_item;
						m_item = NULL;
					}

					else
						++it;
				}
			}

			else if( m_itemSlotType == itemSlotType::CONTAINER_SLOT )
			{
				if( player->HasSpaceInInventory() )
				{
					player->GiveItem( m_item );
					m_item = NULL;
				}
			}
		}
	}
}

void ItemSlot::Draw( void )
{
	DrawButton();

	if( m_item )
	{
		m_item->GetSprite().DrawAnimatedOnScreenPosition( Globals::screen, position_x, position_y );
	}

	if( m_selected )
	{
		m_selectedSprite.DrawAnimatedOnScreenPosition( Globals::screen, position_x, position_y );
	}
}