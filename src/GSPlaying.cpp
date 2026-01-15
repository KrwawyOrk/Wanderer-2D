#include "GSPlaying.h"

#include "Camera.h"
#include "Container.h"
#include "Factory.h"
#include "Game.h"
#include "Globals.h"
#include "GSMapEditor.h"
#include "ItemSlot.h"
#include "Map.h"
#include "MapEntitiesLoader.h"
#include "Particle.h"
#include "SpriteManager.h"
#include "StaticMapItem.h"
#include "Tile.h"
#include "Tools.h"
#include "Item.h"
#include "TextBox.h"

#include <iostream>
#include <sstream>
#include <math.h>

#include "Sprite.h"

GSPlaying::GSPlaying( void )
{
	for( int i = 0 ; i < 323 ; i++ )
	{
		m_keysHeld[i] = false;
	}

	m_mapList.push_back( new Map( "lasek" ) );
	m_mapList.push_back( new Map( "lasek2" ) );

	Map* map = GetMapByName( "lasek" );

	//MapEntitiesLoader mapEntitiesLoader;
	//mapEntitiesLoader.LoadLaboratoryEntities( map );

	ChangeMap( "lasek" );

	m_playingState = GAMEPLAY;

	m_player = new Player;
	Globals::player = m_player;
	m_playerBelt.SetPlayer( Globals::player );

	Globals::camera->Center( Globals::player );

	m_fishing = false;
	m_day = 1;

	m_exitLocationMessage = new BitmapFont( FontStyle::FONT_WHITE_SMALL_OLD );

	Globals::playerBelt = &m_playerBelt;

	m_openedContainer = NULL;
	m_npc = NULL;

	m_informationsConsole = new TextBox( 15, 650, 300, 300, "pliki/font.ttf", 16 );
	Globals::messageLog = m_informationsConsole;
	
	Globals::messageLog->addLine( "Hello message log! 1" );
	Globals::messageLog->addLine( "Hello message log! 2" );
	Globals::messageLog->addLine( "Hello message log! 3" );
	Globals::messageLog->addLine( "Hello message log! 4" );
	Globals::messageLog->addLine( "Hello message log! 5" );
	Globals::messageLog->addLine( "Hello message log! 6" );

	m_flashlightBattery = 1.0f;
	m_flashlightMask = nullptr;
	m_currentMaskRadius = -1;
}

GSPlaying::~GSPlaying( void )
{
	delete m_informationsConsole;
	m_informationsConsole = NULL;

	for (auto& p : m_particles) delete p;
	m_particles.clear();
}

