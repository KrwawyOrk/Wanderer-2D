#include "ItemSlot.h"

#include "Item.h"
#include "Player.h"
#include "Sprite.h"
#include "SpriteManager.h"

ItemSlot::ItemSlot( int x, int y ) : Button( x, y, "itemslot" )
{
	m_item = NULL;
	m_selected = false;
	Globals::spriteManager->GetSprite( m_selectedSprite, "itemslotselected" );
}

void ItemSlot::SetItem( Item* item )
{
	/*if( item )
	{
		if( GetItemSlot() )
		{
			return;
		}

		else
		{
			m_item = item;
		}
	}*/

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
			m_item->OnUse( player );

			std::vector<Item*> &items = player->GetItems();

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
	}
}

void ItemSlot::Draw( void )
{
	DrawButton();

	if( m_item )
	{
		m_item->GetSprite().Draw( Globals::screen, position_x, position_y );
	}

	if( m_selected )
	{
		m_selectedSprite.Draw( Globals::screen, position_x, position_y );
	}
}