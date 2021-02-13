#ifndef MONSTERHEALTHBAR_H
#define MONSTERHEALTHBAR_H

#include "SDL.h"
#include "Sprite.h"

class Monster;

class MonsterHealthBar
{
public:
	MonsterHealthBar( Monster* monster );

	void Draw();

private:
	void DrawHealthBarWhenAttackedByPlayer( SDL_Rect *rect );
	void DrawHealthBarOnMouseOver( SDL_Rect* rect );
	SDL_Rect* GetSDLRectangleForMonsterHealthBar( void );

	Monster* m_monster;

	Sprite m_attackingPlayerSprite;
	Sprite m_attackedByPlayerSprite;
	Sprite m_healthBarSprite;
};

#endif
