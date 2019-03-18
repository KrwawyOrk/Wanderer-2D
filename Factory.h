#ifndef FACTORY_H
#define FACTORY_H

#include <string>

#include "Item.h"

class Box;
class FoodGenerator;
class Monster;
class Position;
class StaticMapItem;
class Tile;

class Factory
{
public:
	Monster*			CreateMonster( std::string name, Position position, int healthPoints, int maxHealthPoints, int baseDamage, std::string spriteName );
	Monster*			CreateMonsterRespawn( std::string name, Position position, int respawnTime );
	Tile*				CreateTile( Position position, std::string spriteName );
	StaticMapItem*		CreateStaticMapItem( Position position, std::string name, bool walkable, std::string spriteName );
	Box*				CreateBox( Position position );
	Item*			CreateItem( Position position, itemType_t itemType );
	FoodGenerator*		CreateFoodGenerator( Position position );
};

#endif