void GSPlaying::InputEvents( void )
{
	m_informationsConsole->handleEvent( Globals::event );

	if( m_openedContainer )
	{
		m_openedContainer->InputEvents();
	}

	if( Globals::event.type == SDL_MOUSEBUTTONDOWN )
	{
		if( Globals::event.button.button == SDL_BUTTON_LEFT )
		{
			Position position; //pozycja wcisniecia przycisku myszki
			position.x = ( Globals::event.button.x + Globals::camera->GetCameraX() ) / Globals::tilesize;
			position.y = ( Globals::event.button.y + Globals::camera->GetCameraY() ) / Globals::tilesize;			

			std::vector<Item*> &items = Globals::currentMap->GetItemsVector();
			std::vector<Item*>::iterator it;
			for( it = items.begin() ; it != items.end() ; )
			{
				Item* item = ( *it );
				int distanceToPick = static_cast<int>(Tools::CalculateDistance( m_player->GetPosition().x, m_player->GetPosition().y, item->GetPosition().x, item->GetPosition().y ));

				if( m_player->HasSpaceInInventory() && item->GetPosition().x == position.x && item->GetPosition().y == position.y && distanceToPick <= 1 )
				{
					it = items.erase( it );
					m_player->GiveItem( item );
				}

				else
				{
					++it;
				}
			}


			std::cout << "Pozycja myszka-button-left: x: " << Globals::event.button.x << " y: " << Globals::event.button.y << std::endl;
			std::cout << "Pozycja po konwersji: x: " << position.x  << " y: " << position.y  << std::endl;
		}

		if( Globals::event.button.button == SDL_BUTTON_RIGHT )
		{
			Position position; //pozycja wcisniecia przycisku myszki
			position.x = ( Globals::event.button.x + Globals::camera->GetCameraX() ) / Globals::tilesize;
			position.y = ( Globals::event.button.y + Globals::camera->GetCameraY() ) / Globals::tilesize;

	
			std::vector<Monster*>&monsters = Globals::currentMap->GetMonstersVector();

			for( std::vector<Monster*>::const_iterator cit = monsters.begin() ; cit != monsters.end() ; ++cit )
			{
				Monster* monster = ( *cit );
				monster->SetAttackedByPlayer( false );

				if(monster->GetPosition().x == position.x && monster->GetPosition().y == position.y )
				{
					if(monster->IsAlive() )
					{
						m_player->AttackMonster( monster );

						if (m_openedContainer)
						{
							m_openedContainer->setWindowVisible( false );
						}
					}
				}
			}

			std::vector<StaticMapItem*> &staticMapItems = Globals::currentMap->GetStaticMapItemVector();

			for( std::vector<StaticMapItem*>::const_iterator cit = staticMapItems.begin() ; cit != staticMapItems.end() ; ++cit )
			{
				StaticMapItem* staticMapItem = ( *cit );
				if( staticMapItem->GetPosition().x == position.x && staticMapItem->GetPosition().y == position.y )
				{
					int distance = static_cast<int>( Tools::CalculateDistance( staticMapItem->GetPosition().x, staticMapItem->GetPosition().y, m_player->GetPosition().x, m_player->GetPosition().y ) );

					if( staticMapItem->HasContainer() && distance <= 1 )
					{
						if( staticMapItem->GetContainer()->IsLocked() )
						{
							std::cout << "Potrzebujesz wytrychu." << std::endl;
						}

						else
						{
							staticMapItem->GetContainer()->setWindowVisible( true );
							m_openedContainer = staticMapItem->GetContainer();
						}
					}

					std::cout << "Wcisnales PPM na StaticMapItem." << std::endl;
				}
			}
		}
	}


	if( Globals::event.type == SDL_MOUSEMOTION )
	{
		MoveCameraOnMouseMotion();
	}

	if( Globals::event.type == SDL_KEYDOWN )
	{
		m_keysHeld[Globals::event.key.keysym.sym] = true;

		switch (Globals::event.key.keysym.sym)
		{
		case SDLK_q:
		case SDLK_ESCAPE:
			Globals::game->Drop();
			break;

		case SDLK_KP4:
			Globals::camera->Move( LEFT );
			break;

		case SDLK_KP6:
			Globals::camera->Move( RIGHT );
			break;

		case SDLK_KP8:
			Globals::camera->Move( UP );
			break;

		case SDLK_KP2:
			Globals::camera->Move( DOWN );
			break;

		case SDLK_c:
			Globals::camera->SetPosition( 0, 0 );
			break;

		case SDLK_i:
			PrintInformationsInConsole();
			break;

		case SDLK_u:
			m_player->LevelUpDamage();
			break;

		case SDLK_y:
			Globals::showCursor = !Globals::showCursor;
			break;

		case SDLK_SPACE:
			Globals::camera->Center( m_player );
			break;

		case SDLK_p:
		{
			bool b = m_player->GetSkillOfType( skillTypes::INCREASED_VISIBILITY ).SkillIsLearned();
			m_player->GetSkillOfType( skillTypes::INCREASED_VISIBILITY ).SetLearnedSkill( !b );
		}
		break;

		case SDLK_1:
			m_player->SetWeapon( CROWBAR );
			break;

		case SDLK_2:
			m_player->SetWeapon( PISTOL );
			break;

		case SDLK_F1:
			Globals::camera->FollowPlayer( !Globals::camera->GetCameraFollowPlayer() );
			break;

		case SDLK_F9:
			OpenMapEditorToEditCurrentMap();
			break;

		case SDLK_F10:
			OpenMapEditorNewMapWindow();
			break;
			
		case SDLK_F11:
			Globals::game->fade_to_black( 10 );
			break;

		case SDLK_F12:
			Globals::game->ToggleFullScreen();
			break;

		case SDLK_RETURN:
			PlayerLeaveMap();
			break;

		case SDLK_h:
			//ShakeScreen( Globals::screen, 20, 10 );
			FadeToBlack( Globals::screen, 1000 );
			break;

		case SDLK_v:
			std::cout << GetCurrentMap()->GetMapName() << std::endl;
			break;

		case SDLK_j:
			m_informationsConsole->addLine( "Dodano nowy przedmiot!" );
			break;

		case SDLK_TAB:
			PlayerBelt* belt = Globals::playerBelt;

			if( belt->GetPlayerBeltState() == playerBeltState_t::INVENTORY_STATE ) 
			{
				belt->SetPlayerBeltState( playerBeltState_t::STATISTIC_STATE );
			}
			else {
				belt->SetPlayerBeltState( playerBeltState_t::INVENTORY_STATE );
			}

			break;
		}
	}

	else if( Globals::event.type == SDL_KEYUP )
	{
		m_keysHeld[Globals::event.key.keysym.sym] = false;
	}

	m_playerBelt.InputEvents();
	m_map->InputEvents();
}

