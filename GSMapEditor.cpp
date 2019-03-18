/*Witaj. Pewnie z ciekawosci przegladasz kod zrodlowy tej gry, ktora zostala napisana przez amatora ha ha. 
Wiem, burdel tu jak cholera ale wszystko dziala (prawie...) tak jak sobie wymarzylem. Jest tutaj kilka rzeczy, ktore w ogole
nie zostaly wykorzystane w programie. No coz pozwole zarzucic cytatem, ktory znalazlem w sieci.*/

//When I wrote this, only God and I understood what I was doing
//Now, God only knows 



#include "GSMapEditor.h"

#include "Camera.h"
#include "Factory.h"
#include "Game.h"
#include "Globals.h"
#include "GSPlaying.h"
#include "Map.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "StaticMapItem.h"
#include "Tile.h"
#include "Text.h"

#include <fstream>
#include <sstream>

GSMapEditor::GSMapEditor()
{
	for( int i = 0 ; i < 323 ; i++ )
	{
		m_keysHeld[i] = false;
	}

	m_mouseHeld = false;
	m_mouseHeldRight = false;

	m_brushSize = 1;
	m_brushPosition.x = 0;
	m_brushPosition.y = 0;

	SDL_GetMouseState( &m_brushPosition.x, &m_brushPosition.y );
	Globals::spriteManager->GetSprite( m_selectedSprite, m_selectedSpriteName );

	m_textInput = new GUITextInput( Position( 5, 5) );
	m_mapNameInput = new GUITextInput( Position( 100, 100 ) );

	m_mapEditorState = MENU;
	m_mode = TILE;
	m_walkableStaticMapItem = false;
	m_showModeInfo = false;
	m_mousePositionInfo = new BitmapFont( FontStyle::FONT_WHITE_SMALL );
	m_spriteName = new BitmapFont( FontStyle::FONT_WHITE_SMALL );
	m_mapEditorNotifications = false;

	m_selectedSpriteName = "trawa";
	Globals::spriteManager->GetSprite( m_selectedSprite, "trawa" );

	m_map = NULL;

	Globals::spriteManager->GetSprite( m_mapEditorGraphics[TILE_YES], "tile_yes" );
	Globals::spriteManager->GetSprite( m_mapEditorGraphics[TILE_NO], "tile_no" );
	Globals::spriteManager->GetSprite( m_mapEditorGraphics[STATICMAPITEM_YES], "staticmapitem_yes" );
	Globals::spriteManager->GetSprite( m_mapEditorGraphics[STATICMAPITEM_NO], "staticmapitem_no" );
	Globals::spriteManager->GetSprite( m_mapEditorGraphics[WALKABLE_YES], "walkable_yes" );
	Globals::spriteManager->GetSprite( m_mapEditorGraphics[WALKABLE_NO], "walkable_no" );
	Globals::spriteManager->GetSprite( m_brushSprite, "brush" );
	Globals::spriteManager->GetSprite( m_backgroundPaper, "paperbackground" );

	std::vector<std::string> mapeditorSprites;
	mapeditorSprites.push_back( "trawa" );
	mapeditorSprites.push_back( "trawa2" );
	mapeditorSprites.push_back( "floor1" );
	mapeditorSprites.push_back( "gravel" );
	mapeditorSprites.push_back( "ziemiatest" );
	mapeditorSprites.push_back( "podloga_kratka" );
	mapeditorSprites.push_back( "chair01" );
	mapeditorSprites.push_back( "chair02" );
	mapeditorSprites.push_back( "chair03" );
	mapeditorSprites.push_back( "chair04" );
	mapeditorSprites.push_back( "chair05" );
	mapeditorSprites.push_back( "chair06" );
	mapeditorSprites.push_back( "pctable01" );
	mapeditorSprites.push_back( "pctable02" );
	mapeditorSprites.push_back( "pctable03" );
	mapeditorSprites.push_back( "pctable04" );
	mapeditorSprites.push_back( "labtestwall" );
	mapeditorSprites.push_back( "labtestwalldirt" );
	mapeditorSprites.push_back( "labwallbase" );
	mapeditorSprites.push_back( "szafkatest" );

	int draw_x = 10;
	int draw_y = 30;
	int spritesInRow = 14;

	for( int i = 0 ; i < mapeditorSprites.size() ; i++ )
	{
		if( i % spritesInRow )
		{
			draw_x += 70;
		}

		else
		{
			if( i >= spritesInRow ) //Zeby od poczatku nie obnizylo spritow za nisko.
			{
				draw_x = 10;
				draw_y += 120;
			}
		}

		m_buttonsVector.push_back( new Button( draw_x, draw_y, mapeditorSprites.at(i) ) );
	}
}

