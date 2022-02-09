#include "HealthBar.h"

#include "Globals.h"
#include "SDL_image.h"

HealthBar::HealthBar(Creature* creature)
{
	m_creature = creature;
	m_surface = NULL;

	m_rect[0].x = 0;
	m_rect[0].y = 0;
	m_rect[0].w = m_surface->w;
	m_rect[0].h = m_surface->h / 2;

	m_rect[1].x = 0;
	m_rect[1].y = m_surface->h / 2;
	m_rect[1].w = (m_creature->GetHealthPoints() * 100 / m_creature->GetMaxHealthPoints() ) * m_surface->w / 100;
	m_rect[1].h = m_surface->h / 2;

	m_position.x = 250;
	m_position.y = 10;

	LoadFiles();
}

void HealthBar::LoadFiles( void )
{
	m_surface = IMG_Load( "grafika/healthBar.bmp" );
}

void HealthBar::Draw( void )
{
	SDL_BlitSurface(m_surface, &m_rect[0], Globals::screen, &m_position);

	m_rect[1].w = (m_creature->GetHealthPoints() * 100 / m_creature->GetMaxHealthPoints() ) * m_surface->w / 100;
	SDL_BlitSurface(m_surface, &m_rect[1], Globals::screen, &m_position);
}

void HealthBar::SetCreature( Creature* creature )
{
	if( creature )
	{
		m_creature = creature;
	}
}








