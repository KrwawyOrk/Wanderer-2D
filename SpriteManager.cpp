#include "SpriteManager.h"
#include "Game.h"
#include "Sprite.h"

#include "SDL_image.h"
#include <iostream>
#include <fstream>
#include <vector>

bool SpriteManager::GetSprite( Sprite& sprite, std::string spriteName )
{
	if( !SpritesLoadedSuccessfully() )
		return false;

	std::map<std::string, SDL_Surface*>::iterator it = m_SDLSurfaceMap.find( spriteName );

	if( it != m_SDLSurfaceMap.end() )
	{
		sprite.m_surface = it->second;
		sprite.m_name = spriteName;
		return true;
	}

	else
	{
		std::cout << "***** Nie istnieje plik graficzny o nazwie: " << spriteName << std::endl;
		return false;
	}

	return false;
}

SDL_Surface* SpriteManager::LoadSurface( std::string path )
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	loadedImage = IMG_Load( path.c_str() );
	optimizedImage = SDL_DisplayFormat( loadedImage );

	return optimizedImage;
}

void SpriteManager::LoadSprites( void )
{
	std::ifstream sprites;
	sprites.open( "pliki/grafika/grafika.txt" );

	if( sprites )
	{
		std::vector<SpriteInfo> spriteInfoVector;

		while( !sprites.eof() )
		{
			SpriteInfo info;

			sprites >> info.m_name;
			sprites >> info.m_path;

			spriteInfoVector.push_back( info );
		}

		sprites.close();

		for( std::vector<SpriteInfo>::const_iterator cit = spriteInfoVector.begin() ; cit != spriteInfoVector.end() ; ++cit )
		{
			if( ( m_SDLSurfaceMap[cit->m_name] = IMG_Load( cit->m_path.c_str() ) ) != NULL )
			{
				m_spritesLoadedSuccessfully = true;
				std::cout << "Wczytalem:\t" << cit->m_path << std::endl;
			}

			else
			{
				m_spritesLoadedSuccessfully = false;
				std::cout << "***** Blad wczytania:\t" << cit->m_path << std::endl;

				break;
			}
		}
	}

	else
	{
		m_spritesLoadedSuccessfully = false;
		std::cout << "Blad otwarcia pliku:\tpliki/grafika/grafika.txt" << std::endl;
	}


	//TEST
		/*for( std::map<std::string, SDL_Surface*>::const_iterator cit = m_SDLSurfaceMap.begin() ; cit != m_SDLSurfaceMap.end() ; ++cit )
		{
			std::cout << cit->first << std::endl;
		}
*/


		//END TEST
}
