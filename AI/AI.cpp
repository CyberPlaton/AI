#include"AI.h"


class AISearch : public fsm::IStateLogic {
public:
	AISearch(AIComponent* cmp) {
		m_AICmp = cmp;
	}

	void execute() override {

		// Get random move direction
		
		float dirx = rand() % 2;
		float diry = rand() % 2;
		if (dirx == 0) dirx = -1.0f; dirx *= 100.f;
		if (diry == 0) diry = -1.0f; diry *= 100.f;
		

		// fly around...
		GameObject* object = m_AICmp->m_ManagedObject;
		if(object->m_Alive) {
			object->physicsCmp->m_Acceleration = 1.5f;

			object->Move(dirx, diry, object->physicsCmp->m_Acceleration);

			std::cout << APP_COLOR << "AI Moved" << white << std::endl;


			if (object->physicsCmp->m_XPos >= 900 || object->physicsCmp->m_XPos <= 1) m_AICmp->m_MoveDirectionX += dirx;
			if (object->physicsCmp->m_YPos >= 700 || object->physicsCmp->m_YPos <= 1) m_AICmp->m_MoveDirectionY += diry;
		}

	}

	AIComponent* m_AICmp = nullptr;
};

class AIPatrol : public fsm::IStateLogic {
public:

	// Circle = true	=> If endpoint reached restart patroling.
	// Circle = false	=> Transit to other specified state at endpoint.
	AIPatrol(AIComponent* cmp, bool circle = true, fsm::States endpointstate = fsm::STATE_INVALID) {

		m_AICmp = cmp;

		// specify endpoint state.
		m_TransitState = endpointstate;
		if (circle == false) { // Enforce a valid state to transit to.
			if (m_TransitState == fsm::STATE_INVALID) throw std::runtime_error(std::string("INVALID OR NO TRANSIT STATE SPECIFIED."));
		}
	}


	void AddPatrolPoint(olc::vi2d v) {

		m_PatrolPoints.push_back(v);
	}


	void execute() override {

		using namespace olc;
		using namespace std;

		Ship* object = static_cast<Ship*>(m_AICmp->m_ManagedObject);

		if (object->m_Alive) {
			if (m_PatrolPointReached == false) {

				object->physicsCmp->m_Acceleration += 1.5f;

				vi2d p1 = vi2d(m_PatrolPoints[0].x, m_PatrolPoints[0].y);
				vi2d p2 = vi2d(object->physicsCmp->m_XPos, object->physicsCmp->m_YPos);

				vi2d endpoint = p1 - p2;

				object->Move(endpoint.x, endpoint.y, object->physicsCmp->m_Acceleration);


				cout << color(colors::CYAN) << "AI left to reach patrolpoint -- " << endpoint.str() << white << endl;
				if (endpoint == vi2d(0, 0)) m_PatrolPointReached = true;

			}
			else {

				if (m_TransitState != fsm::STATE_INVALID && m_PatrolPoints.size() > 0) { // Valid transit state given => no looping of patrol route.

					if (m_PatrolPoints.size() > 0) { // Not all points reached, patrol further.

						m_PatrolPoints.erase(m_PatrolPoints.begin());
						m_PatrolPointReached = false;


						if (m_PatrolPoints.size() == 0) { // All patrol points reached, change state.

							object->aiCmp->ChangeState(m_TransitState);
						}
					}
				}
				else { // circle = true => looping of patrol route.

					// Append the current reached patrol point to the end.
					m_PatrolPoints.push_back(m_PatrolPoints[0]);
					m_PatrolPoints.erase(m_PatrolPoints.begin());

					m_PatrolPointReached = false;
				}
			}

		}
		else {
			cout << color(colors::CYAN) << "AI DEAD" << white << endl;
		}

	}

	fsm::States m_TransitState;
	bool m_PatrolPointReached = false;
	std::vector<olc::vi2d> m_PatrolPoints;
	AIComponent* m_AICmp = nullptr;
};


class AIShootBullet : public fsm::IStateLogic {
public:
	AIShootBullet(AIComponent* cmp) {
		m_AICmp = cmp;

		m_AIShip = static_cast<Ship*>(m_AICmp->m_ManagedObject);

	}

