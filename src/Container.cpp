#include "Container.h"

#include "Button.h"
#include "ItemSlot.h"
#include "Position.h"
#include "SpriteManager.h"

Container::Container()
{
	m_locked = false;
	m_windowVisible = true;
	Globals::spriteManager->GetSprite(m_background, "opened_container");
	m_exitButton = new Button( 585, 335, "close_button", true );

	int draw_x = 595;
	int draw_y = 405;
	int itemSlotsInRow = 5;

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
				draw_x = 595;
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
	if (isWindowVisible())
	{
		for (std::vector<ItemSlot*>::iterator it = m_itemSlots.begin(); it != m_itemSlots.end(); ++it)
		{
			(*it)->Think();
		}

		if (m_exitButton->ButtonClicked())
		{
			setWindowVisible( !isWindowVisible() );
		}
	}
}

void Container::Draw( void )
{
	if ( isWindowVisible() )
	{
		m_background.Draw( Globals::screen, 585, 375 );
		for (std::vector<ItemSlot*>::iterator it = m_itemSlots.begin(); it != m_itemSlots.end(); ++it)
		{
			(*it)->Draw();
		}

		m_exitButton->DrawButton();
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