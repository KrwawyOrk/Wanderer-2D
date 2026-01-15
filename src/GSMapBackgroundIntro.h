#ifndef GSMAPBACKGROUNDINTRO_H
#define GSMAPBACKGROUNDINTRO_H

#include "GameState.h"

#include <iostream>
#include <string>

#include "Sprite.h"

enum class IntroState {
	LABORATORY = 1,
};

class GSMapBackgroundIntro : public GameState
{
public:
	GSMapBackgroundIntro();
	virtual ~GSMapBackgroundIntro()
	{
		std::cout << "Niszcze obiekt klasy GSMapBackgroundIntro" << std::endl;
	}

	virtual void InputEvents( void );
	virtual void Think( void );
	virtual void Update( float deltaTime ) { return; }
	virtual void Draw( void );

	virtual void OnQuit( void ) { return; }

	void LoadIntroBackgrounds( void );

private:
	Sprite m_currentBackground;
	Sprite m_laboratoryBackground;

	IntroState m_introState;
};

#endif


