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
		for (auto& tree : m_Trees)
		{
			jSystem.schedule([&]() { tree->update(); });
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