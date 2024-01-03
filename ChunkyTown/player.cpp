#include "engine.h"
#include "player.h"

Player::Player(const Vector3f& position, float rotX, float rotY) : m_position(position), m_rotX(rotX), m_rotY(rotY)
{

}

void Player::TurnLeftRight(float value)
{
	m_rotY += value * 0.10f;
}

void Player::TurnTopBottom(float value)
{
	m_rotX += value * 0.10f;
	if (m_rotX > 90)
		m_rotX = 90;
	if (m_rotX < -90)
		m_rotX = -90;
}

Vector3f Player::SimulateMove(bool front, bool back, bool left, bool right, bool jump, bool fly, float elapsedTime)
{
	Vector3f delta(0, 0, 0);
	float vitesse = 6.0f;

	if (front == true)
	{
		float xrotrad, yrotrad;
		yrotrad = (m_rotY / 180 * 3.141592654f);
		xrotrad = (m_rotX / 180 * 3.141592654f);
		delta.x += float(sin(yrotrad)) * elapsedTime * vitesse;
		delta.z -= float(cos(yrotrad)) * elapsedTime * vitesse;
	}
	if (back == true)
	{
		float xrotrad, yrotrad;
		yrotrad = (m_rotY / 180.0f * 3.141592654f);
		xrotrad = (m_rotX / 180.0f * 3.141592654f);
		delta.x -= float(sin(yrotrad)) * elapsedTime * vitesse;
		delta.z += float(cos(yrotrad)) * elapsedTime * vitesse;
	}
	if (left == true)
	{
		float yrotrad;
		yrotrad = (m_rotY / 180.0f * 3.141592654f);
		delta.x += float(cos(yrotrad)) * elapsedTime * vitesse;
		delta.z += float(sin(yrotrad)) * elapsedTime * vitesse;
	}
	if (right == true)
	{
		float yrotrad;
		yrotrad = (m_rotY / 180.0f * 3.141592654f);
		delta.x -= float(cos(yrotrad)) * elapsedTime * vitesse;
		delta.z -= float(sin(yrotrad)) * elapsedTime * vitesse;
	}
	if (jump == true)
	{
		if (bOnGround == true)
		{
			m_velocite = 25.f;
			bOnGround = false;
		}	
	}
	if (fly == true)
	{
		delta.y += 20 * elapsedTime;
	}
	return delta;
}

void Player::ApplyTransformation(Transformation& transformation) const
{
	transformation.ApplyRotation(-m_rotX, 1.f, 0, 0);
	transformation.ApplyRotation(-m_rotY, 0, 1.f, 0);
	transformation.ApplyTranslation(-m_position);
}

Vector3f Player::GetPosition()
{
	return this->m_position;
}

void Player::SetPosition(Vector3f pos)
{
	m_position = pos;
}
