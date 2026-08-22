#include "Game.h"

#include "Camera.h"
#include "Factory.h"
#include "GameCursor.h"
#include "GameObject.h"
#include "GameState.h"
#include "Globals.h"
#include "Map.h"
#include "SpriteManager.h"
#include "Timer.h"

#include "GSPlaying.h"
#include "GSMapEditor.h"
#include "GSMap.h"
#include "GSMyHome.h"

#include "ProgressBar.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "SDL.h"
#include "SDL_ttf.h"
#include "nlohmann/json.hpp"

const int FRAMES_PER_SECOND = 30;

Game::Game()
{
	SDL_Init( SDL_INIT_EVERYTHING );
	TTF_Init();

	SDL_WM_SetCaption( "SDL", NULL );

	Factory* factory = new Factory;
	Globals::factory = factory;

	SpriteManager* spriteManager = new SpriteManager;
	Globals::spriteManager = spriteManager;

	Globals::camera = new Camera;

	//SDL_ShowCursor( 0 );
	m_cursor = new GameCursor;

	LoadConfiguration();
	SetScreenMode();

	m_gameState = NULL;
	m_running = true;

	Globals::developer = true;

	InitGameStates();

	std::cout << "Have Fun Playing Wanderer 2D!!! :-) - kind regards Krwawy Ork" << std::endl;
}

Game::~Game()
{
	delete Globals::spriteManager;
	delete Globals::camera;
	delete m_cursor;

	OnQuit();
	SDL_Quit();

	std::cout << "Niszcze obiekt klasy Game." << std::endl;
}

void Game::LoadConfiguration( void )
{
	std::ifstream file( "configuration.json" );
	nlohmann::json config;
	file >> config;

	Globals::fullScreen = config["fullscreen"] == 1 ? true : false;
	Globals::camera->FollowPlayer( config["camera_follow_player"] == 1 ? true : false );
}

void Game::InputEvents( void )
{
	while( SDL_PollEvent( &Globals::event ) )
	{
		if( m_gameState )
			m_gameState->InputEvents();
	}

}

void Game::Think( void )
{
	Globals::currentTime = static_cast<float>(SDL_GetTicks() / 1000.0f);

	if( m_gameState )
	{
		m_gameState->Think();
		//m_cursor->Think();
	}
}

void Game::Update( float deltaTime )
{
	if( m_gameState )
		m_gameState->Update( deltaTime );
}

void Game::Draw( void )
{
	if( m_gameState )
	{
		m_gameState->Draw();
		//m_cursor->Draw();
	}
}

bool Game::SetGameState( std::string gameStateTitle )
{
	ReleaseStuckKeys();

	std::map<std::string, GameState*>::iterator it = m_gameStateMap.find( gameStateTitle );

	if( it != m_gameStateMap.end() )
	{
		m_gameState = it->second;

		if( gameStateTitle == "Play" )
			m_gamestateEnum = PLAYING;

		else if( gameStateTitle == "Map editor" )
			m_gamestateEnum = MAPEDITOR;

		return true;
	}

	return false;
}

GameState* Game::GetGameState( std::string gameStateTitle )
{
	std::map<std::string, GameState*>::iterator it = m_gameStateMap.find( gameStateTitle );

	if( it != m_gameStateMap.end() )
	{
		return it->second;
	}

	return NULL;
}

void Game::InitGameStates( void )
{
	m_gameStateMap["Play"] = new GSPlaying;
	m_gameStateMap["Map editor"] = new GSMapEditor;
	m_gameStateMap["World map"] = new GSMap;

	m_gameStateMap["My home"] = new GSMyHome;

	SetGameState( "Play" );
}

