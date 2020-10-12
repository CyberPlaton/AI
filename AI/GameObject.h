#pragma once

class GameObject;

#include"FiniteStateMachine.h"
#include"Platform.h"

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


/*
Note:
	- we do not use the message-event-system, consider using it in "real" engine.
	- state-machine for functions and logic.
	
*/
struct AIComponent {

	AIComponent(GameObject* go) {

		m_ManagedObject = go;
		m_State = new fsm::State(fsm::States::STATE_WAIT);
	}

	~AIComponent() = default;

	void MapState(std::string name, fsm::IStateLogic* logic) {

		m_StateLogicMap.insert(std::make_pair(name, logic));
	}

	void UnmapLogic(std::string name) {

		m_StateLogicMap.erase(m_StateLogicMap.find(name));
	}

	void ChangeState(fsm::States newState) {
		m_State->NewState(newState);
	}

	void Update() {
		ExecuteStateLogic();
	}


	bool ExecuteStateLogic() {
		switch (m_State->GetState())
		{
		case fsm::STATE_INVALID:
			return false;
			break;

		case fsm::STATE_WAIT:

			m_StateLogicMap.at("wait")->execute();
			break;

		case fsm::STATE_SEARCH:

			m_StateLogicMap.at("search")->execute();
			break;

		case fsm::STATE_ATTACK:
			break;

		case fsm::STATE_DEFEND:
			break;

		case fsm::STATE_FLEE:
			break;

		case fsm::STATE_DIE:
			break;

		default:
			return false;
			break;
		}
	};

	std::map<std::string, fsm::IStateLogic*> m_StateLogicMap;
	GameObject* m_ManagedObject;
	fsm::State* m_State;


	// Direction to move of managed gameobject.
	float m_MoveDirectionX;
	float m_MoveDirectionY;


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
	std::string m_ID; // Every bullets gets an ID, maybe ships too.
};



class Bullet : public GameObject{
public:
	Bullet(float xpos, float ypos, float xdir, float ydir,
			float r, float g, float b,
		GameObject* master, GraphicsComponent::GeometryType type = GraphicsComponent::GeometryType::LINE) {


		std::stringstream stream;
		stream << static_cast<const void*>(this);
		m_ID = stream.str();



		m_Alive = true;
		m_MasterObject = master;
		this->graphicsCmp = new GraphicsComponent(xpos, ypos, r, g, b, type);

		this->physicsCmp = new PhysicsComponent(xpos, ypos);
		physicsCmp->m_XDirection = xdir;
		physicsCmp->m_YDirection = ydir;
		physicsCmp->m_Size = 3.0;
		physicsCmp->m_Acceleration = 1.0;
		physicsCmp->m_Velocity = 3.142;
		physicsCmp->m_IsCollidable = true;
	}

	~Bullet() = default;


	void Update(float dt = 0.16) {

		int width = 945;
		int height = 705;


		Move(physicsCmp->m_XDirection, physicsCmp->m_YDirection, physicsCmp->m_Acceleration);
		

		if (physicsCmp->m_XPos + physicsCmp->m_Size >= width) {
			m_Alive = false;
		}
		if (physicsCmp->m_XPos <= 0) {
			m_Alive = false;
		}
		if (physicsCmp->m_YPos + physicsCmp->m_Size >= height) {
			m_Alive = false;
		}
		if (physicsCmp->m_YPos <= 0) {

			m_Alive = false;
		}


		graphicsCmp->m_XPos = physicsCmp->m_XPos;
		graphicsCmp->m_YPos = physicsCmp->m_YPos;


		if (physicsCmp->m_XPos == physicsCmp->m_XDirection &&
			physicsCmp->m_YPos == physicsCmp->m_YDirection) {

			std::cout << "Bullet reached target and ceased." << std::endl;
			m_Alive = false;
		}

		m_FPS = dt;
	}


	void Move(float xdir, float ydir, float acceleration) override {

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

		if (obj == m_MasterObject) return false; // bullets dont collide with own shooter

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
	GameObject* m_MasterObject = nullptr;

private:

};