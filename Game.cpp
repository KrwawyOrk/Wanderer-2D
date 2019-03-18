#include "Game.h"

#include "Camera.h"
#include "Factory.h"
#include "GameCursor.h"
#include "GameObject.h"
#include "GameState.h"
#include "Globals.h"
#include "Map.h"
#include "SpriteManager.h"

#include "GSBattleFight.h"
#include "GSPlaying.h"
#include "GSMapEditor.h"
#include "GSCraftMenu.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "SDL_ttf.h"
#include "Timer.h"

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

	SetScreenMode();

	m_gameState = NULL;
	m_running = true;

	Globals::developer = true;

	InitGameStates();
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
		Globals::currentTime = SDL_GetTicks() / 1000;

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

		else if( gameStateTitle == "Craft menu" )
			m_gamestateEnum = CRAFTMENU;

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
	m_gameStateMap["Craft menu"] = new GSCraftMenu;

	SetGameState( "Play" );
}

void Game::SetScreenMode( void )
{
	if( Globals::fullScreen == true )
	{
		Globals::screen = SDL_SetVideoMode( Globals::resolution_x, Globals::resolution_y, 32, SDL_SWSURFACE | SDL_FULLSCREEN );
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
	Uint32 old_time, current_time;
	float ftime;
	//-----------------

	// Need to initialize this here for event loop to work
	current_time = SDL_GetTicks();
	//----------------------------------------------------

	if( Globals::spriteManager->SpritesLoadedSuccessfully() )	// Glowna petla gry rozpocznie prace tylko wtedy jezeli nie bylo problemow z zaladowaniem spritow.
	{
		while( Globals::game->GameIsRunning() )
		{
			// Update the timing information
			old_time = current_time;
			current_time = SDL_GetTicks();
			ftime = (current_time - old_time) / 1000.0f;
			Globals::deltaTime = ftime;

			Globals::game->InputEvents();
			Globals::game->Think();
			Globals::game->Update( ftime );

			SDL_FillRect( Globals::screen, &Globals::screen->clip_rect, SDL_MapRGB( Globals::screen->format, 0, 0, 0 ) );
			Globals::game->Draw();

			SDL_Flip( Globals::screen );

			SDL_Delay( 1 );
		}
	}

	delete game;

	system("pause");

	return 0;
}


