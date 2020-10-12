#pragma once

#include "GameObject.h"
#include "ColorConsole.h"


class Ship : public GameObject{
public:

	Ship(float x, float y, float r, float g, float b, GraphicsComponent::GeometryType type) {

		m_Alive = true;
		this->graphicsCmp = new GraphicsComponent(x, y, r, g, b, type);
		this->physicsCmp = new PhysicsComponent(x, y);
		physicsCmp->m_Size = 20;
		physicsCmp->m_IsCollidable = true;
	}


	~Ship() = default;

	void ApplyGravitation(float xdir, float ydir) override {

	}

	void AddAIComponent() {
		aiCmp = new AIComponent(this);
	}


	void Update(float dt) {

		int width = 945;
		int height = 705;
		m_FPS = dt;


		ApplyGravitation(0.0, 0.0); // Apply gravitation in direction of screen down.
		if (aiCmp != nullptr) {
			aiCmp->Update();
		}


		if (physicsCmp->m_XPos + physicsCmp->m_Size >= width) {
			physicsCmp->m_XPos= width-(physicsCmp->m_Size);
		}
		if (physicsCmp->m_XPos <= 0) {
			physicsCmp->m_XPos = 1;
		}
		if (physicsCmp->m_YPos + physicsCmp->m_Size >= height) {
			physicsCmp->m_YPos = height-(physicsCmp->m_Size);
		}
		if (physicsCmp->m_YPos <= 0) {
			physicsCmp->m_YPos = 1;
		}


		graphicsCmp->m_XPos = physicsCmp->m_XPos;
		graphicsCmp->m_YPos = physicsCmp->m_YPos;
	}



	 void Move(float xdir, float ydir, float acceleration) override {

		// 0.16f is FPS, but we may find a way to make it flexible.
		physicsCmp->m_Acceleration = acceleration;
		physicsCmp->m_Velocity += physicsCmp->m_Acceleration * m_FPS;

		physicsCmp->m_XPos += (100.0) * xdir *(physicsCmp->m_Velocity * m_FPS); // move in x look(!) direction.
		physicsCmp->m_YPos += (100.0) * ydir *(physicsCmp->m_Velocity * m_FPS); // move in y look (!) direction.


		physicsCmp->m_Acceleration = 0;
		physicsCmp->m_Velocity += m_FPS * (-physicsCmp->m_Velocity);
	}


	bool IsColliding(GameObject* obj) override {

		if (!obj->m_Alive) return false;

		for (auto it : m_SlaveBullets) {

			if (strcmp(it->m_ID.c_str(), obj->m_ID.c_str()) == 0) { // Dont collide with own bullets.
				return false;
			}
		}

		bool collision = false;

		float own_x_pos = physicsCmp->m_XPos;
		float own_y_pos = physicsCmp->m_YPos;
		int own_size = physicsCmp->m_Size;

		if (own_x_pos < (obj->physicsCmp->m_XPos + obj->physicsCmp->m_Size) &&
			(own_x_pos + own_size) > obj->physicsCmp->m_XPos &&
			own_y_pos < (obj->physicsCmp->m_YPos + obj->physicsCmp->m_Size) &&
			(own_y_pos + own_size) > obj->physicsCmp->m_YPos) {

			collision = true;

			physicsCmp->m_IsColliding = true;
			obj->physicsCmp->m_IsColliding = true;
		}
		return (collision);
	}


	void ResolveCollision(GameObject* obj) override{


		physicsCmp->m_Velocity = -physicsCmp->m_Velocity;

		physicsCmp->m_IsColliding = false;



		std::cout << APP_COLOR << "Collision at X::( "<< physicsCmp->m_XPos << " : " << obj->physicsCmp->m_XPos <<" )--Y::( " <<physicsCmp->m_YPos << " : " << obj->physicsCmp->m_YPos <<" )" << white << std::endl;
		std::cout << APP_COLOR << "Of Ships 1::" << this << " and 2::" << obj << white << std::endl;
	}


	float m_FPS;

	float m_PreviousXPos;
	float m_PreviousYPos;

	std::vector<Bullet*> m_SlaveBullets; // e.g. to non collide with.

};



