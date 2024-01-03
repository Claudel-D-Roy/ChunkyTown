#ifndef PLAYER_H__
#define PLAYER_H__

#include "transformation.h"

class Player
{
	public:
		Player(const Vector3f& position, float rotX = 0, float rotY = 0);

		Vector3f SimulateMove(bool front, bool back, bool left, bool right, bool jump, bool fly, float elapsedTime);
		Vector3f GetPosition();
		void SetPosition(Vector3f pos);
		void TurnLeftRight(float value);
		void TurnTopBottom(float value);
		void ApplyTransformation(Transformation& transformation) const;
		
		float m_velocite = 0;
		bool bOnGround = true;

	private:
		Vector3f m_position;
		float m_rotX = 0, m_rotY = 0;
};
#endif