GSMapEditor::~GSMapEditor()
{
	delete m_textInput;
	m_textInput = NULL;

	delete m_mapNameInput;
	m_mapNameInput = NULL;

	delete m_mousePositionInfo;
	m_mousePositionInfo = NULL;
}

void GSMapEditor::InputEvents( void )
{
	//---------------------------------------------------- MAPPING ----------------------------------------------------
	//---------------------------------------------------- MAPPING ----------------------------------------------------
	//---------------------------------------------------- MAPPING ----------------------------------------------------

	if( m_mapEditorState == MAPPING )
	{
		if( Globals::event.type == SDL_MOUSEBUTTONDOWN && Globals::event.button.button == SDL_BUTTON_LEFT )
		{
			m_mouseHeld = true;
		}

		else if( Globals::event.type == SDL_MOUSEBUTTONUP && Globals::event.button.button == SDL_BUTTON_LEFT )
		{
			m_mouseHeld = false;
		}

		if( Globals::event.type == SDL_MOUSEBUTTONDOWN && Globals::event.button.button == SDL_BUTTON_RIGHT )
		{
			m_mouseHeldRight = true;
			if( m_mapEditorNotifications )
			{
				std::cout << "m_mouseHeldRight = true" << std::endl;
			}
		}

		else if( Globals::event.type == SDL_MOUSEBUTTONUP && Globals::event.button.button == SDL_BUTTON_RIGHT )
		{
			m_mouseHeldRight = false;
			if( m_mapEditorNotifications )
			{
				std::cout << "m_mouseHeldRight = false" << std::endl;
			}
		}

		m_textInput->InputEvents();

		if( m_textInput->InputEnabled() )
		{
			if( Globals::event.type == SDL_KEYDOWN )
			{
				switch( Globals::event.key.keysym.sym )
				{
				case SDLK_RETURN:
					if( Globals::spriteManager->GetSprite( m_selectedSprite, m_textInput->GetText() ) )
					{
						m_selectedSpriteName = m_textInput->GetText();
					}

					m_textInput->Clear();
					m_textInput->DisableInput();

					break;
				}
			}
		}


		else
		{
			if( Globals::event.type == SDL_MOUSEMOTION )
			{
				Position position;
				position.x = ( Globals::event.button.x	+ Globals::camera->GetCameraX() ) / Globals::tilesize;
				position.y = ( Globals::event.button.y	+ Globals::camera->GetCameraY() ) / Globals::tilesize;

				int mouse_x = Globals::event.motion.x;
				int mouse_y = Globals::event.motion.y;
				if( mouse_y < 20 )
				{
					Globals::camera->velocity_y = -Globals::camera->cameraVelocity;
				}

				else if( mouse_y > 748 )
				{
					Globals::camera->velocity_y = Globals::camera->cameraVelocity;
				}

				else if( mouse_x < 20 )
				{
					Globals::camera->velocity_x = -Globals::camera->cameraVelocity;
				}

				else if( mouse_x > 1004 )
				{
					Globals::camera->velocity_x = Globals::camera->cameraVelocity;
				}

				else
				{
					Globals::camera->velocity_x = 0;
					Globals::camera->velocity_y = 0;
					
				}

				SetBrushPosition( position );
			}

			if( m_mouseHeld && !m_selectedSpriteName.empty() && m_brushPosition.x >= 0 && m_brushPosition.y >= 0 )
			{
				switch( m_mode )
				{
				case TILE:
					AddTile( m_map, m_brushPosition, m_selectedSpriteName );
					break;

				case STATIC_ITEM:
					AddStaticMapItem( m_map, m_brushPosition, m_walkableStaticMapItem, m_selectedSpriteName );
					break;
				}
			}

			/*if( m_mouseHeldRight && !m_selectedSpriteName.empty() && m_brushPosition.x >= 0 && m_brushPosition.y >= 0 )
			{
				switch( m_mode )
				{
				case TILE:
					RemoveTile( m_map, m_brushPosition );
					break;

				case STATIC_ITEM:
					RemoveTile( m_map, m_brushPosition );
					break;
				}
			}*/

			if( Globals::event.type == SDL_MOUSEBUTTONDOWN )
			{
				if( Globals::event.button.button == SDL_BUTTON_LEFT )
				{
					if( !m_selectedSpriteName.empty() )
					{
						if( m_brushPosition.x >= 0 && m_brushPosition.y >= 0 ) //Na naszej mapie nic nie może mieć minusowej pozycji. Zaczynamy od x = 0, y = 0!!!
						{
							switch( m_mode )
							{
							case TILE:
								{
									//Fajna funkcja. Im wiekszy brush size tym wiecej mozemy umiescic tilow.
									for( int x = 0 ; x < m_brushSize ; x++ )
									{
										for( int y = 0 ; y < m_brushSize ; y++ )
										{
											AddTile( m_map, Position( m_brushPosition.x + x, m_brushPosition.y + y ), m_selectedSpriteName );
										}
									}
								}
								break;

							case STATIC_ITEM:
								AddStaticMapItem( m_map, m_brushPosition, m_walkableStaticMapItem, m_selectedSpriteName );
								break;
							}
						}
					}

				}

				else if( Globals::event.button.button == SDL_BUTTON_RIGHT )
				{
					switch( m_mode )
					{
					case TILE:
						RemoveTile( m_map, m_brushPosition );
						break;

					case STATIC_ITEM:
						RemoveStaticMapItem( m_map, m_brushPosition );
						break;
					}
				}
			}

		}
		if( Globals::event.type == SDL_KEYDOWN )
		{
			m_keysHeld[Globals::event.key.keysym.sym] = true;

			switch( Globals::event.key.keysym.sym )
			{
			case SDLK_UP:
				Globals::camera->Move( ( cameraDirection_t )UP );
				break;

			case SDLK_DOWN:
				Globals::camera->Move( ( cameraDirection_t )DOWN );
				break;

			case SDLK_LEFT:
				Globals::camera->Move( ( cameraDirection_t )LEFT );
				break;

			case SDLK_RIGHT:
				Globals::camera->Move( ( cameraDirection_t )RIGHT );
				break;

			case SDLK_KP1:
				Globals::camera->Move( ( cameraDirection_t )LEFT );
				Globals::camera->Move( ( cameraDirection_t )DOWN );
				break;

			case SDLK_KP3:
				Globals::camera->Move( ( cameraDirection_t )DOWN );
				Globals::camera->Move( ( cameraDirection_t )RIGHT );
				break;

			case SDLK_KP7:
				Globals::camera->Move( ( cameraDirection_t )UP );
				Globals::camera->Move( ( cameraDirection_t )LEFT );
				break;

			case SDLK_KP9:
				Globals::camera->Move( ( cameraDirection_t )UP );
				Globals::camera->Move( ( cameraDirection_t )RIGHT );
				break;

			case SDLK_ESCAPE:
				{
					Globals::game->SetGameState( "Play" );
					Globals::camera->Center( Globals::currentPlayer );

					m_textInput->Clear();
					m_selectedSprite.SetSDLSurface( NULL );

					break;
				}

			case SDLK_t:
				if( !m_textInput->InputEnabled() )
					m_mode = TILE;
				break;

			case SDLK_s:
				if( !m_textInput->InputEnabled() )
					m_mode = STATIC_ITEM;
				break;

			case SDLK_m:
				if( !m_textInput->InputEnabled() )
					m_mode = MONSTER;
				break;

			case SDLK_w:
				if( m_mode == STATIC_ITEM && !m_textInput->InputEnabled() )
					m_walkableStaticMapItem = !m_walkableStaticMapItem;

				break;

			case SDLK_p:
				if( !m_textInput->InputEnabled() )
				{
					m_mode = PROTECTION_ZONE;
				}

				break;

			case SDLK_SPACE:
				{
					if( !m_selectedSpriteName.empty() && m_brushPosition.x >= 0 && m_brushPosition.y >= 0 )
					{
						switch( m_mode )
						{
						case TILE:
							AddTile( m_map, m_brushPosition, m_selectedSpriteName );
							break;

						case STATIC_ITEM:
							AddStaticMapItem( m_map, m_brushPosition, m_walkableStaticMapItem, m_selectedSpriteName );
							break;
						}
					}

					break;
				}

			case SDLK_F6:
				SaveMap( m_map );
				break;

			case SDLK_x:
				{
					switch( m_mode )
					{
					case TILE:
						RemoveTile( m_map, m_brushPosition );
						break;

					case STATIC_ITEM:
						RemoveStaticMapItem( m_map, m_brushPosition );
						break;
					}
				}

				break;

			case SDLK_F1:
				std::cout << "Pozycja pedzla, x: " << m_brushPosition.x << " y: " << m_brushPosition.y << std::endl;
				std::cout << "Pozycja kamery, x: " << Globals::camera->GetCameraX() << " y: " << Globals::camera->GetCameraY() << std::endl;
				break;

			case SDLK_DELETE:
				delete m_map;
				m_map = NULL;

				m_mapEditorState = MENU;

				break;

			case SDLK_i:
				if( m_textInput->InputEnabled() == false )
				{
					m_mapEditorState = SPRITES;
				}
				break;

			case SDLK_KP_PLUS:
				{
					m_brushSize++;
					if( m_brushSize > 3 )
					{
						m_brushSize = 3;
					}
				}
				break;

			case SDLK_KP_MINUS:
				{
					m_brushSize--;
					if( m_brushSize < 1 )
					{
						m_brushSize = 1;
					}
				}
				break;
			}
		}

		if( Globals::event.type == SDL_KEYUP )
		{
			m_keysHeld[Globals::event.key.keysym.sym] = false;

			switch( Globals::event.key.keysym.sym )
			{
			case SDLK_RCTRL:
				Globals::camera->Center( GetBrushPosition() );
			}
		}
	}

	//---------------------------------------------------- MENU ----------------------------------------------------
	//---------------------------------------------------- MENU ----------------------------------------------------
	//---------------------------------------------------- MENU ----------------------------------------------------

	else if( m_mapEditorState == MENU )
	{
		m_mapNameInput->InputEvents();

		if( m_mapNameInput->InputEnabled() )
		{
			if( Globals::event.type == SDL_KEYDOWN )
			{
				switch( Globals::event.key.keysym.sym )
				{
				case SDLK_RETURN:
					{
						GSPlaying* gsPlaying = dynamic_cast<GSPlaying*>( Globals::game->GetGameState( "Play" ) );
						if( gsPlaying )
						{
							if( gsPlaying->MapExists( m_mapNameInput->GetText() ) )
							{
								std::cout << "Istnieje juz mapa: " << m_mapNameInput->GetText() << std::endl;
								m_mapNameInput->Clear();
							}

							else
							{
								std::cout << "Tworze mape o nazwie: " << m_mapNameInput->GetText() << std::endl;
								m_map = new Map( m_mapNameInput->GetText() );	
								m_mapEditorState = MAPPING;
							}
						}
					}

					break;
				}
			}
		}

		else
		{
			if( Globals::event.type == SDL_KEYDOWN && Globals::event.key.keysym.sym == SDLK_ESCAPE )
			{	
				Globals::game->SetGameState( "Play" );
				Globals::camera->FollowPlayer( true );
			}
		}
		
	}

	//---------------------------------------------------- SPRITES ----------------------------------------------------
	//---------------------------------------------------- SPRITES ----------------------------------------------------
	//---------------------------------------------------- SPRITES ----------------------------------------------------

	else if( m_mapEditorState == SPRITES )
	{
		if( Globals::event.type == SDL_KEYDOWN )
		{
			switch( Globals::event.key.keysym.sym )
			{
			case SDLK_ESCAPE:
				m_mapEditorState = MAPPING;
				break;

			case SDLK_i:
				m_mapEditorState = MAPPING;
				break;
			}
		}
	}
}

