#ifndef PLAYER_H
#define PLAYER_H

#include "Monster.h"

enum weaponType_t
{
	CROWBAR = 1,
	PISTOL
};

const int CROWBAR_DAMAGE = 2;
const int PISTOL_DAMAGE = 5;
const int PISTOL_ATTACK_RANGE = 4;
const int ATTACK_DELAY = 1;
const int MAX_ATTACK_DISTANCE = PISTOL_ATTACK_RANGE;
const int SLEEPING_TIME = 2;
const float BASE_VELOCITY = 160.0f;
const int INVENTORY_LIMIT = 20;

class Item;
class Monster;

class Player : public Creature
{
public:
	Player();
	virtual ~Player() {}

	virtual void Think( void );
	virtual void Draw( void );

	int GetActionPoints( void ) const { return m_actionPoints; }
	void SetActionPoints( int actionPoints ) { m_actionPoints = actionPoints; }

	int GetMaxActionPoints( void ) const { return m_maxActionPoints; }
	void SetMaxActionPoints( int maxActionPoints ) { m_maxActionPoints = maxActionPoints; }

	int GetFood( void ) const { return m_food; }
	void SetFood( int food ) { m_food = food; }

	int GetMaxFood( void ) const { return m_maxFood; }
	void SetMaxFood( int food ) { m_maxFood = food; }

	int GetToolBox( void ) const { return m_toolbox; }
	void SetToolBox( int box ) { m_toolbox = box; }

	int GetJunk( void ) const { return m_junk; }
	void SetJunk( int junk ) { m_junk = junk; }

	int GetExperiencePoints( void ) const { return m_experiencePoints; }
	void SetExperiencePoints( int experiencePoints ) { m_experiencePoints = experiencePoints; }

	void SetMorphicHammer( bool b ) { m_morphicHammer = b; }
	bool GetMorphicHammer( void ) { return m_morphicHammer; }

	void SetMiningAbility( bool b ) { m_miningAbility = b ; }
	bool GetMiningAbility( void ) { return m_miningAbility; }

	void SetMiningMode( bool b ) { m_miningMode = b; }
	bool GetMiningMode( void ) { return m_miningMode; }

	void SetMiningHammerDurability( int durability ) { m_miningHammerDurability = durability; }
	int GetMiningHammerDurability( void ) const { return m_miningHammerDurability; }

	int GetBaseDamage( void ) { return m_damage; }
	void SetBaseDamage( int damage ) { m_damage = damage; }

	int GetWeaponDamage( void ) const;

	void AttackMonster( Monster* monster );

	void SetAttackedMonster( Monster* monster );
	void AttackingMonster( void );
	void CheckMonsterAttackDistance( void );

	virtual void Move( direction_t direction );
	void PickBox( void );
	void RestoreActionPoints( void ) { m_actionPoints = m_maxActionPoints; }
	void GiveFood( int food );
	void Selected( bool s ) { m_selected = s; }
	bool IsSelected( void ) { return m_selected; }

	void SetWeapon( weaponType_t weaponType ) { m_weaponType = weaponType; }
	weaponType_t GetWeaponType( void ) const { return m_weaponType; }

	void SetPistolAmmunity( int ammunition ) { m_pistolAmmunition = ammunition; }
	int GetPistolAmmunition( void ) const { return m_pistolAmmunition; }

	void GoSleep( void ); //odpoczynek przywraca nam pkt wytrzymalosci
	bool IsSleeping( void ) const { return m_sleeping; }

	std::vector<Item*>& GetItems( void ) { return m_itemsVector; }
	void GiveItem( Item* item );
	bool HasSpaceInInventory( void ) const;

private:
	bool m_selected;
	Sprite m_spriteSelected;

	int m_actionPoints;
	int m_maxActionPoints;
	int m_damage;
	int m_food;
	int m_maxFood;
	int m_toolbox;
	int m_junk;
	int m_experiencePoints;

	bool m_morphicHammer;
	int m_miningHammerDurability;
	bool m_miningAbility;
	bool m_miningMode;

	Monster* m_attackedMonster;
	int m_nextAttack;
	int m_attackSpeed; //tylko liczby int

	weaponType_t m_weaponType;
	int m_pistolAmmunition;

	bool m_sleeping;
	int m_sleepingTime;

	Sprite m_sleepingPlayerSprite;
	std::vector<Item*> m_itemsVector;
};

#endif
