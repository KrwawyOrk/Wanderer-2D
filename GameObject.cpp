#include "GameObject.h"
#include "Camera.h"
#include "Globals.h"
#include "SpriteManager.h"

GameObject::GameObject()
{
	flposition_x = static_cast<float>( m_position.x * Globals::tilesize );
	flposition_y = static_cast<float>( m_position.y * Globals::tilesize );
	m_velocity = 0;
}

void GameObject::Update( float deltaTime )
{
	if( flposition_x < m_position.x * Globals::tilesize )
	{
		flposition_x = std::min( flposition_x + m_velocity * deltaTime, static_cast<float>( m_position.x * Globals::tilesize ) );
	}

	else if( flposition_x > m_position.x * Globals::tilesize )
	{
		flposition_x = std::max( flposition_x - m_velocity * deltaTime, static_cast<float>( m_position.x * Globals::tilesize ) );
	}

	if( flposition_y < m_position.y * Globals::tilesize )
	{
		flposition_y = std::min( flposition_y + m_velocity * deltaTime, static_cast<float>( m_position.y * Globals::tilesize ) );
	}

	else if( flposition_y > m_position.y * Globals::tilesize )
	{
		flposition_y = std::max( flposition_y - m_velocity * deltaTime, static_cast<float>( m_position.y * Globals::tilesize ) );
	}
}

void GameObject::Draw( void )
{
	m_sprite.Draw( Globals::screen, static_cast<int>( flposition_x - Globals::camera->GetCameraX() ), static_cast<int>( flposition_y - Globals::camera->GetCameraY() - m_sprite.GetSDLSurface()->h + Globals::tilesize ) );
}

void GameObject::Move( direction_t direction )
{
	switch( direction )
	{
	case NORTH:
		m_position.y--;
		break;

	case SOUTH:
		m_position.y++;
		break;

	case EAST:
		m_position.x++;
		break;

	case WEST:
		m_position.x--;
		break;
	}
}

bool GameObject::IsMoving( void )
{
	return !( flposition_x == Globals::tilesize * m_position.x && flposition_y == Globals::tilesize * m_position.y );
}
	