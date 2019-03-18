#include "PlayerBelt.h"

#include "Button.h"
#include "BitmapFont.h"
#include "Game.h"
#include "Globals.h"
#include "ItemSlot.h"
#include "Player.h"
#include "SpriteManager.h"

#include <iostream>
#include <sstream>

PlayerBelt::PlayerBelt()
{
	Globals::spriteManager->GetSprite( m_top, "gui_top" );
	Globals::spriteManager->GetSprite( m_bottom, "gui_bottom" );
	Globals::spriteManager->GetSprite( m_left, "gui_left" );
	Globals::spriteManager->GetSprite( m_right, "gui_right" );
	Globals::spriteManager->GetSprite( m_detail01, "detail01" );
	Globals::spriteManager->GetSprite( m_pistol, "pistol" );
	Globals::spriteManager->GetSprite( m_crowbar, "crowbar" ); //todo - get a crowbar sprite...
	Globals::spriteManager->GetSprite( m_inventoryBelt, "inventory_belt" );

	m_food = new Text( 14, WHITE, Position( 25, 650 ) );
	//m_day = new Text( 14, WHITE, Position( 25, 680 ) );

	m_healthPoints = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_movementPoints = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_toolbox = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_junk = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_information = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_pistolAmmunition = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );
	m_experiencePoints = new BitmapFont( FontStyle::FONT_WHITE_SMALL );
	m_damage = new BitmapFont( FontStyle::FONT_WHITE_SMALL );

	m_inventoryButton = new Button( 590, 700, "inventory_button" );
	m_quitButton = new Button( 965, 0, "quit_button", true );

	//Tworzymy sloty itemow, dwa rzedy po 10 slotow.
	int draw_x = 10;
	int draw_y = 760;
	int itemSlotsInRow = 10;

	for( int i = 0 ; i < 20 ; i++ )
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

		m_itemSlots.push_back( new ItemSlot( draw_x, draw_y ) );
	}
}

PlayerBelt::~PlayerBelt()
{
	delete m_healthPoints;
	m_healthPoints = NULL;

	delete m_movementPoints;
	m_movementPoints = NULL;

	delete m_food;
	m_food = NULL;

	delete m_toolbox;
	m_toolbox = NULL;

	delete m_junk;
	m_junk = NULL;

	delete m_information;
	m_information = NULL;

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

	for( std::vector<ItemSlot*>::iterator it = m_itemSlots.begin() ; it != m_itemSlots.end() ; ++it )
	{
		delete ( *it );
		( *it ) = NULL;
	}
}

void PlayerBelt::InputEvents( void )
{
	if( m_inventoryButton->ButtonClicked() )
	{
		Globals::game->SetGameState( "Craft menu" );
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
}

void PlayerBelt::Draw( void )
{
	//m_top.Draw( Globals::screen, 0, 0 );
	//m_bottom.Draw( Globals::screen, 0, 615 );
	//m_left.Draw( Globals::screen, 0, 40 );
	//m_right.Draw( Globals::screen, 1004, 40 );
	//m_detail01.Draw( Globals::screen, 0, 0 );

	m_food->DrawText( Globals::screen );

	m_information->show_text( 300, 10, "Technik - gra w fazie development.", Globals::screen );

	std::ostringstream sshp;
	sshp << m_player->GetHealthPoints() << " / " << m_player->GetMaxHealthPoints();
	m_healthPoints->show_text( 190, 670, sshp.str(), Globals::screen ); //trzeba teraz wspolrzedne dobrze trafic ^_^

	std::ostringstream ssmp;
	ssmp << m_player->GetActionPoints() << " / " << m_player->GetMaxActionPoints();
	m_movementPoints->show_text( 190, 725, ssmp.str(), Globals::screen );

	/*std::ostringstream sstoolbox;
	sstoolbox << "Narzedzia " << m_player->GetToolBox();
	m_toolbox->show_text( 25, 675, sstoolbox.str(), Globals::screen );

	std::ostringstream ssjunk;
	ssjunk << "Zlom " << m_player->GetJunk();
	m_junk->show_text( 25, 700, ssjunk.str(), Globals::screen );*/

	std::ostringstream sspistolammunition;
	sspistolammunition << m_player->GetPistolAmmunition();
	m_pistolAmmunition->show_text( 955, 716, sspistolammunition.str(), Globals::screen );

	switch( m_player->GetWeaponType() )
	{
	case PISTOL:
		m_pistol.Draw( Globals::screen, 775, 628 );
		break;

	case CROWBAR:
		m_crowbar.Draw( Globals::screen, 775, 628 );
		break;
	};

	std::ostringstream ssexperiencepoints;
	ssexperiencepoints << m_player->GetExperiencePoints();
	m_experiencePoints->show_text( 375, 650, "EXPERIENCE POINTS " + ssexperiencepoints.str(), Globals::screen );

	std::ostringstream ssdamage;
	ssdamage << m_player->GetBaseDamage() << " [" << m_player->GetWeaponDamage() << "]";

	m_damage->show_text( 375, 670, "DAMAGE " + ssdamage.str(), Globals::screen );

	m_inventoryButton->DrawButton();
	m_quitButton->DrawButton();

	m_inventoryBelt.Draw( Globals::screen, 5, 732 );

	for( std::vector<ItemSlot*>::iterator it = m_itemSlots.begin() ; it != m_itemSlots.end() ; ++it )
	{
		( *it )->Draw();
	}
}