#ifndef PLAYER_H
#define PLAYER_H

#include "Monster.h"
#include "Skill.h"

#include "SDL.h"
#include <vector>

const int CROWBAR_DAMAGE = 2;
const int PISTOL_DAMAGE = 5;

const int MALEE_ATTACK_RANGE = 1;
const int PISTOL_ATTACK_RANGE = 4;

const float ATTACK_DELAY_MALEE = 0.3f;
const float ATTACK_DELAY_GUN = 1.5f;

const int MAX_ATTACK_DISTANCE = PISTOL_ATTACK_RANGE;
const float BASE_VELOCITY = 100.0f;
const int INVENTORY_LIMIT = 40;

class Item;
class Monster;

enum weaponType_t
{
	CROWBAR = 1,
	PISTOL
};

class Skills
{
public:
	int m_battle;
	int m_medicine;
	bool m_lockpicking;
	bool m_looting;
	bool m_cooking;
};


//animation
const int FOO_WIDTH = 100;
const int FOO_HEIGHT = 100;

const int FOO_RIGHT = 0;
const int FOO_LEFT = 1;
const int FOO_UP = 2;
const int FOO_DOWN = 3;
//end animation

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

	int GetMonstersKilled( void ) const { return m_monstersKilled; }

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

	Skills& GetSkills( void ) { return m_skills; }

	int GetWeaponDamage( void );

	void StopAttackingMonster( void )
	{
		m_attackedMonster = NULL;
	}

	void SetAttackedMonster( Monster* monster );
	void CheckMonsterAttackDistance( void );

	Monster* GetAttackedMonster( void )
	{
		return m_attackedMonster;
	}

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

	std::vector<Item*>& GetItems( void ) { return m_itemsVector; }
	void GiveItem( Item* item );
	bool HasSpaceInInventory( void ) const;

	Skill& GetSkillOfType( skillTypes::skillType_t skillType );
	void SetTimeToNextAttack( float time );

	//animation
	void set_clips();
	//endanimation

	bool IsCooldownReadyToAttack( void );
	bool IsEnemyInAttackDistance(int distance, Monster* monster );
	void AttackMonster( Monster* monster );
	void AttackMonsterWithMaleeWeapon( Monster* monster );
	void AttackMonsterWithDistanceWeapon( Monster* monster );
	bool CanFireAtTheMonster( Monster* monster );
	void RemovePistolAmmunition( void );
	void GiveAmmunition( int count );

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
	int m_monstersKilled;

	bool m_morphicHammer;
	int m_miningHammerDurability;
	bool m_miningAbility;
	bool m_miningMode;

	Monster* m_attackedMonster;
	float m_nextAttack;
	float m_attackSpeed; //tylko liczby int

	weaponType_t m_weaponType;
	int m_pistolAmmunition;

	std::vector<Item*> m_itemsVector;

	Skills m_skills;
	std::vector<Skill> m_skillsVector;

	//animation
	int frame;
	int status;
	int offSet;
	SDL_Rect clipsRight[3];
	SDL_Rect clipsLeft[3];
	SDL_Rect clipsUp[3];
	SDL_Rect clipsDown[3];

	Sprite m_animationSprite;
};

#endif
