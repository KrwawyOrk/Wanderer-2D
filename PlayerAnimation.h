/*#ifndef PLAYERANIMATION_H
#define PLAYERANIMATION_H

class Player;
class Sprite;

namespace PlayerAnimationStatus
{
	enum animationStatus_t
	{
		IDLE = 0,
		WALKING_LEFT,
		WALKING_RIGHT,
		WALKING_UP,
		WALKING_DOWN
	};
}

class PlayerAnimation
{
public:
	PlayerAnimation();
	void Draw( void );

private:
	Player* m_player;
	int m_playerHeight;
	int m_playerWidth;
	animationStatus_t m_animationStatus;
	Sprite m_sprite;
};

#endif*/