void GSPlaying::Think( void )
{
	//Globals::camera->Think();
	m_map->Think();
	m_player->Think();
	m_playerBelt.Think();

	if (m_player->IsMoving())
	{
		m_npc = NULL;
	}

	CheckIfContainerIsOpened();
	ThinkMovementInputButtonsHeld();

	for (auto it = m_particles.begin(); it != m_particles.end(); )
	{
		(*it)->Update( Globals::deltaTime );   // <--- TU się dzieje magia

		if ((*it)->IsDead())
		{
			delete* it;
			it = m_particles.erase( it );
		}
		else
		{
			++it;
		}
	}
}

void GSPlaying::ThinkMovementInputButtonsHeld( void )
{
	if (m_keysHeld[SDLK_UP] || m_keysHeld[SDLK_w])
	{
		m_player->Move( NORTH );
	}

	else if (m_keysHeld[SDLK_DOWN] || m_keysHeld[SDLK_s])
	{
		m_player->Move( SOUTH );
	}

	else if (m_keysHeld[SDLK_RIGHT] || m_keysHeld[SDLK_d])
	{
		m_player->Move( EAST );
	}

	else if (m_keysHeld[SDLK_LEFT] || m_keysHeld[SDLK_a])
	{
		m_player->Move( WEST );
	}
}

void GSPlaying::Update( float deltaTime )
{
	m_map->Update( deltaTime );
	m_player->Update( deltaTime );
	Globals::camera->Update( deltaTime );

	UpdateFlashlightBattery( deltaTime ); // ← używamy zewnętrznego deltaTime
}

void GSPlaying::Draw()
{
	// 1. Buforujemy scenę
	SDL_Surface* sceneBuffer = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		Globals::screen->w, Globals::screen->h,
		Globals::screen->format->BitsPerPixel,
		Globals::screen->format->Rmask,
		Globals::screen->format->Gmask,
		Globals::screen->format->Bmask,
		0
	);

	if (!sceneBuffer) {
		DrawUI();
		return;
	}

	// Rysuj scenę do bufora
	SDL_Surface* temp = Globals::screen;
	Globals::screen = sceneBuffer;
	DrawScene();
	Globals::screen = temp;

	// 2. Czyszczenie ekranu
	SDL_FillRect( Globals::screen, nullptr, SDL_MapRGB( Globals::screen->format, 0, 0, 0 ) );

	// 3. Światło tylko jeśli bateria > 0
	if (m_flashlightBattery > 0.01f) {
		int R = GetFlashlightRadius();
		EnsureFlashlightMask( R );

		if (m_flashlightMask) {
			int mx, my;
			SDL_GetMouseState( &mx, &my );

			int srcX = 0, srcY = 0;
			int dstX = mx - R, dstY = my - R;
			int w = R * 2, h = R * 2;

			// Obsługa krawędzi
			if (dstX < 0) { srcX = -dstX; w -= srcX; dstX = 0; }
			if (dstY < 0) { srcY = -dstY; h -= srcY; dstY = 0; }
			if (dstX + w > Globals::screen->w) w = Globals::screen->w - dstX;
			if (dstY + h > Globals::screen->h) h = Globals::screen->h - dstY;

			if (w > 0 && h > 0) {
				SDL_LockSurface( Globals::screen );
				SDL_LockSurface( sceneBuffer );
				SDL_LockSurface( m_flashlightMask );

				Uint32* screen = (Uint32*)Globals::screen->pixels;
				Uint32* scene = (Uint32*)sceneBuffer->pixels;
				Uint32* mask = (Uint32*)m_flashlightMask->pixels;

				int sp = Globals::screen->pitch / sizeof( Uint32 );
				int bp = sceneBuffer->pitch / sizeof( Uint32 );
				int mp = m_flashlightMask->pitch / sizeof( Uint32 );

				for (int y = 0; y < h; ++y) {
					for (int x = 0; x < w; ++x) {
						Uint32 scenePixel = scene[(dstY + y) * bp + (dstX + x)];
						Uint32 maskPixel = mask[(srcY + y) * mp + (srcX + x)];

						Uint8 sr, sg, sb;
						SDL_GetRGB( scenePixel, sceneBuffer->format, &sr, &sg, &sb );

						Uint8 intensity;
						SDL_GetRGB( maskPixel, m_flashlightMask->format, &intensity, &intensity, &intensity );

						sr = (sr * intensity) / 255;
						sg = (sg * intensity) / 255;
						sb = (sb * intensity) / 255;

						screen[(dstY + y) * sp + (dstX + x)] = SDL_MapRGB( Globals::screen->format, sr, sg, sb );
					}
				}

				SDL_UnlockSurface( m_flashlightMask );
				SDL_UnlockSurface( sceneBuffer );
				SDL_UnlockSurface( Globals::screen );
			}
		}
	}

	SDL_FreeSurface( sceneBuffer );
	DrawUI();
}

