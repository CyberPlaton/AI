#include"AI.h"


int main()
{
	AI demo;
	
	if (demo.Construct(960, 720, 1, 1, false, true, false))
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
	_update(fElapsedTime);

	Clear(olc::VERY_DARK_GREY);
#ifdef _DEBUG
	_drawDebugGrid();
#endif // _DEBUG
	_draw();


	return true;
}


void AI::_initScene() {

	Ship* s1 = new Ship(500, 500, 150, 150, 255, GraphicsComponent::GeometryType::RECTANGLE);
	Ship* s2 = new Ship(50, 100, 100, 100, 25, GraphicsComponent::GeometryType::RECTANGLE);


	g_Objects.push_back(s1);
	g_Objects.push_back(s2);

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
	DrawString(vi2d(g_Objects[0]->physicsCmp->m_XPos-20, g_Objects[0]->physicsCmp->m_YPos-10), "Creator", olc::DARK_RED);

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

	g_Objects[0]->physicsCmp->m_XDirection = GetMouseX();
	g_Objects[0]->physicsCmp->m_YDirection = GetMouseY();



	for (auto it : g_Objects) {

		if (it->m_Alive) {
			it->Update(dt);
		}
		else { // Erase elements that are not alive.

			std::vector<GameObject*>::iterator iter = std::find(g_Objects.begin(), g_Objects.end(), it);
			if (iter != g_Objects.end()) {

				g_Objects.erase(iter); // Erase found element.
			}

		}
	}


	for (auto obj1 : g_Objects) {

		for (auto obj2 : g_Objects) {

			if (obj1 == obj2) continue; // do not resolve collision with self.
			else {

				if (obj1->IsColliding(obj2)) {
					obj1->ResolveCollision(obj2);
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

		g_Objects[0]->Move(0.0, -1.0, speed);
		cout << APP_COLOR << "Key::W " << white << endl;
	}
	
	if (GetKey(olc::Key::A).bHeld) {

		g_Objects[0]->Move(-1.0, 0.0, speed);
		cout << APP_COLOR << "Key::A" << white << endl;
	}

	if (GetKey(olc::Key::S).bHeld) {

		g_Objects[0]->Move(0.0, 1.0, speed);
		cout << APP_COLOR << "Key::S" << white << endl;
	}

	if (GetKey(olc::Key::D).bHeld) {

		g_Objects[0]->Move(1.0, 0.0, speed);
		cout << APP_COLOR << "Key::D" << white << endl;
	}

	if (GetKey(olc::Key::ESCAPE).bReleased) {
		cout << APP_COLOR << "Key::ESCAPE" << white << endl;
		exit(0);
	}


	if(GetMouse(0).bReleased){
	
		using namespace olc;

		vi2d posvec = vi2d(g_Objects[0]->physicsCmp->m_XPos - ScreenWidth()/2, g_Objects[0]->physicsCmp->m_YPos - ScreenHeight()/2);
		posvec = posvec.norm();

		vi2d dirvec = vi2d(g_Objects[0]->physicsCmp->m_XDirection - ScreenWidth() / 2, g_Objects[0]->physicsCmp->m_YDirection - ScreenHeight() / 2);
		//dirvec = dirvec.norm();
		

		Bullet* b = new Bullet(g_Objects[0]->physicsCmp->m_XPos+posvec.x, g_Objects[0]->physicsCmp->m_YPos+posvec.y,
								posvec.x+dirvec.x, posvec.y+dirvec.y,
								254, 0, 0, g_Objects[0], GraphicsComponent::GeometryType::CIRCLE);



		g_Objects.push_back(b);
		static_cast<Ship*>(g_Objects[0])->m_SlaveBullets.push_back(b); // Save bullet in slave objects.
	}

}