#include "Container.h"
#include "Factory.h"
#include "Game.h"
#include "Globals.h"
#include "Item.h"
#include "GSPlaying.h"
#include "StaticMapItem.h"
#include "Map.h"
#include "MapEntitiesLoader.h"

void MapEntitiesLoader::LoadLaboratoryEntities( Map* map )
{
	StaticMapItem* item = Globals::factory->CreateStaticMapItem( Position( 11, 4 ), "unknown", false, "szafkatest" );
	Container* container = new Container;
	container->AddItem( Globals::factory->CreateItem( Position( 0, 0 ), itemType_t::HEALTH_REGENERATION ));
	item->SetContainer( container );
	map->GetStaticMapItemVector().push_back( item );

	StaticMapItem* item2 = Globals::factory->CreateStaticMapItem( Position( 8, 10 ), "unknown", false, "szafkatest" );
	Container* container2 = new Container;
	item2->SetContainer( container2 );
	map->GetStaticMapItemVector().push_back( item2 );
}