void GSMapEditor::Think( void )
{
	//---------------------------------------------------- MAPPING ----------------------------------------------------
	//---------------------------------------------------- MAPPING ----------------------------------------------------
	//---------------------------------------------------- MAPPING ----------------------------------------------------
	if( m_mapEditorState == MAPPING )
	{
		Globals::camera->Update( Globals::deltaTime );
		
		Globals::camera->Think();
		m_map->Think();

		if( m_keysHeld[SDLK_TAB] )
			m_showModeInfo = true;

		if( !m_keysHeld[SDLK_TAB] )
			m_showModeInfo = false;

		if( m_showModeInfo )
			std::cout << "m_showModeInfo = true" << std::endl;

		if( m_mouseHeld )
		{
			if( m_mapEditorNotifications )
			{
				std::cout << "Mouse HELD" << std::endl;
			}
		}
	}

	//---------------------------------------------------- SPRITES ----------------------------------------------------
	//---------------------------------------------------- SPRITES ----------------------------------------------------
	//---------------------------------------------------- SPRITES ----------------------------------------------------

	else if( m_mapEditorState == SPRITES )
	{
		for( std::vector<Button*>::iterator it = m_buttonsVector.begin() ; it != m_buttonsVector.end() ; ++it )
		{
			Button* button = (*it);
			if( button->ButtonClicked() )
			{
				m_selectedSpriteName = button->GetSprite().GetSpriteName();
				Globals::spriteManager->GetSprite( m_selectedSprite, m_selectedSpriteName );
				m_mapEditorState = MAPPING;
			}
		}
	}
}

