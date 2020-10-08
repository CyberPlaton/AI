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
	_initScene();


	return true;
}

bool AI::OnUserUpdate(float fElapsedTime) {



	_update(fElapsedTime);
	_handleKeyboard();
	_draw();

	return true;
}


void AI::_initScene() {

	Ship* s1 = new Ship(10, 10, 150, 150, 255, GraphicsComponent::GeometryType::RECTANGLE);
	


	g_Objects.push_back(s1);
}


void AI::_draw() {

	using namespace olc;


	Clear(VERY_DARK_BLUE);


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

			DrawCircle(vi2d(it->physicsCmp->m_XPos, it->physicsCmp->m_YPos), it->physicsCmp->m_Size);

		}
		else if (it->graphicsCmp->m_Geometry == 3) { // line

			// Skip line for now.
		}
	}



	DrawString(vi2d(0, 0), "something", olc::WHITE, 1);

}


void AI::_update(float dt) {

}

void AI::_handleKeyboard() {

	using namespace std;

	if (GetKey(olc::Key::W).bHeld) { // Holding down button.

		g_Objects[0]->physicsCmp->m_YPos -= 1;
		cout << APP_COLOR << "Key::W " << white << endl;

	}
	
	if (GetKey(olc::Key::A).bHeld) {

		g_Objects[0]->physicsCmp->m_XPos -= 1;
		cout << APP_COLOR << "Key::A" << white << endl;

	}

	if (GetKey(olc::Key::S).bHeld) {

		g_Objects[0]->physicsCmp->m_YPos += 1;
		cout << APP_COLOR << "Key::S" << white << endl;

	}

	if (GetKey(olc::Key::D).bHeld) {

		g_Objects[0]->physicsCmp->m_XPos += 1;
		cout << APP_COLOR << "Key::D" << white << endl;

	}

	if (GetKey(olc::Key::ESCAPE).bReleased) {
		cout << APP_COLOR << "Key::ESCAPE" << white << endl;
		exit(0);
	}


}