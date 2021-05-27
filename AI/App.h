#include "OLCInterface.h"

class App : public olc::PixelGameEngine
{
public:
	App()
	{
		sAppName = "AI App";
	}

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	std::vector<BTNode*> m_Nodes;
	std::vector<BTBlackboard*> m_BBs;
	std::vector<BTExpert*> m_Experts;
};

