#ifndef NPC_H
#define NPC_H

#include "GameObject.h"

class Player;
class Sprite;

class NPC : public GameObject
{
public:
	NPC();

	virtual void OnVisit( Player* player ) = 0;
	const Sprite* GetTalkSprite( void ) const
	{
		return m_talkSprite;
	}

	void CloseTalkSprite( void )
	{
		m_talkSprite = NULL;
	}

protected:
	bool m_visited;
	bool m_questCompleted;

	Sprite* m_talkSprite;

	Sprite m_helloSprite;
	Sprite m_questInProgressSprite;
	Sprite m_questCompletedSprite;
};

//----------------------------------------------------------------------------------------
class JasonPorter : public NPC
{
public:
	JasonPorter();

	void OnVisit( Player* player );
};

#endif