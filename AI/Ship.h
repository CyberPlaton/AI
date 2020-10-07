#pragma once
#include "GameObject.h"


class Ship : public GameObject{
public:

	Ship(float x, float y, float r, float g, float b, GraphicsComponent::GeometryType type) {

		this->graphicsCmp = new GraphicsComponent(x, y, r, g, b, type);
		this->physicsCmp = new PhysicsComponent(x, y);
	}


	~Ship() = default;


	void Update(float dt = 0.16) {

		physicsCmp->m_Velocity = dt * physicsCmp->m_Acceleration;

		physicsCmp->m_XPos += dt * physicsCmp->m_Velocity;
		physicsCmp->m_YPos += dt * physicsCmp->m_Velocity;

		if (physicsCmp->m_XPos >= 648) {
			physicsCmp->m_XPos = 0;
		}
		if (physicsCmp->m_XPos <= 0) {
			physicsCmp->m_XPos = 648;
		}
		if (physicsCmp->m_YPos >= 480) {
			physicsCmp->m_YPos = 0;
		}
		if (physicsCmp->m_YPos <= 0) {
			physicsCmp->m_YPos = 480;
		}


		graphicsCmp->m_XPos = physicsCmp->m_XPos;
		graphicsCmp->m_YPos = physicsCmp->m_YPos;
	}

	bool IsColliding(GameObject* obj) override {

		bool collisionX = false;
		bool collisionY = false;

		float own_x_pos = physicsCmp->m_XPos;
		float own_y_pos = physicsCmp->m_YPos;

		if (own_x_pos + physicsCmp->m_Size >= obj->physicsCmp->m_XPos &&
			obj->physicsCmp->m_XPos + obj->physicsCmp->m_Size >= own_x_pos) collisionX = true;

		if (own_y_pos + physicsCmp->m_Size >= obj->physicsCmp->m_YPos &&
			obj->physicsCmp->m_YPos + obj->physicsCmp->m_Size >= own_y_pos) collisionY = true;

		return (collisionX && collisionY);
	}



};

