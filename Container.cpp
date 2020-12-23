#include "Container.h"

#include "Button.h"
#include "ItemSlot.h"
#include "Position.h"

Container::Container()
{
	m_locked = false;

	int draw_x = 10;
	int draw_y = 150;
	int itemSlotsInRow = 2;

	for( int i = 0 ; i < 10 ; i++ )
	{
		if( i % itemSlotsInRow )
		{
			draw_x += 51;
		}

		else
		{
			if( i >= itemSlotsInRow )
			{
				draw_x = 10;
				draw_y += 51;
			}
		}

		m_itemSlots.push_back( new ItemSlot( draw_x, draw_y, itemSlotType::CONTAINER_SLOT ) );
	}

	
}

Container::~Container()
{
	delete m_exitButton;
}

void Container::InputEvents( void )
{
	
}

void Container::Think( void )
{
	for( std::vector<ItemSlot*>::iterator it = m_itemSlots.begin() ; it != m_itemSlots.end() ; ++it )
	{
		( *it )->Think();
	}
}

void Container::Draw( void )
{
	for( std::vector<ItemSlot*>::iterator it = m_itemSlots.begin() ; it != m_itemSlots.end() ; ++it )
	{
		( *it )->Draw();
	}
}

void Container::AddItem( Item* item )
{
	if( item )
	{
		for( std::vector<ItemSlot*>::iterator iter = GetItemSlots().begin() ; iter != GetItemSlots().end() ; ++iter )
		{
			ItemSlot* slot = ( *iter );
			if( slot->HasItem() == false )
			{
				slot->SetItem( item );
				break;
			}

		}
	}
}

bool Container::HasSpaceInContainer( void )
{
	if( m_itemSlots.size() < 20 )
	{
		return true;
	}

	else
	{
		std::cout << "Nie ma miejsca w ekwipunku." << std::endl;
		return false;
	}
}