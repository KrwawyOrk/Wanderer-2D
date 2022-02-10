#include "MouseCursor.h"

#include "Globals.h"
#include "SpriteManager.h"


MouseCursor::MouseCursor(void)
{
	m_state = NORMAL;

	Globals::spriteManager->GetSprite( m_cursorSprites[NORMAL], "mouse_cursor_normal" );
	Globals::spriteManager->GetSprite( m_cursorSprites[NORMAL], "mouse_cursor_malee_attack" );
	Globals::spriteManager->GetSprite( m_cursorSprites[NORMAL], "mouse_cursor_pistol_attack" );
	Globals::spriteManager->GetSprite( m_cursorSprites[NORMAL], "mouse_cursor_hand" );
}


MouseCursor::~MouseCursor(void)
{
}

void MouseCursor::Draw( void )
{
	int mouse_x = Globals::event.motion.x;
	int mouse_y = Globals::event.motion.y;

	m_cursorSprites[m_state].Draw( Globals:: screen, mouse_x, mouse_y );
}
