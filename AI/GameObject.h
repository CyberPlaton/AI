#pragma once

class GameObject;

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

		m_Size = 10;
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