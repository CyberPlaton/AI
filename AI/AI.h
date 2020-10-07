#pragma once

#include"GameObject.h"
#include"Platform.h"
#include"Ship.h"
#include"ColorConsole.h" // Include after Platform to avoid MINMAX collision from win32.


class AI : public olc::PixelGameEngine {
public:
	AI() {
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override{
		// Called once at the start, so create things here

		using namespace std;

		cout << APP_COLOR << "Program initialized. Starting..." << white << endl;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override{
		// called once per frame
#pragma omp parallel for
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
		return true;
	}
};
