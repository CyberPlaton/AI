#pragma once

#include"GameObject.h"
#include"Platform.h"
#include"OLCGameEngineInterface.h"
#include"Ship.h"
#include"ColorConsole.h" // Include after Platform to avoid MINMAX collision from win32.



std::vector<GameObject*> g_Objects;


class AI : public olc::PixelGameEngine {
public:
	AI() {
		sAppName = "AI";
	}

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;


private:
	void _initScene();
	void _draw();
	void _update(float dt);
	void _handleKeyboard();
	void _handleMouse();
	void _drawDebugGrid();

};
