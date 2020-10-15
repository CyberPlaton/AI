#pragma once

#include"GameObject.h"
#include"Platform.h"
#include"OLCGameEngineInterface.h"
#include"Ship.h"
#include"ColorConsole.h" // Include after Platform to avoid MINMAX collision from win32.


std::vector<GameObject*> g_Objects;


class AI : public olc::PixelGameEngine {
public:
	/*
	AI() {
		sAppName = "AI";
	}
	*/

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;


	static AI* Get();
	static void Del();

private:
	void _initScene();
	void _draw();
	void _update(float dt);
	void _handleKeyboard();
	void _handleMouse();
	void _drawDebugGrid();


	static AI* m_AIApp;
};


struct vec2d {
	float x;
	float y;
};

// Holds data needed for "understanding" the environment and computing solutions.
struct AIInformationAccumulator {


	void Update() {

		_accumulateEnemiePositionData();
	}


	// Whether this AI-Instance was hit.
	bool m_WasHit = false;
	std::map<vec2d*, GameObject*> m_EnemiesPositionsData; // Map objects to their positions.

private:
	void _accumulateEnemiePositionData() {

		for (auto it : g_Objects) {

			if (!(typeid(it) == typeid(Bullet))) { // Skip bullet positions
				vec2d pos;
				pos.x = it->physicsCmp->m_XPos;
				pos.y = it->physicsCmp->m_YPos;

				m_EnemiesPositionsData.insert(std::make_pair(&pos, it));
			}
		}
	}


	bool _isObjectVisible(float ownposx, float ownposy, GameObject* obj) {

		// Cast a ray (line) to object, if collision on way occurs, then obj not visible.
		if (1) { // returns true on collision.

			return false;
		}
		else return true;
	}
};