void GSPlaying::DrawScene( void )
{
	m_map->Draw();
	m_player->Draw();;
	DrawParticles();
}

void GSPlaying::DrawUI( void )
{
	DrawExitLocationMessage();
	DrawOpenedContainer();
	m_informationsConsole->render( Globals::screen );
	m_playerBelt.Draw();
}

Map* GSPlaying::GetMapByName( std::string mapName )
{
	std::list<Map*>::iterator it;
	for( it = m_mapList.begin() ; it != m_mapList.end() ; ++it )
	{
		if( ( *it )->GetMapName() == mapName )
			return *it;
	}

	return NULL;
}

bool GSPlaying::MapExists( std::string mapName ) const
{
	std::list<Map*>::const_iterator cit;
	for( cit = m_mapList.begin() ; cit != m_mapList.end() ; ++cit )
	{
		if( ( *cit )->GetMapName() == mapName )
			return true;
	}

	return false;
}

void GSPlaying::ChangeMap( const std::string& mapName )
{
	FadeToBlack( Globals::screen, 1000 );

	for (const auto& map : m_mapList)
	{
		if (map->GetMapName() == mapName)
		{
			m_map = map;
			Globals::currentMap = m_map;

			std::cout << "// Zmiana mapy na " << map->GetMapName() << std::endl;
			return;
		}
	}

	// Dodano obsługę sytuacji, gdy mapa nie została znaleziona
	std::cerr << "// Błąd: Mapa o nazwie " << mapName << " nie znaleziona." << std::endl;
}

void GSPlaying::PlaceProtectionZone( Position& position )
{
	for( std::vector<Tile*>::iterator it = m_map->GetTilesVector().begin() ; it != m_map->GetTilesVector().end() ; ++it )
	{
		if( position.x == ( *it )->GetPosition().x && position.y == ( *it )->GetPosition().y )
		{
			( *it )->SetProtectionZone( true );
		}
	}
}

void GSPlaying::PlayerLeaveMap( void )
{
	Map* map = Globals::currentMap;
	if( m_player->GetPosition().x == map->GetExitPosition().x && m_player->GetPosition().y == map->GetExitPosition().y )
	{
		std::cout << "Opuszczamy mape" << std::endl;
		FadeToBlack( Globals::screen, 1000 );
		Globals::game->SetGameState( "World map" );


		// ---------------------- -----------BUG FIX ---------------------------------
		//fix buga, czasem po powrocie z lokalizacji nasza postac sama sie poruszala...
		//ktos cos? wazne, ze po dodaniu ponizszego kawalka kodu wszystko dziala :)
		//wyglada na to, ze input klawiatury dostal zawieszki :o
		for( int i = 0 ; i < 323 ; i++ )
		{
			m_keysHeld[i] = false;
		}
		//koniec fix buga
	}
}

void GSPlaying::DrawExitLocationMessage( void )
{
	if( m_player->GetPosition().x == m_map->GetExitPosition().x && m_player->GetPosition().y == m_map->GetExitPosition().y )
	{
		m_exitLocationMessage->show_text( 1280 / 2, 720 / 2, "Press ENTER to exit.", Globals::screen );
	}
}

