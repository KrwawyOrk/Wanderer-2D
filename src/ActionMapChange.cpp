#include "ActionMapChange.h"

ActionMapChange::ActionMapChange( Position position, std::string nextMapName, Position landPosition )
{
	m_position.x = position.x;
	m_position.y = position.y;

	m_nextMapName = nextMapName;
	m_landPosition.x = landPosition.x;
	m_landPosition.y = landPosition.y;
}
