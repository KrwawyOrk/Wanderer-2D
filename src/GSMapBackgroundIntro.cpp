#include "GSMapBackgroundIntro.h"

#include "Globals.h"
#include "SpriteManager.h"

GSMapBackgroundIntro::GSMapBackgroundIntro()
{
	LoadIntroBackgrounds();
	m_introState = IntroState::LABORATORY;
}

void GSMapBackgroundIntro::InputEvents( void )
{
	//TODO
}

void GSMapBackgroundIntro::Think( void )
{
	//TODO
}

void GSMapBackgroundIntro::Draw( void )
{
	//TODO
	m_laboratoryBackground.Draw( Globals::screen, 0, 0 );
}

void GSMapBackgroundIntro::LoadIntroBackgrounds( void )
{
	//Ustawiamy standardowe tlo intro
	Globals::spriteManager->GetSprite( m_currentBackground, "intro_laboratory" );

	Globals::spriteManager->GetSprite( m_laboratoryBackground, "intro_laboratory" );
}