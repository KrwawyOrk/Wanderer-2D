#include "PlayerBelt.h"

#include "Button.h"
#include "BitmapFont.h"
#include "Container.h"
#include "Game.h"
#include "Globals.h"
#include "Item.h"
#include "ItemSlot.h"
#include "Player.h"
#include "ProgressBar.h"
#include "SpriteManager.h"

#include <iostream>
#include <sstream>

PlayerBelt::PlayerBelt()
{
	Globals::spriteManager->GetSprite( m_bottomBelt, "bottom_belt_pixel" );
	Globals::spriteManager->GetSprite( m_top, "gui_top" );
	Globals::spriteManager->GetSprite( m_bottom, "inventory_background" );
	Globals::spriteManager->GetSprite( m_left, "gui_left" );
	Globals::spriteManager->GetSprite( m_right, "gui_right" );
	Globals::spriteManager->GetSprite( m_detail01, "detail01" );
	Globals::spriteManager->GetSprite( m_pistol, "pistol" );
	Globals::spriteManager->GetSprite( m_crowbar, "crowbar" ); //todo - get a crowbar sprite...
	Globals::spriteManager->GetSprite( m_inventoryBelt, "inventory_belt" );
	Globals::spriteManager->GetSprite( m_playerHealthBar, "player_healthbar" );
	Globals::spriteManager->GetSprite( m_gui_background, "gui_background" );

	Globals::spriteManager->GetSprite( m_lowhealthpoints, "lowhealthpoints" );
	Globals::spriteManager->GetSprite( m_gameplaygui, "gameplaygui" );

	m_food = new Text( 14, WHITE, Position( 25, 650 ) );
	//m_day = new Text( 14, WHITE, Position( 25, 680 ) );

	m_toolbox = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_junk = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_pistolAmmunition = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_experiencePoints = new BitmapFont( FontStyle::FONT_WHITE_SMALL );
	m_damage = new BitmapFont( FontStyle::FONT_WHITE_SMALL );
	m_monstersKilled = new BitmapFont( FontStyle::FONT_WHITE_SMALL );

	m_healthPoints = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_flashlightBattery = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );

	m_inventoryButton = new Button( 1155, 390, "inventory_button", true );
	m_quitButton = new Button( 1220, 390, "quit_button", true );

	//Tworzymy sloty itemow, dwa rzedy po 10 slotow.
	int draw_x_start = 15;
	int draw_x = draw_x_start;
	int draw_y = 240;
	int itemSlotsInRow = 2;

	for( int i = 0 ; i < INVENTORY_LIMIT ; i++ )
	{
		if( i % itemSlotsInRow )
		{
			draw_x += 51;
		}

		else
		{
			if( i >= itemSlotsInRow )
			{
				draw_x = draw_x_start;
				draw_y += 51;
			}
		}

		m_itemSlots.push_back( new ItemSlot( draw_x, draw_y, itemSlotType::INVENTORY_SLOT ) );
	}

	m_containerSlotTest = new ItemSlot( 50, 50, itemSlotType::CONTAINER_SLOT );
	m_containerSlotTest->SetItem( new Item( Position( 5, 5 ), HEALTH_REGENERATION ) );

	m_container = new Container;
	m_container->AddItem( new Item( Position( 5, 5 ), HEALTH_REGENERATION ) );
	m_container->AddItem( new Item( Position( 5, 5 ), HEALTH_REGENERATION ) );
	m_container->AddItem( new Item( Position( 5, 5 ), HEALTH_REGENERATION ) );
	m_container->AddItem( new Item( Position( 5, 5 ), HEALTH_REGENERATION ) );

	m_playerBeltState = playerBeltState_t::STATISTIC_STATE;

	m_interactAction = new Button( 50, 200, "interact_action_icon", true );
}

PlayerBelt::~PlayerBelt()
{
	delete m_healthPoints;
	m_healthPoints = NULL;

	delete m_food;
	m_food = NULL;

	delete m_toolbox;
	m_toolbox = NULL;

	delete m_junk;
	m_junk = NULL;

	delete m_healthPoints;
	m_healthPoints = NULL;

	delete m_pistolAmmunition;
	m_pistolAmmunition = NULL;

	delete m_experiencePoints;
	m_experiencePoints = NULL;

	delete m_inventoryButton;
	m_inventoryButton = NULL;

	delete m_quitButton;
	m_quitButton = NULL;

	delete m_flashlightBattery;
	m_flashlightBattery = NULL;

	for( std::vector<ItemSlot*>::iterator it = m_itemSlots.begin() ; it != m_itemSlots.end() ; ++it )
	{
		delete ( *it );
		( *it ) = NULL;
	}

	delete m_containerSlotTest;
	m_containerSlotTest = NULL;
}

void PlayerBelt::InputEvents( void )
{
	if( m_inventoryButton->ButtonClicked() )
	{
		std::cout << "Clicked inventory button!" << std::endl;
	}

	if( m_quitButton->ButtonClicked() )
	{
		Globals::game->Drop();
	}
}

void PlayerBelt::Think( void )
{
	for( std::vector<ItemSlot*>::iterator it = m_itemSlots.begin() ; it != m_itemSlots.end() ; ++it )
	{
		( *it )->Think();
	}

	m_containerSlotTest->Think();
	//m_container->Think();
}

