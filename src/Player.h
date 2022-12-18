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

	void LoadPlayerDataFromXML( const std::string& fileName );

	virtual void Think( void );
	virtual void Draw( void );

	int GetExperiencePoints( void ) const { return m_experiencePoints; }
	void SetExperiencePoints( int experiencePoints ) { m_experiencePoints = experiencePoints; }

	int GetMonstersKilled( void ) const { return m_monstersKilled; }

	int GetBaseDamage( void ) { return m_damage; }
	void SetBaseDamage( int damage ) { m_damage = damage; }

	Skills& GetSkills( void ) { return m_skills; }

	int GetWeaponDamage( void );

	virtual void Move( direction_t direction );
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
	void LevelUpDamage( void );
	void CheckReductionInSpeedAtLowHealthPoints( void );

private:
	bool m_selected;
	Sprite m_spriteSelected;


	int m_damage;
	int m_experiencePoints;
	int m_monstersKilled;

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
