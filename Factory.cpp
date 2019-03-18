#include "Factory.h"

#include "Box.h"
#include "FoodGenerator.h"
#include "GameObject.h"
#include "Globals.h"
#include "Monster.h"
#include "SpriteManager.h"
#include "StaticMapItem.h"
#include "Tile.h"

Monster* Factory::CreateMonster( std::string name, Position position, int healthPoints, int maxHealthPoints, int baseDamage, std::string spriteName )
{
	Monster* m = new Monster;

	m->SetName( name );
	m->SetPosition( position );
	m->SetFloatX( static_cast<float>( position.x * Globals::tilesize ) );
	m->SetFloatY( static_cast<float>( position.y * Globals::tilesize ) );
	m->SetVelocity( 50.0f );
	m->SetHealthPoints( healthPoints );
	m->SetMaxHealthPoints( maxHealthPoints );
	m->SetBaseDamage( baseDamage );
	m->SetNextMove( 0 );
	m->SetNextAttack( 0 );
	m->SetAttackingPlayer( false );
	m->SetAttackedByPlayer( false );

	Globals::spriteManager->GetSprite( m->GetSprite(), spriteName );

	return m;
}

Monster* Factory::CreateMonsterRespawn( std::string name, Position position, int respawnTime )
{
	Monster* m = new Monster;

	m->SetName( name );
	m->SetPosition( position );
	m->SetFloatX( static_cast<float>( position.x * Globals::tilesize ) );
	m->SetFloatY( static_cast<float>( position.y * Globals::tilesize ) );
	m->SetVelocity( 50.0f );
	m->SetHealthPoints( 20 );
	m->SetMaxHealthPoints( 20 );
	m->SetBaseDamage( 5 );
	m->SetNextMove( 0 );
	m->SetNextAttack( 0 );
	m->SetAttackingPlayer( false );
	m->SetAttackedByPlayer( false );
	m->SetRespawnTime( respawnTime );
	m->SetRespawnPosition( position );
	m->SetDeathTime( 0 );

	Globals::spriteManager->GetSprite( m->GetSprite(), "monsterinfo" );

	return m;
}

Tile* Factory::CreateTile( Position position, std::string spriteName )
{
	Tile* t = new Tile( position, spriteName );

	return t;
}

StaticMapItem* Factory::CreateStaticMapItem( Position position, std::string name, bool walkable, std::string spriteName )
{
	return new StaticMapItem( position, name, walkable, spriteName );
}

Box* Factory::CreateBox( Position position )
{
	Box* box = new Box;
	box->SetPosition( position );

	return box;
}

Item* Factory::CreateItem( Position position, itemType_t itemType )
{
	return new Item( position, itemType );
}

FoodGenerator* Factory::CreateFoodGenerator( Position position )
{
	return new FoodGenerator( position );
}


