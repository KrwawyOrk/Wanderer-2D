#include "Camera.h"

#include "GameObject.h"
#include "Globals.h"
#include "Tile.h"

#include "SDL.h"
#include <algorithm>
#include <cmath>

void Camera::Center( GameObject* gameObject ) {
	if (!gameObject) return;
	cam_x = gameObject->GetPosition().x * Globals::tilesize - Globals::resolution_x / 2;
	cam_y = gameObject->GetPosition().y * Globals::tilesize - Globals::resolution_y / 2;
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
	// 1. Manualny scroll (edytor, krawędzie myszki, klawisze)
	cam_x += static_cast<int>(velocity_x * deltaTime);
	cam_y += static_cast<int>(velocity_y * deltaTime);

	// 2. PŁYNNY FOLLOW ZA GRACZEM — tylko gdy włączony
	if (m_followPlayer && Globals::player)
	{
		float targetX = Globals::player->GetFloatX() - static_cast<float>(SCREEN_WIDTH) / 2.0f;
		float targetY = Globals::player->GetFloatY() - static_cast<float>(SCREEN_HEIGHT) * 0.32f;

		const float deadzone = 60.0f;     // Zwiększ do 60-80: mniej drgań!
		constexpr float smoothFull = 0.04f; // Pełna prędkość poza deadzone
		constexpr float smoothDead = 0.8f; // Bardzo wolna W deadzone (stabilność!)

		float diffX = targetX - cam_x;
		float diffY = targetY - cam_y;

		// Hybryda: zawsze lerp, ale z różną prędkością
		float currentSmoothX = (std::abs( diffX ) > deadzone) ? smoothFull : smoothDead;
		float currentSmoothY = (std::abs( diffY ) > deadzone) ? smoothFull : smoothDead;

		cam_x = static_cast<int>(cam_x + diffX * currentSmoothX);
		cam_y = static_cast<int>(cam_y + diffY * currentSmoothY);
	}

	// ——— SINUSOIDALNY SHAKE (najlepsza wersja) ———
	if (shakeTimer > 0.0f)
	{
		shakeTimer -= deltaTime;

		// <<< TUTAJ musi być tutaj – wewnątrz ifa! >>>
		static float time = 0.0f;                    // pamięta się między klatkami
		time += deltaTime * 18.0f;                   // ← reguluj szybkość fal (15–25)

		float offsetX = sinf( time ) * shakeStrength * 0.65f;   // lewo-prawo
		float offsetY = cosf( time * 1.15f ) * shakeStrength * 0.45f; // góra-dół, lekko inna faza

		cam_x += static_cast<int>(offsetX);
		cam_y += static_cast<int>(offsetY);

		// Decay
		shakeStrength *= 0.94f;   // 0.92–0.96 – im mniejsza wartość, tym szybciej wygasa

		// Koniec shake
		if (shakeTimer <= 0.0f)
		{
			shakeTimer = shakeStrength = 0.0f;
			time = 0.0f;           // reset czasu na następny shake
		}
	}
	// ——— KONIEC SHAKE ———

	// 3. Clamp do granic (żeby nie wychodziła poza świat)
	if (cam_x < 0) cam_x = 0;
	if (cam_y < 0) cam_y = 0;
	// (górne granice możesz dodać później na podstawie rozmiaru mapy)
}

void Camera::Think( void )
{
	if( cam_x < 0 )
		cam_x = 0;

	if( cam_y < 0 )
		cam_y = 0;
}

void Camera::StartShake( float strength, float duration )
{
	shakeStrength = strength;      // np. 4–10
	shakeDuration = duration;      // np. 0.2–0.5 s
	shakeTimer = duration;      // startujemy od pełnego czasu
}