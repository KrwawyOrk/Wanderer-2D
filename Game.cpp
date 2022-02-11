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

#include "GSBattleFight.h"
#include "GSPlaying.h"
#include "GSMapEditor.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "SDL.h"
#include "SDL_ttf.h"

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
	Globals::camera->FollowPlayer( true );

	//SDL_ShowCursor( 0 );
	m_cursor = new GameCursor;

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

	SetGameState( "Play" );
}

void Game::SetScreenMode( void )
{
	if( Globals::fullScreen == true )
	{
		Globals::screen = SDL_SetVideoMode( Globals::resolution_x, Globals::resolution_y, 32, SDL_HWSURFACE | SDL_FULLSCREEN );
	}

	else
	{
		Globals::screen = SDL_SetVideoMode( Globals::resolution_x, Globals::resolution_y, 32, SDL_SWSURFACE );
	}
}

void Game::OnQuit( void )
{

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

			SDL_FillRect( Globals::screen, &Globals::screen->clip_rect, SDL_MapRGB( Globals::screen->format, 0, 0, 0 ) );
			Globals::game->Draw();

			SDL_Flip( Globals::screen );	
		}

		SDL_Delay( 1 );
	}

	delete game;

	system("pause");

	return 0;
}

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func( void )
{
	return _iob;
}