void GSPlaying::DrawOpenedContainer( void )
{
	if( m_openedContainer )
	{
		m_openedContainer->Draw();
	}
}

void GSPlaying::CheckIfContainerIsOpened( void )
{
	if( m_openedContainer )
	{
		m_openedContainer->Think();
	}

	if( m_openedContainer && m_player->IsMoving() )
	{
		m_openedContainer = NULL;
	}
}

void GSPlaying::OpenMapEditorToEditCurrentMap( void )
{
	if (Globals::developer == true)
	{
		GSMapEditor* mapEditor = dynamic_cast<GSMapEditor*>(Globals::game->GetGameState( "Map editor" ));
		if (mapEditor)
		{
			mapEditor->GiveMap( m_map );
			mapEditor->SetBrushPosition( GetPlayer()->GetPosition() );
			mapEditor->SetMapEditorState( MAPPING );

			Globals::camera->FollowPlayer( false );
			Globals::game->SetGameState( "Map editor" );
		}
	}
}

void GSPlaying::OpenMapEditorNewMapWindow( void )
{
	if (Globals::developer == true)
	{
		GSMapEditor* mapEditor = dynamic_cast<GSMapEditor*>(Globals::game->GetGameState( "Map editor" ));
		if (mapEditor)
		{
			mapEditor->SetMapEditorState( MENU );
			Globals::game->SetGameState( "Map editor" );
		}
	}
}

void GSPlaying::PrintInformationsInConsole( void )
{
	std::cout << "**********************************" << std::endl;
	std::cout << m_player->GetPosition().x << "\t" << m_player->GetPosition().y << std::endl;
	std::cout << "Pozycja startowa mapy: " << Globals::currentMap->GetStartPosition().x << "\t" << Globals::currentMap->GetStartPosition().y << std::endl;
	std::cout << "Pozycja wyjsciowa mapy: " << Globals::currentMap->GetExitPosition().x << "\t" << Globals::currentMap->GetExitPosition().y << std::endl;
	std::cout << "Punkty zycia: " << m_player->GetHealthPoints() << "/" << m_player->GetMaxHealthPoints() << std::endl;
	std::cout << "Pozycja kamery: x: " << Globals::camera->GetCameraX() << " y: " << Globals::camera->GetCameraY() << std::endl;
	m_player->PrintLevelAdvanceInformations();
	std::cout << "**********************************" << std::endl;
}

void GSPlaying::MoveCameraOnMouseMotion( void )
{
	int mouse_x = Globals::event.motion.x;
	int mouse_y = Globals::event.motion.y;

	if (mouse_y < 2)
	{
		Globals::camera->velocity_y = -Globals::camera->cameraVelocity;
	}

	else if (mouse_y > Globals::resolution_y - 2)
	{
		Globals::camera->velocity_y = Globals::camera->cameraVelocity;
	}

	else if (mouse_x < 2)
	{
		Globals::camera->velocity_x = -Globals::camera->cameraVelocity;
	}

	else if (mouse_x > Globals::resolution_x - 2)
	{
		Globals::camera->velocity_x = Globals::camera->cameraVelocity;
	}

	else
	{
		Globals::camera->velocity_x = 0;
		Globals::camera->velocity_y = 0;

	}
}

void GSPlaying::ShakeScreen( SDL_Surface* screen, int shake_count, int shake_intensity )
{
	for (int i = 0; i < shake_count; i++) 
	{
		int x = rand() % shake_intensity - shake_intensity / 2;
		int y = rand() % shake_intensity - shake_intensity / 2;

		SDL_Rect offset;
		offset.x = x;
		offset.y = y;

		SDL_BlitSurface( screen, NULL, screen, &offset );
		SDL_UpdateRect( screen, 0, 0, 0, 0 );
		SDL_Delay( 50 );
	}
}

void GSPlaying::HandleMouseClickMapActions( int mouse_x, int mouse_y, std::vector<json>& actions )
{
	//todo
}