	void execute() override {

		using namespace std;
		using namespace olc;

		cout << APP_COLOR << "AI shot Bullets count -- "<< shotBullets << white << endl;

		if (m_AIShip->m_SlaveBullets.size() > 0) return;


		// TEST: Shoot on player.
		vi2d posvec = vi2d(m_AIShip->physicsCmp->m_XPos - 960/2, m_AIShip->physicsCmp->m_YPos - 720/2);
		posvec = posvec.norm();

		vi2d dirvec = vi2d(g_Objects[0]->physicsCmp->m_XPos, g_Objects[0]->physicsCmp->m_YPos);

		// Shoot with some randomization
		float offsetx, offsety;
		int specifier = 0;
		switch (m_AICmp->AILevel) {
		case AIComponent::AI_LEVEL_EASY:
			specifier = 75;
			break;
		case AIComponent::AI_LEVEL_NORMAL:
			specifier = 50;
			break;
		case AIComponent::AI_LEVEL_HARD:
			specifier = 20;
			break;
		default:
			break;
		}
		offsetx = rand() % specifier;
		offsety = rand() % specifier;
		offsetx = (offsetx <= specifier/2) ? -offsetx : offsetx;
		offsety = (offsety <= specifier/2) ? -offsety : offsety;


		Bullet* b = new Bullet(m_AIShip->physicsCmp->m_XPos + posvec.x, m_AIShip->physicsCmp->m_YPos + posvec.y,
			posvec.x + dirvec.x + offsetx, posvec.y + dirvec.y + offsety,
			0, 255, 100, m_AIShip, GraphicsComponent::GeometryType::CIRCLE);



		g_Objects.push_back(b);
		m_AIShip->m_SlaveBullets.push_back(b); // Save bullet in slave objects.
		shotBullets++;
	}
	static int shotBullets;
	Ship* m_AIShip = nullptr;
	olc::vi2d m_EnemyPos;
	AIComponent* m_AICmp = nullptr;
};
int AIShootBullet::shotBullets = 0;




int main()
{
	AI demo;
	
	if (demo.Construct(960, 720, 1, 1, true, true, false))
		demo.Start();


	return 0;
}


bool AI::OnUserCreate() {
	using namespace std;

	cout << APP_COLOR << "Program initialized. Starting..." << white << endl;

	SetPixelMode(olc::Pixel::ALPHA);

	_initScene();

	return true;
}


bool AI::OnUserUpdate(float fElapsedTime) {


	_handleKeyboard();
	_handleMouse();
	_update(fElapsedTime);


	Clear(olc::WHITE);
#ifdef _DEBUG
	_drawDebugGrid();
#endif // _DEBUG
	_draw();


	return true;
}


void AI::_initScene() {

	Ship* s1 = new Ship(500, 500, 150, 150, 255, GraphicsComponent::GeometryType::RECTANGLE);
	s1->m_ID = "Creator";

	// AI controlled ship
	Ship* s2 = new Ship(50, 100, 100, 100, 25, GraphicsComponent::GeometryType::RECTANGLE);
	s2->m_ID = "AI";
	s2->AddAIComponent();
	s2->aiCmp->MapState("search", new AISearch(s2->aiCmp));
	s2->aiCmp->MapState("patrol", new AIPatrol(s2->aiCmp, false, fsm::STATE_ATTACK));
	s2->aiCmp->MapState("shoot_player", new AIShootBullet(s2->aiCmp));

	AIPatrol* p = static_cast<AIPatrol*>(s2->aiCmp->m_StateLogicMap.at("patrol"));
	p->AddPatrolPoint(olc::vi2d(400, 400));
	p->AddPatrolPoint(olc::vi2d(10, 0));
	s2->aiCmp->ChangeState(fsm::STATE_PATROL);



	// Define some obstacles
	Ship* s3 = new Ship(450, 450, 125, 125, 125, GraphicsComponent::GeometryType::RECTANGLE);
	Ship* s4 = new Ship(400, 450, 125, 125, 125, GraphicsComponent::GeometryType::RECTANGLE);
	Ship* s5 = new Ship(350, 450, 125, 125, 125, GraphicsComponent::GeometryType::RECTANGLE);
	Ship* s6 = new Ship(450, 500, 125, 125, 125, GraphicsComponent::GeometryType::RECTANGLE);
	Ship* s7 = new Ship(500, 550, 125, 125, 125, GraphicsComponent::GeometryType::RECTANGLE);
	s3->m_ID = s4->m_ID = s5->m_ID = s6->m_ID = s7->m_ID = "Obstacle";

	g_Objects.push_back(s1);
	g_Objects.push_back(s2);


	g_Objects.push_back(s3);
	g_Objects.push_back(s4);
	g_Objects.push_back(s5);
	g_Objects.push_back(s6);
	g_Objects.push_back(s7);

}


