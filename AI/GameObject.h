#pragma once

class GameObject;

struct GraphicsComponent {
	GraphicsComponent(float x, float y, float r, float g, float b) {

		m_XPos = x;
		m_YPos = y;
		m_R = r; m_G = g; m_B = b;
	}

	~GraphicsComponent() = default;

	void Draw() {


	}


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

		m_Size = 1;
		m_Acceleration = 0;
		m_Velocity = 0;
	}

	~PhysicsComponent() = default;



	int m_Size;
	float m_Acceleration;
	float m_Velocity;
	float m_XPos;
	float m_YPos;

};


class GameObject {
public:
	virtual ~GameObject(){}

	virtual void Update(float dt){}

	virtual bool IsColliding(GameObject* obj) { return false; }


	PhysicsComponent* physicsCmp;
	GraphicsComponent* graphicsCmp;
};