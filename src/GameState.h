#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <iostream>

class GameState
{
public:
	static std::vector<GameState*> gameStatesVector;

	GameState()
	{
		gameStatesVector.push_back( this );
	}

	virtual ~GameState()
	{
		std::cout << "Niszcze obiekt klasy GameState." << std::endl;
	}

	virtual void InputEvents( void ) = 0;
	virtual void Think( void ) = 0;
	virtual void Update( float deltaTime ) = 0;
	virtual void Draw( void ) = 0;

	virtual void OnQuit( void ) = 0;
};

#endif