void AI::_draw() {

	using namespace olc;

	for (auto it : g_Objects) {
	
		if (it->graphicsCmp->m_Geometry == 0) { // rect

			//DrawRect(it->physicsCmp->m_XPos, it->physicsCmp->m_YPos, it->physicsCmp->m_Size, it->physicsCmp->m_Size,
				//	Pixel(it->graphicsCmp->m_R, it->graphicsCmp->m_G, it->graphicsCmp->m_B));


			FillRect(it->physicsCmp->m_XPos, it->physicsCmp->m_YPos, it->physicsCmp->m_Size, it->physicsCmp->m_Size,
				Pixel(it->graphicsCmp->m_R, it->graphicsCmp->m_G, it->graphicsCmp->m_B));

		}
		else if (it->graphicsCmp->m_Geometry == 1) { // tri

			DrawTriangle(vi2d(it->physicsCmp->m_XPos, it->physicsCmp->m_YPos),
						vi2d(it->physicsCmp->m_XPos + it->physicsCmp->m_Size, it->physicsCmp->m_YPos),
					vi2d(it->physicsCmp->m_XPos, it->physicsCmp->m_YPos + it->physicsCmp->m_Size));
			
		}
		else if (it->graphicsCmp->m_Geometry == 2) { // circle

			FillCircle(vi2d(it->physicsCmp->m_XPos, it->physicsCmp->m_YPos), it->physicsCmp->m_Size, Pixel(it->graphicsCmp->m_R, it->graphicsCmp->m_G, it->graphicsCmp->m_B));

		}
		else if (it->graphicsCmp->m_Geometry == 3) { // line

			//DrawLine(vi2d(it->physicsCmp->m_XPos, it->physicsCmp->m_YPos), vi2d(it->physicsCmp->m_XDirection, it->physicsCmp->m_YDirection),
				//Pixel(it->graphicsCmp->m_R, it->graphicsCmp->m_G, it->graphicsCmp->m_B));
		}
	}


	// Draw debug circle around cubes
#pragma omp parallel
	for (auto it : g_Objects) {
		DrawCircle(vi2d(it->physicsCmp->m_XPos + it->physicsCmp->m_Size / 2, it->physicsCmp->m_YPos + it->physicsCmp->m_Size / 2), it->physicsCmp->m_Size, olc::DARK_GREEN, 255);
	}	


	std::string mouse_pos = "MOUSEPOS ( " + std::to_string(GetMousePos().x) + ":" + std::to_string(GetMousePos().y) + " )";
	DrawString(vi2d(0, 0), mouse_pos, olc::BLACK, 1);

	for (auto it : g_Objects) {
		DrawString(vi2d(it->physicsCmp->m_XPos - 20, it->physicsCmp->m_YPos - 10), it->m_ID,
			Pixel(it->graphicsCmp->m_R, it->graphicsCmp->m_G, it->graphicsCmp->m_B));
	}
}



void AI::_drawDebugGrid() {

	using namespace olc;

	int w = ScreenWidth();
	int h = ScreenHeight();
	int offsetx = w/47;
	int offsety = h/35;


	for (int i = 0; i < w; i += offsetx) {

		DrawLine(vi2d(i, 0), vi2d(i, h), olc::DARK_YELLOW);
	}

	for (int j = 0; j < h; j += offsety) {

		DrawLine(vi2d(0, j), vi2d(w, j), olc::DARK_YELLOW);
	}
}



