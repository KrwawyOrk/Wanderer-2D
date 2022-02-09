#include "GameCursor.h"

#include "Globals.h"
#include "SpriteManager.h"


GameCursor::GameCursor()
{
	Globals::spriteManager->GetSprite( m_cursor, "cursor" );
	m_cursorState = IDLE;
}

void GameCursor::Draw( void )
{
	switch( m_cursorState )
	{
	case IDLE:
		m_cursor.Draw( Globals::screen, pos_x, pos_y );
		break;
	}
}

void GameCursor::Think( void )
{
	SDL_GetMouseState( &pos_x, &pos_y );
}
