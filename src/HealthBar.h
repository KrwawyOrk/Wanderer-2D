#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "Creature.h"

class Creature;

//Obrazek z paskiem HP jest podzielony na dwie czesci. Bialo-szare tlo i czerwony pasek.
//Rozmiar obrazka: 300x50px, na tlo i czerwony pasek przypada po: 300x25px.

class HealthBar
{
public:
	HealthBar(Creature* creature);
	~HealthBar() { SDL_FreeSurface(m_surface); }

	void SetCreature( Creature* creature );
	void Draw( void );

private:
	void LoadFiles( void );

	Creature* m_creature;

	SDL_Surface* m_surface;
	SDL_Rect m_rect[2];
	SDL_Rect m_position;
};

#endif