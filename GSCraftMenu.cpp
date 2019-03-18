#include "GSCraftMenu.h"

#include "Globals.h"
#include "Game.h"
#include "Camera.h"
#include "SpriteManager.h"
#include "Map.h"

#include <iostream>
#include <sstream>

GSCraftMenu::GSCraftMenu()
{
	m_player = Globals::player;
	m_craftRecipes = new BitmapFont( FontStyle::FONT_WHITE_SMALL );
	m_description = new BitmapFont( FontStyle::FONT_WHITE_SMALL );

	m_playerItemsText = new Text( 14, GOLD, Position( 5, 5 ) );
	std::ostringstream items;
	items << "Twoje zasoby | Skrzynia narzedziowa: " << m_player->GetToolBox() << " | Zlom: " << m_player->GetJunk();
	m_playerItemsText->SetText( items.str() );
	m_playerItemsText->Update();


	m_craftHealthKitText = new Text( 14, WHITE, Position( 100, 50 ) );
	m_craftHealthKitText->SetText( "[F1] Apteczka | +30 punkty zycia | Wymagania: 1 skrzynia narzedziowa" );
	m_craftHealthKitText->Update();

	m_craftToolBoxText = new Text( 14, WHITE, Position( 100, 110 ) );
	m_craftToolBoxText->SetText( "[F2] Skrzynia narzedziowa | Element do wytwarzania | Wymagania: 2 sztuki zlomu" );
	m_craftToolBoxText->Update();

	m_craftMorphicHammerText = new Text( 14, WHITE, Position( 100, 160 ) );
	m_craftMorphicHammerText->SetText( "[F3] Mlot Przemiany | Pozyskuje zlom | Wymagania: 2 skrzynie narzedziowe" );
	m_craftMorphicHammerText->Update();

	Globals::spriteManager->GetSprite( m_background, "worldmap" );
}

GSCraftMenu::~GSCraftMenu()
{
	delete m_playerItemsText;
	delete m_craftHealthKitText;
	delete m_craftToolBoxText;
	delete m_craftMorphicHammerText;
	delete m_craftRecipes;
}

void GSCraftMenu::InputEvents( void )
{
	if( Globals::event.type == SDL_KEYDOWN )
	{
		switch( Globals::event.key.keysym.sym )
		{
		case SDLK_F1:
			CraftHealthKit();
			break;

		case SDLK_F2:
			CraftToolBox();
			break;

		case SDLK_F3:
			CraftMorphicHammer();
			break;

		case SDLK_F4:
			LearnMiningAbility();
			break;

		case SDLK_ESCAPE:
			OnQuit();
			break;
		}
	}
}

void GSCraftMenu::Think( void )
{
	std::ostringstream items;
	items << "Twoje zasoby | Skrzynia narzedziowa: " << m_player->GetToolBox() << " | Zlom: " << m_player->GetJunk();
	m_playerItemsText->SetText( items.str() );
	m_playerItemsText->Update();
}

void GSCraftMenu::Draw( void )
{
	m_background.Draw( Globals::screen, 0, 0 );

	std::vector<Item*> &items = Globals::player->GetItems();

	int x = 50;
	int y = 50;
	for( std::vector<Item*>::const_iterator cit = items.begin() ; cit != items.end() ; ++cit )
	{
		Item* item = (*cit);
		item->GetSprite().Draw( Globals::screen, x, y );

		x += 60;
	}

	//m_playerItemsText->DrawText( Globals::screen );

	///*m_craftHealthKitText->DrawText( Globals::screen );
	//m_craftToolBoxText->DrawText( Globals::screen );
	//m_craftMorphicHammerText->DrawText( Globals::screen );*/

	//m_craftRecipes->show_text( 5, 200, "[F1] Apteczka | +30 punkty zycia | Wymagania: 1 skrzynia narzedziowa\n[F2] Skrzynia narzedziowa | Element do wytwarzania | Wymagania: 2 sztuki zlomu\n[F3] Mlot Przemiany | Pozyskuje zlom | Wymagania: 2 skrzynie narzedziowe\n[F4] Ulepsz Mlot Przemiany | Pozwala niszczyc sciany i wydobywac z nich zlom | Wymagania: 2 skrzynie narzedziowe, 3 szt zlomu", Globals::screen );
	//m_description->show_text( 5, 15, "Urzadzenie w ktore wpatrujesz swoj zmeczony wzrok to CRFT-8989.\nJest to drukarka trojwymiarowa, ktora potrafi tworzyc przedmioty\nale musisz wgrac odpowiednia\ndyskietke z programem materializujacym.", Globals::screen );
}

void GSCraftMenu::OnQuit( void )
{
	m_player->SetPosition( Globals::currentMap->GetStartPosition() );
	m_player->SetFloatX( static_cast<float>( m_player->GetPosition().x * Globals::tilesize ) );
	m_player->SetFloatY( static_cast<float>( m_player->GetPosition().y * Globals::tilesize ) );

	Globals::game->SetGameState( "Play" );
	Globals::camera->Center( Globals::currentPlayer );
}

void GSCraftMenu::CraftHealthKit( void )
{
	if( m_player->GetToolBox() >= 1 )
	{
		m_player->Heal( Globals::healthKitHeal );
		m_player->SetToolBox( m_player->GetToolBox() - 1 );
	}
}

void GSCraftMenu::CraftToolBox( void )
{
	if( m_player->GetJunk() >= 2 )
	{
		m_player->SetToolBox( m_player->GetToolBox() + 1 );
		m_player->SetJunk( m_player->GetJunk() - 2 );
	}
}

void GSCraftMenu::CraftMorphicHammer( void )
{
	if( !m_player->GetMorphicHammer() &&  m_player->GetToolBox() >= 2 )
	{
		m_player->SetMorphicHammer( true );
		m_player->SetToolBox( m_player->GetToolBox() - 2 );
	}
}

void GSCraftMenu::LearnMiningAbility( void )
{
	if( m_player->GetMorphicHammer() && m_player->GetJunk() >= 3 && m_player->GetToolBox() >= 2 )
	{
		m_player->SetMiningAbility( true );
		m_player->SetJunk( m_player->GetJunk() - 3 );
		m_player->SetToolBox( m_player->GetToolBox() - 2 );
	}
}