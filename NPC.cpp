#include "NPC.h"

#include "Player.h"

NPC::NPC()
{
	m_talkSprite = NULL;
	m_visited = false;
	m_questCompleted = false;
}

//----------------------------------------------------------------------------------------
JasonPorter::JasonPorter()
{

}

void JasonPorter::OnVisit( Player* player )
{
	if( m_visited == false )
	{
		m_talkSprite = &m_helloSprite;
		m_visited = true;
	}

	else if( m_visited == true && m_questCompleted == false)
	{
		m_talkSprite = &m_questInProgressSprite;
	}

	else if( m_visited == true && m_questCompleted == true )
	{
		m_talkSprite = &m_questCompletedSprite;
	}
}