void GSMapEditor::Draw( void )
{
	//---------------------------------------------------- MAPPING ----------------------------------------------------
	//---------------------------------------------------- MAPPING ----------------------------------------------------
	//---------------------------------------------------- MAPPING ----------------------------------------------------

	if( m_mapEditorState == MAPPING )
	{
		m_map->Draw();
		m_selectedSprite.Draw( Globals::screen, m_brushPosition );
		m_brushSprite.Draw( Globals::screen, m_brushPosition );
		m_textInput->Draw();
		m_selectedSprite.Draw( Globals::screen, 5, 50 );
		DrawMousePositionInfo();
		m_spriteName->show_text( 700, 10, m_selectedSprite.GetSpriteName(), Globals::screen );
		
		if( m_mode == TILE )
			m_mapEditorGraphics[TILE_YES].Draw( Globals::screen, 250, 20 );

		else
			m_mapEditorGraphics[TILE_NO].Draw( Globals::screen, 250, 20 );

		if( m_mode == STATIC_ITEM )
			m_mapEditorGraphics[STATICMAPITEM_YES].Draw( Globals::screen, 360, 20 );

		else
			m_mapEditorGraphics[STATICMAPITEM_NO].Draw( Globals::screen, 360, 20 );

		if( m_walkableStaticMapItem )
			m_mapEditorGraphics[WALKABLE_YES].Draw( Globals::screen, 470, 20 );

		else
			m_mapEditorGraphics[WALKABLE_NO].Draw( Globals::screen, 470, 20 );
	}


	//---------------------------------------------------- MENU ----------------------------------------------------
	//---------------------------------------------------- MENU ----------------------------------------------------
	//---------------------------------------------------- MENU ----------------------------------------------------


	else if( m_mapEditorState == MENU )
	{
		m_backgroundPaper.Draw( Globals::screen, 25, 25 );
		m_mapNameInput->Draw();
	}

	//---------------------------------------------------- SPRITES ----------------------------------------------------
	//---------------------------------------------------- SPRITES ----------------------------------------------------
	//---------------------------------------------------- SPRITES ----------------------------------------------------

	else if( m_mapEditorState == SPRITES )
	{
		for( std::vector<Button*>::iterator it = m_buttonsVector.begin() ; it != m_buttonsVector.end() ; ++it )
		{
			Button* button = (*it);
			button->DrawButton();

			if( button->ButtonHover() )
			{
				m_spriteName->show_text( button->position_x, button->position_y - 20, button->GetSprite().GetSpriteName(), Globals::screen );
			}
		}
	}
}