#include<algorithm>
void AI::_update(float dt) {

	using namespace std;


	g_Objects[0]->physicsCmp->m_XDirection = GetMouseX();
	g_Objects[0]->physicsCmp->m_YDirection = GetMouseY();



	for (auto it : g_Objects) {

		if (it->IsAlive()) {
			it->Update(dt);
			it->RemoveDeadBullets();

			if (it->aiCmp != nullptr) {

				cout << color(colors::DARKMAGENTA) << "AI Ship pos (" << it->physicsCmp->m_XPos << ":" << it->physicsCmp->m_YPos << " )" << white << endl;
			}			
		}
		else { // Erase elements that are not alive.

			std::vector<GameObject*>::iterator iter = std::find(g_Objects.begin(), g_Objects.end(), it);
			if (iter != g_Objects.end()) {

				GameObject* s = *iter._Ptr;

				try { // Capsule deletion into try-catch, as it makes sometimes out-of-range errors.
					cout << color(colors::RED) << "Erased element -- " << s->m_ID << white << endl;
					g_Objects.erase(iter); // Erase found element.
				}
				catch (const char* err) {
					cout << color(colors::RED) << "Error on erasing element -- " << s->m_ID << white << endl;
				}
			}

		}
	}


	for (auto obj1 : g_Objects) {

		for (auto obj2 : g_Objects) {

			if (obj1 == obj2) continue; // do not resolve collision with self.
			else {

				if (obj1->IsColliding(obj2)) {
					obj1->ResolveCollision(obj2); // Resolve collision with other object.
				}
			}
		}
	}
	
}

void AI::_handleKeyboard() {

	using namespace std;

	float speed = 0.3142;

	if (GetKey(olc::Key::SHIFT).bHeld) {

		speed = 3.142;
	}


	if (GetKey(olc::Key::W).bHeld) { // Holding down button.

		g_Objects[0]->Move(0.0, -100.0, speed);
		cout << APP_COLOR << "Key::W " << white << endl;
	}
	
	if (GetKey(olc::Key::A).bHeld) {

		g_Objects[0]->Move(-100.0, 0.0, speed);
		cout << APP_COLOR << "Key::A" << white << endl;
	}

	if (GetKey(olc::Key::S).bHeld) {

		g_Objects[0]->Move(0.0, 100.0, speed);
		cout << APP_COLOR << "Key::S" << white << endl;
	}

	if (GetKey(olc::Key::D).bHeld) {

		g_Objects[0]->Move(100.0, 0.0, speed);
		cout << APP_COLOR << "Key::D" << white << endl;
	}

	if (GetKey(olc::Key::ESCAPE).bReleased) {
		cout << APP_COLOR << "Key::ESCAPE" << white << endl;
		exit(0);
	}

	if (GetKey(olc::Key::SPACE).bReleased) {

		for (auto it : g_Objects) {
			if (it->aiCmp != nullptr) {
				it->aiCmp->ChangeState(fsm::STATE_PATROL);
			}
		}
	}
}

void AI::_handleMouse() {

	if (GetMouse(0).bReleased) {

		using namespace olc;

		vi2d p1 = vi2d(g_Objects[0]->physicsCmp->m_XDirection, g_Objects[0]->physicsCmp->m_YDirection);
		vi2d p2 = vi2d(g_Objects[0]->physicsCmp->m_XPos, g_Objects[0]->physicsCmp->m_YPos);

		vi2d endpoint = p1 - p2;

		Bullet* b = new Bullet(g_Objects[0]->physicsCmp->m_XPos, g_Objects[0]->physicsCmp->m_YPos,
			endpoint.x, endpoint.y,
			254, 0, 0, g_Objects[0], GraphicsComponent::GeometryType::CIRCLE);


		g_Objects.push_back(b);
		static_cast<Ship*>(g_Objects[0])->m_SlaveBullets.push_back(b); // Save bullet in slave objects.
	}
}