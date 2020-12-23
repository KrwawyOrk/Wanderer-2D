#include "Skill.h"

Skill::Skill( skillTypes::skillType_t skillType, bool learned, int skillLevel )
{
	m_skillType = skillType;
	m_learned = learned;
	m_skillLevel = skillLevel;
}
