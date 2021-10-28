#include "LootGenerator.h"

#include "Container.h"
#include "Item.h"
#include "Tools.h"

void LootGenerator::FillContainerWithRandomLoot( Container* container )
{
	if( container )
	{
		for( int i = 0 ; i < 3; i++ )
		{
			if( Tools::GenerateRandomNumber(3) == 1 )
			{
				container->AddItem( new Item( Position( -1, -1 ), HEALTH_REGENERATION ) );
			}

		}

		if( Tools::GenerateRandomNumber(4) == 1 )
		{
			container->AddItem( new Item( Position( -1, -1 ), EXPERIENCE_BOOK ) );
		}

		if( Tools::GenerateRandomNumber(4) == 1 )
		{
			container->AddItem( new Item( Position( -1, -1 ), AMMO_PACK ) );
		}

		container->AddItem( new Item( Position( -1, -1 ), AMMO_PACK ) );
	}
}