void Game::SetScreenMode( void )
{
	if( Globals::fullScreen == true )
	{
		Globals::screen = SDL_SetVideoMode(
			Globals::resolution_x,
			Globals::resolution_y,
			32,
			SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF   // ← tylko to! (bez SDL_SRCALPHA!)
		);
	}

	else
	{
		Globals::screen = SDL_SetVideoMode(
			Globals::resolution_x,
			Globals::resolution_y,
			32,
			SDL_HWSURFACE | SDL_DOUBLEBUF   // ← tylko to! (bez SDL_SRCALPHA!)
		);
	}
}

void Game::OnQuit( void )
{

}

void Game::ToggleFullScreen( void )
{
	Uint32 flags = Globals::screen->flags;

	if (!Globals::fullScreen)
	{
		flags |= SDL_FULLSCREEN;
		Globals::fullScreen = true;
	}

	else {
		flags ^= SDL_FULLSCREEN;
		Globals::fullScreen = false;
	}

	Globals::fullScreen = SDL_SetVideoMode( Globals::resolution_x, Globals::resolution_y, 32, flags );
}

void Game::FadeToBlack( SDL_Surface* screen, int fadeTimeMs ) {
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

void Game::ReleaseStuckKeys( void )
{
	// Resetuje wszystkie klawisze, które mogły "zostać" wciśnięte
	// po powrocie z innego stanu gry (np. GSMyRoom)
	for (int i = 0; i < 323; i++)
	{
		( *Globals::keysHeld )[i] = false;
	}

	// Opcjonalnie: zresetuj również stany myszy jeśli używasz
	// m_mouseButtons[3] = { false, false, false };

	// Dodatkowo: wymuś aktualizację stanu SDL
	SDL_PumpEvents();

	// Opcjonalnie: wyczyść kolejkę zdarzeń (jeśli potrzeba)
	SDL_Event event;
	while (SDL_PollEvent( &event ))
	{
		// opróżniamy kolejkę
	}
}

int main( int argc, char* argv[] )
{
	srand( time(NULL) );

	Game* game = new Game;
	Globals::game = game;

	// Timing variables
	float old_time, current_time;
	float ftime;
	//-----------------

	// Need to initialize this here for event loop to work
	current_time = static_cast<float>(SDL_GetTicks() );
	//----------------------------------------------------

	//The frame rate regulator
    Timer fps;

	//ProgressBar progressBar( Globals::screen, 150, 10 );
	//progressBar.startAnimation( 5000 );

	if( Globals::spriteManager->SpritesLoadedSuccessfully() )	// Glowna petla gry rozpocznie prace tylko wtedy jezeli nie bylo problemow z zaladowaniem spritow.
	{
		while( Globals::game->GameIsRunning() )
		{
			//Start the frame timer
			fps.start();

			// Update the timing information
			old_time = current_time;
			current_time = static_cast<float>(SDL_GetTicks() );
			ftime = (current_time - old_time) / 1000.0f;
			Globals::deltaTime = ftime;

			Globals::game->InputEvents();
			Globals::game->Think();
			Globals::game->Update( ftime );
			//progressBar.update();

			/*if (progressBar.isFinished())
			{
				std::cout << "Bron zostala naladowana..." << std::endl;
				progressBar.startAnimation( 5000 );
			}*/

			SDL_FillRect( Globals::screen, &Globals::screen->clip_rect, SDL_MapRGB( Globals::screen->format, 0, 0, 0 ) );
			Globals::game->Draw();
			//progressBar.draw( static_cast<int>( Globals::player->GetFloatX() - Globals::camera->GetCameraX()), static_cast<int>(Globals::player->GetFloatY() - Globals::camera->GetCameraY() - 60 ) );

			SDL_Flip( Globals::screen );
		}

		int frameTicks = fps.get_ticks();

		if (frameTicks < 1000 / FRAMES_PER_SECOND)
		{
			SDL_Delay(
				(1000 / FRAMES_PER_SECOND) - frameTicks
			);
		}
	}

	delete game;

	system("pause");

	return 0;
}

#if defined(_MSC_VER) && (_MSC_VER >= 1900)
FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func( void )
{
	return _iob;
}
#endif
