#include "MonsterHealthBar.h"

#include "Camera.h"
#include "Globals.h"
#include "Monster.h"
#include "SpriteManager.h"

MonsterHealthBar::MonsterHealthBar( Monster* monster )
{
	m_monster = monster;

	Globals::spriteManager->GetSprite( m_attackingPlayerSprite, "attacked02" );
	Globals::spriteManager->GetSprite( m_attackedByPlayerSprite, "attacked01" );
	Globals::spriteManager->GetSprite( m_healthBarSprite, "healthbar" );
}

void MonsterHealthBar::Draw( void )
{
	DrawHealthBarWhenAttackedByPlayer( GetSDLRectangleForMonsterHealthBar() );
	DrawHealthBarOnMouseOver( GetSDLRectangleForMonsterHealthBar() );
}

SDL_Rect* MonsterHealthBar::GetSDLRectangleForMonsterHealthBar( void )
{
	SDL_Rect rect[2];
	rect[0].x = 0;
	rect[0].y = 5;
	rect[0].w = ( m_monster->GetHealthPoints() * 100 / m_monster->GetMaxHealthPoints() ) * 50 / 100;
	rect[0].h = 5;

	rect[1].x = 0;
	rect[1].y = 0;
	rect[1].w = 50;
	rect[1].h = 5;

	return rect;
}

void MonsterHealthBar::DrawHealthBarWhenAttackedByPlayer( SDL_Rect *rect )
{
	if( m_monster->GetAttackedByPlayer() )
	{
		m_healthBarSprite.Draw( Globals::screen, static_cast<int>( m_monster->GetFloatX() - Globals::camera->GetCameraX() ), static_cast<int>( m_monster->GetFloatY() - 10 - Globals::camera->GetCameraY() ), &rect[1] );
		m_healthBarSprite.Draw( Globals::screen, static_cast<int>( m_monster->GetFloatX() - Globals::camera->GetCameraX() ), static_cast<int>( m_monster->GetFloatY() - 10 - Globals::camera->GetCameraY() ), &rect[0] );
	}
}

void MonsterHealthBar::DrawHealthBarOnMouseOver( SDL_Rect* rect )
{
	Position mouse;
	mouse.x = ( Globals::event.motion.x + Globals::camera->GetCameraX() ) / Globals::tilesize;
	mouse.y = ( Globals::event.motion.y + Globals::camera->GetCameraY() ) / Globals::tilesize;

	if( mouse.x == m_monster->GetPosition().x && mouse.y == m_monster->GetPosition().y )
	{
		if( !m_monster->GetAttackedByPlayer() )
		{
			m_attackingPlayerSprite.Draw( Globals::screen, static_cast<int>( m_monster->GetFloatX() - Globals::camera->GetCameraX() ), static_cast<int>( m_monster->GetFloatY() - Globals::camera->GetCameraY() ) );
		}

		m_healthBarSprite.Draw( Globals::screen, static_cast<int>( m_monster->GetFloatX() - Globals::camera->GetCameraX() ), static_cast<int>( m_monster->GetFloatY() - 10 - Globals::camera->GetCameraY() ), &rect[1] );
		m_healthBarSprite.Draw( Globals::screen, static_cast<int>( m_monster->GetFloatX() - Globals::camera->GetCameraX() ), static_cast<int>( m_monster->GetFloatY() - 10 - Globals::camera->GetCameraY() ), &rect[0] );
	}
}