void GSMapEditor::MoveBrush( brushDirection_t brushDirection )
{
	if( m_mapEditorState == MAPPING )
	{
		switch( brushDirection )
		{
		case UP:
			m_brushPosition.y--;
			break;

		case DOWN:
			m_brushPosition.y++;
			break;

		case LEFT:
			m_brushPosition.x--;
			break;

		case RIGHT:
			m_brushPosition.x++;
			break;
		}

		Globals::camera->Center( m_brushPosition );
	}
}

void GSMapEditor::SaveMap( Map* map )
{
	std::vector<Tile*>& mapTiles = m_map->GetTilesVector();
	std::vector<Tile*>::const_iterator cit;

	std::string path = "mapeditor/" + m_map->GetMapName() + "_tiles.txt";
	std::ofstream tiles( "mapeditor/tiles.txt" );

	if( tiles )
	{
		for( cit = mapTiles.begin() ; cit != mapTiles.end() ; ++cit )
		{
			tiles << ( *cit )->GetPosition().x;
			tiles << " ";
			tiles << ( *cit )->GetPosition().y;
			tiles << " ";
			tiles << ( *cit )->GetSprite().GetSpriteName();
			tiles << "\n";
		}

		std::cout << "Zapisuje TILE do pliku mapeditor/map_tiles.txt, ilosc TILE: " << mapTiles.size() << std::endl;

		tiles.close();
	}

	std::vector<StaticMapItem*>& mapStaticItems = m_map->GetStaticMapItemVector();
	std::vector<StaticMapItem*>::const_iterator citt;

	std::ofstream staticItems( "mapeditor/staticmapitems.txt" );

	if( staticItems )
	{
		for( citt = mapStaticItems.begin() ; citt != mapStaticItems.end() ; ++citt )
		{
			staticItems << ( *citt )->GetPosition().x;
			staticItems << " ";
			staticItems << ( *citt )->GetPosition().y;
			staticItems << " ";
			staticItems << ( *citt )->GetName();
			staticItems << " ";
			staticItems << ( *citt )->IsWalkable();
			staticItems << " ";
			staticItems << ( *citt )->GetSprite().GetSpriteName();
			staticItems << "\n";
		}

		std::cout << "Zapisuje STATIC MAP ITEMS do pliku mapeditor/map_staticitems.txt.\n ilosc STATIC MAP ITEMS: " << mapStaticItems.size() << std::endl;

		staticItems.close();
	}
}

