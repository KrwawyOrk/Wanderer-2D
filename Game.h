#ifndef GAME_H
#define GAME_H

#include <map>
#include <string>

class GameCursor;
class GameState;
class Timer;

enum gamestate_t
{
	PLAYING = 1,
	MAPEDITOR,
	CRAFTMENU
};

class Game
{
public:
	Game();
	~Game();

	void InputEvents( void );
	void Think( void );
	void Update( float deltaTime );
	void Draw( void );

	bool SetGameState( std::string gameStateTitle );
	GameState* GetGameState( std::string gameStateTitle );

	bool GameIsRunning( void ) const { return m_running; }
	void Drop( void ) { m_running = false; }

	gamestate_t GetGameStateEnum( void ) const { return m_gamestateEnum; }

private:
	void InitGameStates( void );
	void SetScreenMode( void );
	void OnQuit( void );

	std::map<std::string, GameState*> m_gameStateMap;
	GameState* m_gameState;
	gamestate_t m_gamestateEnum;

	GameCursor* m_cursor;

	bool m_running;
};

#endif