void GSPlaying::FadeToBlack( SDL_Surface* screen, int fadeTimeMs ) {
	// Create a temporary surface for the fade effect
	SDL_Surface* fadeSurface = SDL_CreateRGBSurface( SDL_SWSURFACE, screen->w, screen->h, 32,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask );

	if (!fadeSurface) {
		return; // Error handling
	}

	// Fill the fade surface with black
	SDL_FillRect( fadeSurface, NULL, SDL_MapRGB( fadeSurface->format, 0, 0, 0 ) );

	// Calculate steps for fade
	const int steps = 30; // Number of fade steps
	const int delay = fadeTimeMs / steps;

	// Set initial alpha to fully transparent
	SDL_SetAlpha( fadeSurface, SDL_SRCALPHA, 0 );

	// Fade loop
	for (int alpha = 0; alpha <= 255; alpha += (255 / steps)) {
		// Update alpha value
		SDL_SetAlpha( fadeSurface, SDL_SRCALPHA, alpha );

		// Draw the current screen content
		SDL_BlitSurface( screen, NULL, screen, NULL );

		// Draw fade surface over screen
		SDL_BlitSurface( fadeSurface, NULL, screen, NULL );

		// Update display
		SDL_Flip( screen );

		// Delay to control fade speed
		SDL_Delay( delay );
	}

	// Ensure final state is fully black
	SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );
	SDL_Flip( screen );

	// Clean up
	SDL_FreeSurface( fadeSurface );
}

void GSPlaying::EmitParticles( float worldX, float worldY, ParticleType type, int count )
{
	for (int i = 0; i < count; ++i)
	{
		m_particles.push_back( new Particle( worldX, worldY, type ) );
	}
}

void GSPlaying::DrawParticles( void )
{
	SDL_LockSurface( Globals::screen );
	for (auto* p : m_particles)
	{
		p->Draw( Globals::screen,
			Globals::camera->GetCameraX(),
			Globals::camera->GetCameraY() );
	}
	SDL_UnlockSurface( Globals::screen );
}

void GSPlaying::UpdateFlashlightBattery( float deltaTime )
{
	if (!IsFlashlightOn()) return;

	const float DRAIN_RATE = 1.0f / 600.0f; // 10 minut
	float current = Globals::player->GetFlashlightBattery();
	m_player->SetFlashlightBattery( current - DRAIN_RATE * deltaTime );
}

bool GSPlaying::IsFlashlightOn() const
{
	// Możesz tu dodać warunek: np. czy gracz trzyma latarkę
	return true; // albo np.: return m_playerHasFlashlight;
}

int GSPlaying::GetFlashlightRadius() const
{
	const int MIN_RADIUS = 20;
	const int MAX_RADIUS = 130;
	float battery = m_player->GetFlashlightBattery();
	return MIN_RADIUS + (int)((MAX_RADIUS - MIN_RADIUS) * battery);
}

void GSPlaying::EnsureFlashlightMask( int radius )
{
	if (m_currentMaskRadius == radius && m_flashlightMask)
		return; // już mamy odpowiednią maskę

	ReleaseFlashlightMask();
	m_flashlightMask = CreateFlashlightMask( radius );
	m_currentMaskRadius = radius;
}

void GSPlaying::ReleaseFlashlightMask()
{
	if (m_flashlightMask) {
		SDL_FreeSurface( m_flashlightMask );
		m_flashlightMask = nullptr;
		m_currentMaskRadius = -1;
	}
}

SDL_Surface* GSPlaying::CreateFlashlightMask( int radius ) const
{
	if (radius <= 0) return nullptr;

	int size = radius * 2;
	SDL_Surface* mask = SDL_CreateRGBSurface( SDL_SWSURFACE, size, size, 32,
		0x000000FF, 0x0000FF00, 0x00FF0000, 0 );

	if (!mask) return nullptr;

	Uint32* pixels = (Uint32*)mask->pixels;
	int pitch = mask->pitch / sizeof( Uint32 );

	for (int y = 0; y < size; ++y) {
		for (int x = 0; x < size; ++x) {
			int dx = x - radius;
			int dy = y - radius;
			int distSq = dx * dx + dy * dy;
			if (distSq <= radius * radius) {
				float dist = sqrtf( (float)distSq );
				float t = 1.0f - (dist / (float)radius);
				if (t < 0) t = 0;
				t = powf( t, 1.8f ); // miękki zanik
				Uint8 intensity = (Uint8)(t * 255);
				pixels[y * pitch + x] = SDL_MapRGB( mask->format, intensity, intensity, intensity );
			}
			else {
				pixels[y * pitch + x] = SDL_MapRGB( mask->format, 0, 0, 0 );
			}
		}
	}

	return mask;
}