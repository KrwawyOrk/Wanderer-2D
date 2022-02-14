#ifndef CREATURE_H
#define CREATURE_H

#include "GameObject.h"

class Creature : public GameObject
{
public:
	Creature();
	virtual ~Creature() {}

	int GetHealthPoints( void ) const { return m_healthPoints; }
	void SetHealthPoints( int healthPoints ) { m_healthPoints = healthPoints; }

	int GetMaxHealthPoints( void ) const { return m_maxHealthPoints; }
	void SetMaxHealthPoints( int maxHealthPoints ) { m_maxHealthPoints = maxHealthPoints; }

	virtual void Move( direction_t direction );
	virtual void Die( void ) { return; }
	virtual void Heal( int healPower );

	bool IsAttacked( void )
	{
		return m_isAttacked;
	}

protected:
	int m_healthPoints;
	int m_maxHealthPoints;
	bool m_isAttacked;
};

#endif