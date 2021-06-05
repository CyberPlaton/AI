#pragma once

#include "BehaviorTree.h"
#include "VGJS.h"
#include "BTFactory.h"


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


	bool reloadBTFromFile(const std::string& path, const std::string& treeName)
	{
		BehaviorTree* tree = BTFactory::readFromFile(path);

		if (tree)
		{
			for (auto& t : m_Trees)
			{
				if (t->m_Name.compare(treeName) == 0)
				{
					t = tree;
					return true;
				}
			}
		}


		return false;
	}

private:

	vgjs::JobSystem jSystem;
	std::vector<BehaviorTree*> m_Trees;
};