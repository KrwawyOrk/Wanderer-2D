#include "HealthBar.h"

#include "Globals.h"
#include "SDL_image.h"

HealthBar::HealthBar(Player* player)
{
	m_player = player;
	m_surface = NULL;

	m_rect[0].x = 0;
	m_rect[0].y = 0;
	m_rect[0].w = 300;
	m_rect[0].h = 25;

	m_rect[1].x = 0;
	m_rect[1].y = 25;
	m_rect[1].w = (m_player->GetHealthPoints() * 100 / m_player->GetMaxHealthPoints() ) * 300 / 100;
	m_rect[1].h = 25;

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

	m_rect[1].w = (m_player->GetHealthPoints() * 100 / m_player->GetMaxHealthPoints() ) * 300 / 100;
	SDL_BlitSurface(m_surface, &m_rect[1], Globals::screen, &m_position);
}








