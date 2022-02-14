#include "Camera.h"

#include "GameObject.h"
#include "Globals.h"
#include "Tile.h"

void Camera::Center( GameObject* gameObject )
{
	cam_x = static_cast<int>( gameObject->GetPosition().x * Globals::tilesize + Globals::tilesize / 2 ) - SCREEN_HEIGHT / 2;
	cam_y = static_cast<int>( gameObject->GetPosition().y * Globals::tilesize + Globals::tilesize / 2 ) - SCREEN_WIDTH / 2 + 50; // dodatkowo 50 pixeli wiecej, poniewaz uwzgledniam pasek interfejsu
}

void Camera::Center( int x, int y )
{
	cam_x = static_cast<int>( x * Globals::tilesize + Globals::tilesize / 2 ) - SCREEN_HEIGHT / 2;
	cam_y = static_cast<int>( y * Globals::tilesize + Globals::tilesize / 2 ) - SCREEN_WIDTH / 2 + 50;
}

void Camera::Move( cameraDirection_t cameraDirection )
{
	switch( cameraDirection )
	{
	case UP:
		cam_y -= Globals::tilesize;
		break;

	case DOWN:
		cam_y += Globals::tilesize;
		break;

	case LEFT:
		cam_x -= Globals::tilesize;
		break;

	case RIGHT:
		cam_x += Globals::tilesize;
		break;
	}
}

bool Camera::InCameraView( GameObject* gameObject ) const
{
	if( gameObject->GetPosition().x * Globals::tilesize > cam_x - Globals::tilesize && gameObject->GetPosition().x * Globals::tilesize < cam_x + SCREEN_HEIGHT && gameObject->GetPosition().y * Globals::tilesize > cam_y - Globals::tilesize && gameObject->GetPosition().y * Globals::tilesize < cam_y + SCREEN_WIDTH )
	{
		return true;
	}

	return false;
}

bool Camera::InCameraView( Tile* tile ) const
{
	if( tile->GetPosition().x * Globals::tilesize > cam_x - Globals::tilesize && tile->GetPosition().x * Globals::tilesize < cam_x + SCREEN_HEIGHT && tile->GetPosition().y * Globals::tilesize > cam_y - Globals::tilesize && tile->GetPosition().y * Globals::tilesize < cam_y + SCREEN_WIDTH )
	{
		return true;
	}

	return false;
}

void Camera::Update( float deltaTime )
{
	if( m_followPlayer == true )
	{
		cam_x = static_cast<int>( ( Globals::player->GetFloatX() ) - SCREEN_HEIGHT / 2 );
		cam_y = static_cast<int>( ( Globals::player->GetFloatY() ) - SCREEN_WIDTH / 2 + 50 );
	}

	else
	{
		cam_x += static_cast<int>(velocity_x * deltaTime);
		cam_y += static_cast<int>(velocity_y * deltaTime);
	}
}

void Camera::Think( void )
{
	if( cam_x < 0 )
		cam_x = 0;

	if( cam_y < 0 )
		cam_y = 0;
}