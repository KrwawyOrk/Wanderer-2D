#ifndef CAMERA_H
#define CAMERA_H

#include "Globals.h"
#include "Position.h"

enum cameraDirection_t
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};

const int SCREEN_HEIGHT = Globals::resolution_x;
const int SCREEN_WIDTH = Globals::resolution_y;

class GameObject;
class Tile;

class Camera
{
public:
	Camera() : cam_x( 0 ), cam_y( 0 ), velocity_x( 0 ), velocity_y( 0 ), cameraVelocity( 1000 ), m_followPlayer( false ) {}

	int GetCameraX( void ) const { return cam_x; }
	int GetCameraY( void ) const { return cam_y; }

	void Center( GameObject* gameObject );
	void Center( int x, int y );
	void Center( Position& position )
	{
		Center( position.x, position.y );
	}

	void SetPosition(int x, int y )
	{
		cam_x = x;
		cam_y = y;
	}

	void FollowPlayer( bool b )
	{
		m_followPlayer = b;
	}

	bool GetCameraFollowPlayer( void )
	{
		return m_followPlayer;
	}

	bool InCameraView( GameObject* gameObject ) const;
	bool InCameraView( Tile* tile ) const;

	void Move( cameraDirection_t cameraDirection  );
	void Think( void );
	void Update( float deltaTime );

	int velocity_x;
	int velocity_y;
	const int cameraVelocity;

private:
	int cam_x;
	int cam_y;

	bool m_followPlayer;
};

#endif