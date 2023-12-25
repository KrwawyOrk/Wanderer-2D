#include "GSBattleFight.h"

#include "Game.h"
#include "Globals.h"
#include "GSPlaying.h"
#include "SpriteManager.h"

#include <iostream>

GSBattleFight::GSBattleFight()
{
	Globals::spriteManager->GetSprite( m_background, "bitwa_tlo" );
	m_player = Globals::player;

	m_attackButton = new Button( 230, 290, "button_attack" );
}

GSBattleFight::~GSBattleFight()
{
	delete m_attackButton;
}

void GSBattleFight::InputEvents( void )
{
	if( m_attackButton->ButtonClicked() )
	{
		m_player->NotAttackingMonster();
		Globals::game->SetGameState( "Play" );

		OnQuit();
	}

	if( Globals::event.type == SDL_KEYDOWN )
	{
		switch( Globals::event.key.keysym.sym )
		{
		case SDLK_ESCAPE:
			m_player->NotAttackingMonster();
			Globals::game->SetGameState( "Play" );
			OnQuit();
			break;

		case SDLK_1:
			Attack();
		}
	}
}

void GSBattleFight::Think()
{

}

void GSBattleFight::Update( float deltaTime )
{

}

void GSBattleFight::Draw( void )
{
	GSPlaying* playing = dynamic_cast<GSPlaying*>( Globals::game->GetGameState( "Play" ) );
	if( playing )
	{
		playing->Draw();
	}

	m_background.Draw( Globals::screen, 192, 68 );
	m_player->GetSprite().Draw( Globals::screen, 370, 135 );
	
	Monster* attackedMonster = m_player->GetAttackedMonster();
	if( attackedMonster )
	{
		attackedMonster->GetSprite().Draw( Globals::screen, 545, 135 );
	}

	m_attackButton->DrawButton();
}

void GSBattleFight::OnQuit( void )
{
}

void GSBattleFight::Attack( void )
{
	Monster* attackedMonster = m_player->GetAttackedMonster();
	if( attackedMonster )
	{
		attackedMonster->SetHealthPoints( attackedMonster->GetHealthPoints() - 50 );

		if( !attackedMonster->IsAlive() )
		{
			attackedMonster->Die();
			m_player->NotAttackingMonster();
			Globals::game->SetGameState( "Play" );
		}
	}
}