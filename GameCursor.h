#include "SDL.h"

#include "Sprite.h"

enum cursorState_t
{
	IDLE = 1,
	MOVEPLAYER_NORTH,
	MOVEPLAYER_SOUTH,
	MOVEPLAYER_EAST,
	MOVEPLAYER_WEST,
	ATTACK
};

class GameCursor
{
public:
	GameCursor();
	
	void Think();
	void Draw();

private:
	Sprite m_cursor;
	cursorState_t m_cursorState;
	int pos_x;
	int pos_y;
};

