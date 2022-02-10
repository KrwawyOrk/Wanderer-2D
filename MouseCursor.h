#ifndef MOUSECURSOR_H
#define MOUSECURSOR_H

#include "Sprite.h"

enum mouseCursorState_t
{
	NORMAL,
	MALEE_ATTACK,
	PISTOL_ATTACK,
	HAND
};

class MouseCursor
{
public:
	MouseCursor(void);
	~MouseCursor(void);

	void Draw( void );

	mouseCursorState_t GetState( void ) const
	{
		return m_state;
	}

	void SetState( mouseCursorState_t state )
	{
		m_state = state;
	}
	
private:
	mouseCursorState_t m_state;
	Sprite m_cursorSprites[4];
};

#endif
