#ifndef SKILL_H
#define SKILL_H

namespace skillTypes
{
	enum skillType_t
	{
		DOOR_OPENING = 1,
		INCREASED_VISIBILITY,
		ABILITY_TO_TALK,
		INCREASED_SPEED,
		REPAIR_AND_MECHANICS,
		HACKING,
		PICK_UP,
		MOVING_THINGS,
		BIGGER_BACKPACK,
		INCREASED_TREATMENT,
		READING_ABILITY
	};
}

class Skill
{
public:
	Skill( skillTypes::skillType_t skillType, bool learned, int skillLevel );

	skillTypes::skillType_t GetSkillType( void ) const
	{
		return m_skillType;
	}

	bool SkillIsLearned( void ) const
	{
		return m_learned;
	}

	void SetLearnedSkill( bool b )
	{
		m_learned = b;
	}

	void SetSkillLevel( int i )
	{
		m_skillLevel = i;
	}

	int GetSkillLevel( void ) const 
	{
		return m_skillLevel;
	}

private:
	skillTypes::skillType_t m_skillType;
	bool m_learned;
	int m_skillLevel;
};

#endif

