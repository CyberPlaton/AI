#pragma once

#include "BehaviorTree.h"
#include "VGJS.h"


class AIEngine
{
public:
	AIEngine(){}
	~AIEngine()
	{
		jSystem.terminate();
		jSystem.wait_for_termination();		
		m_Trees.clear();
	}


	void update()
	{
		using namespace std;

		for (auto& tree : m_Trees)
		{
#ifdef _DEBUG_OUT

			cout << color(colors::YELLOW);
			cout << "Behavior Tree Update: \"" << tree->m_Name << "\"" << white << endl;

			tree->update();
#else

			jSystem.schedule([&]() { tree->update(); });
#endif

		}
	}


	void addBT(BehaviorTree* tree)
	{
		m_Trees.push_back(tree);
	}


private:

	vgjs::JobSystem jSystem;
	std::vector<BehaviorTree*> m_Trees;
};