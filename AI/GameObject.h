#pragma once

class GameObject;

#include"FiniteStateMachine.h"

struct GraphicsComponent {

	enum GeometryType {
		RECTANGLE = 0,
		TRIANGLE = 1,
		CIRCLE = 2,
		LINE = 3
	};


	GraphicsComponent(float x, float y, float r, float g, float b, GeometryType type) {

		m_XPos = x;
		m_YPos = y;
		m_R = r; m_G = g; m_B = b;
		m_Geometry = type;
	}

	~GraphicsComponent() = default;

	void Draw() {

	}

	// Geometry
	GeometryType m_Geometry;

	// Position
	float m_XPos;
	float m_YPos;

	// Color
	float m_R;
	float m_G;
	float m_B;
};



struct PhysicsComponent {
	PhysicsComponent(float x, float y) {
		m_XPos = x;
		m_YPos = y;

		m_Velocity = m_Acceleration = 0;
		m_Gravitation = 9.8;
	}

	~PhysicsComponent() = default;



	float m_Velocity; // speed
	float m_Acceleration;

	int m_Size;
	float m_XPos;
	float m_YPos;

	float m_XDirection; // Goto direction as 2D-vector.
	float m_YDirection;

	bool m_IsColliding = false;
	bool m_IsCollidable = false;


	// optional
	float m_Gravitation;
};



struct AIComponent {

};



class GameObject {
public:
	virtual ~GameObject(){}

	virtual void Update(float dt){}

	virtual bool IsColliding(GameObject* obj) { return false; }
	virtual void ResolveCollision(GameObject* obj) {}
	virtual void Move(float xdir, float ydir, float acceleration){}
	virtual void ApplyGravitation(float xdir, float ydir){}

	AIComponent* aiCmp = nullptr;
	PhysicsComponent* physicsCmp = nullptr;
	GraphicsComponent* graphicsCmp = nullptr;
	fsm::State* stateCmp = nullptr;

	bool m_Alive;
};


#include<iostream>
class Bullet : public GameObject{
public:
	Bullet(float xpos, float ypos, float xdir, float ydir,
			float r, float g, float b, GraphicsComponent::GeometryType type = GraphicsComponent::GeometryType::LINE) {

		m_Alive = true;

		this->graphicsCmp = new GraphicsComponent(xpos, ypos, r, g, b, type);
		this->physicsCmp = new PhysicsComponent(xpos, ypos);
		physicsCmp->m_Size = 2.0;

		physicsCmp->m_Acceleration = 1000.0;
		physicsCmp->m_Velocity = 3.142;
		physicsCmp->m_IsCollidable = true;
	}

	~Bullet() = default;


	void Update(float dt = 0.16) {

		int width = 945;
		int height = 705;


		Move(physicsCmp->m_XDirection, physicsCmp->m_YDirection, physicsCmp->m_Acceleration);


		if (physicsCmp->m_XPos + physicsCmp->m_Size >= width) {
			physicsCmp->m_XPos = width - (physicsCmp->m_Size);
		}
		if (physicsCmp->m_XPos <= 0) {
			physicsCmp->m_XPos = 1;
		}
		if (physicsCmp->m_YPos + physicsCmp->m_Size >= height) {
			physicsCmp->m_YPos = height - (physicsCmp->m_Size);
		}
		if (physicsCmp->m_YPos <= 0) {
			physicsCmp->m_YPos = 1;
		}


		graphicsCmp->m_XPos = physicsCmp->m_XPos;
		graphicsCmp->m_YPos = physicsCmp->m_YPos;

		m_FPS = dt;
	}


	void Move(float xdir, float ydir, float acceleration) override {

		xdir = -1.0;
		ydir = -1.0;

		// 0.16f is FPS, but we may find a way to make it flexible.
		physicsCmp->m_Acceleration = acceleration;
		physicsCmp->m_Velocity += physicsCmp->m_Acceleration * m_FPS;

		physicsCmp->m_XPos += xdir * (physicsCmp->m_Velocity * m_FPS); // move in x look(!) direction.
		physicsCmp->m_YPos += ydir * (physicsCmp->m_Velocity * m_FPS); // move in y look (!) direction.


		physicsCmp->m_Velocity += m_FPS * (-physicsCmp->m_Velocity);


		using namespace std;
		if (physicsCmp->m_Velocity <= 0) {
		
			m_Alive = false;
		}
		std::cout << "Bullet velocity " << physicsCmp->m_Velocity << std::endl;
		std::cout << "Bullet acceleration " << physicsCmp->m_Acceleration << std::endl;
		std::cout << "Bullet pos " << physicsCmp->m_XPos << ":" << physicsCmp->m_YPos <<std::endl;
	}


	bool IsColliding(GameObject* obj) override {

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


	void ResolveCollision(GameObject* obj) override {

		m_Alive = false;
	}

private:
	float m_FPS;

private:

};