void GSMapEditor::AddTile( Map *map, Position& position, std::string spriteName )
{
	bool canAddTile = true;

	std::vector<Tile*>& tiles = map->GetTilesVector();
	std::vector<Tile*>::const_iterator cit;

	for( cit = tiles.begin() ; cit != tiles.end() ; ++cit )	// Sprawdzamy czy nie ma juz jakiegos TILE na tej pozycji.
	{
		if( ( *cit )->GetPosition().x == position.x && ( *cit )->GetPosition().y == position.y )
			canAddTile = false;
	}

	if( canAddTile )
		tiles.push_back( Globals::factory->CreateTile( position, spriteName ) );

	else
		if( m_mapEditorNotifications )
		{
			std::cout << "Nie mozna postawic wiecej niz jeden TILE na pozycji x: " << m_brushPosition.x << " y: " << m_brushPosition.y << std::endl;
		}
}

void GSMapEditor::AddStaticMapItem( Map* map, Position& position, bool walkable, std::string spriteName )
{
	bool canAddStaticMapItem = true;

	std::vector<StaticMapItem*>& staticMapItems = map->GetStaticMapItemVector();
	std::vector<StaticMapItem*>::const_iterator cit;

	for( cit = staticMapItems.begin() ; cit != staticMapItems.end() ; ++cit )
	{
		if( ( *cit )->GetPosition().x == position.x && ( *cit )->GetPosition().y == position.y )
			canAddStaticMapItem = false;
	}

	if( canAddStaticMapItem )
	{
		staticMapItems.push_back( Globals::factory->CreateStaticMapItem( position, "unknown", walkable, spriteName ) );
	}

	else
	{
		if( m_mapEditorNotifications )
		{
			std::cout << "Nie mozna postawic wiecej niz jeden STATIC MAP ITEM na pozycji x: " << m_brushPosition.x << " y: " << m_brushPosition.y << std::endl;
		}
	}
}

void GSMapEditor::RemoveTile( Map* map, Position& position )
{
	std::vector<Tile*>& tiles = map->GetTilesVector();
	std::vector<Tile*>::iterator cit;

	for( cit = tiles.begin() ; cit != tiles.end() ; )
	{
		if( ( *cit )->GetPosition().x == position.x && ( *cit )->GetPosition().y == position.y )
		{
			delete ( *cit );
			cit = tiles.erase( cit );
		}

		else
			++cit;
	}
}

void GSMapEditor::RemoveStaticMapItem( Map* m_map, Position& position )
{
	std::vector<StaticMapItem*>& staticMapItems = m_map->GetStaticMapItemVector();
	std::vector<StaticMapItem*>::iterator cit;

	for( cit = staticMapItems.begin() ; cit != staticMapItems.end() ; )
	{
		if( ( *cit )->GetPosition().x == position.x && ( *cit )->GetPosition().y == position.y )
		{
			delete ( *cit );
			cit = staticMapItems.erase( cit );
		}

		else
			++cit;
	}
}

void GSMapEditor::DrawMousePositionInfo( void )
{
	int mouse_x = ( Globals::event.motion.x + Globals::camera->GetCameraX() ) / Globals::tilesize;
	int mouse_y = ( Globals::event.motion.y + Globals::camera->GetCameraY() ) / Globals::tilesize;

	std::stringstream ss;
	ss << "x: " << mouse_x << "  y: " << mouse_y;

	m_mousePositionInfo->show_text( 900, 10, ss.str(), Globals::screen );
}