void PlayerBelt::Draw( void )
{
	//m_interactAction->DrawButton();

	//m_gameplaygui.Draw( Globals::screen, 0, 0 );

	//m_top.Draw( Globals::screen, 0, 0 );
	
	//m_left.Draw( Globals::screen, 0, 40 );
	//m_right.Draw( Globals::screen, 1004, 40 );
	//m_detail01.Draw( Globals::screen, 0, 0 );

	//m_gui_background.Draw( Globals::screen, 434, 698 );

	//m_food->DrawText( Globals::screen );

	/*std::ostringstream sstoolbox;
	sstoolbox << "Narzedzia " << m_player->GetToolBox();
	m_toolbox->show_text( 25, 675, sstoolbox.str(), Globals::screen );

	std::ostringstream ssjunk;
	ssjunk << "Zlom " << m_player->GetJunk();
	m_junk->show_text( 25, 700, ssjunk.str(), Globals::screen );*/

	DrawSelectedWeapon();
	DrawBeltCards();
	DrawStatsInformations();

	//std::string testmsg = "Welcome! I think you\nwill ready to hack!\n";
	//m_experiencePoints->show_text( 1155, 350, "Welcome! I think you\nwill ready to hack!\nUse your knowledge of\nprogramming to open doors.", Globals::screen );

	//m_inventoryButton->DrawButton();
	//m_quitButton->DrawButton();

	//m_inventoryBelt.Draw( Globals::screen, 522, 744 );
	//DrawPlayerHealthBar(480, 100);

	//m_bottom.Draw( Globals::screen, 1155, 435 );

	//m_containerSlotTest->Draw();
	//m_container->Draw();

	//DrawConditionAlerts();
	//m_bottomBelt.Draw( Globals::screen, 0, 639 );
}

void PlayerBelt::InsertItemToInventory( Item* item )
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

void PlayerBelt::SortInventory( void )
{
	std::vector<Item*> &inventory = m_player->GetItems();
	inventory.erase( std::remove( inventory.begin(), inventory.end(), nullptr ), inventory.end() );
}

void PlayerBelt::DrawPlayerHealthBar( int position_x, int position_y ) // x = 525, y = 715
{
	SDL_Rect rect[2];
	rect[0].x = 0;
	rect[0].y = m_playerHealthBar.GetSDLSurface()->h / 2; //rect[0].y = 15;
	rect[0].w = ( m_player->GetHealthPoints() * 100 / m_player->GetMaxHealthPoints() ) * m_playerHealthBar.GetSDLSurface()->w / 100;
	rect[0].h = m_playerHealthBar.GetSDLSurface()->h / 2; //rect[0].h = 15;

	rect[1].x = 0;
	rect[1].y = 0;
	rect[1].w = m_playerHealthBar.GetSDLSurface()->w;
	rect[1].h = m_playerHealthBar.GetSDLSurface()->h / 2;

	m_playerHealthBar.Draw( Globals::screen, position_x, position_y, &rect[1] );
	m_playerHealthBar.Draw( Globals::screen, position_x, position_y, &rect[0] );

	std::ostringstream sshp;
	sshp << m_player->GetHealthPoints() << " / " << m_player->GetMaxHealthPoints();
	m_healthPoints->show_text( position_x + 220, position_y, sshp.str(), Globals::screen );
}

void PlayerBelt::DrawConditionAlerts( void )
{
	if (m_player->GetHealthPoints() <= 10)
	{
		m_lowhealthpoints.Draw( Globals::screen, 50, 50 );
	}
}

void PlayerBelt::DrawStatsInformations( void )
{
	std::ostringstream sshealthpoints;
	sshealthpoints << "HP: " << m_player->GetHealthPoints() << " / " << m_player->GetMaxHealthPoints();
	m_healthPoints->show_text( 15, 15, sshealthpoints.str(), Globals::screen );

	std::ostringstream sspistolammunition;
	sspistolammunition << "AMMO: " << m_player->GetPistolAmmunition();
	m_pistolAmmunition->show_text( 15, 65, sspistolammunition.str(), Globals::screen );

	std::ostringstream ssflashlightbattery;
	ssflashlightbattery << "FLASH: " << static_cast<int>(m_player->GetFlashlightBattery() * 100) << "%";
	m_flashlightBattery->show_text( 15, 115, ssflashlightbattery.str(), Globals::screen );
}

void PlayerBelt::DrawInventorySlots( void )
{
	for (std::vector<ItemSlot*>::iterator it = m_itemSlots.begin(); it != m_itemSlots.end(); ++it)
	{
		(*it)->Draw();
	}
}

void PlayerBelt::DrawBeltCards( void )
{
	//switch (m_playerBeltState)
	//{
	//case playerBeltState_t::STATISTIC_STATE:
	//	//DrawStatsInformations();
	//	break;

	//case playerBeltState_t::INVENTORY_STATE:
	//	DrawInventorySlots();
	//	break;
	//}

	DrawInventorySlots();
}

void PlayerBelt::DrawSelectedWeapon( void )
{
	switch (m_player->GetWeaponType())
	{
	case PISTOL:
		m_pistol.Draw( Globals::screen, 145, 830 );
		break;

	case CROWBAR:
		m_crowbar.Draw( Globals::screen, 145, 830 );
		break;
	};

}

void PlayerBelt::SetPlayerBeltState( playerBeltState_t playerBeltState )
{
	m_playerBeltState = playerBeltState;
}