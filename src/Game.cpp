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

void Game::FadeToBlack( int duration )
{
	SDL_Surface* screen = Globals::screen;
	int fadeSteps = 50; // Liczba kroków do powolnego przyciemniania ekranu
	int fadeDelay = duration / fadeSteps; // Czas oczekiwania między krokami

	Uint8 alpha = 255; // Początkowa wartość alpha (pełna widoczność)
	Uint32 fadeColor = SDL_MapRGBA( screen->format, 0, 0, 0, alpha ); // Kolor czarny z wartością alpha

	for (int i = 0; i < fadeSteps; i++) {
		alpha -= 255 / fadeSteps; // Zmniejszenie wartości alpha o 1/50 wartości
		fadeColor = SDL_MapRGBA( screen->format, 0, 0, 0, alpha ); // Aktualizacja koloru z nową wartością alpha

		SDL_FillRect( screen, NULL, fadeColor ); // Wypełnienie całego ekranu nowym kolorem
		SDL_Flip( screen ); // Odświeżenie ekranu

		SDL_Delay( fadeDelay ); // Oczekiwanie na kolejny krok
	}
}

void Game::fade_to_black( int delay )
{
	Uint32 color = SDL_MapRGB( Globals::screen->format, 0, 0, 0 );
	Uint8 alpha = 0;

	while (alpha < 255)
	{
		SDL_FillRect( Globals::screen, NULL, color );
		SDL_WM_SetCaption( "Fading to black...", NULL );
		SDL_Flip( Globals::screen );

		alpha += 1;
		color = SDL_MapRGB( Globals::screen->format, 0, 0, 0 );
		color = SDL_MapRGBA( Globals::screen->format, 0, 0, 0, alpha );

		SDL_Delay( delay );
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

		SDL_Delay( 10 );
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
