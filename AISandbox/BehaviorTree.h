#pragma once

#include "ColorConsole.h"
#include "BTNodes.h"
#include "BTBlackboard.h"
#include "Any.h"


class BehaviorTree
{
public:
	BehaviorTree(std::string name) : m_Name(name) {}


	BTNodeResult update()
	{
		return m_Root->tick();
	}

	void setRoot(BTNode* node)
	{
		m_Root = node;
	}


public:

	BTNode* m_Root = nullptr;

	std::string m_Name;

};
