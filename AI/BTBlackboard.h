#pragma once

#include "BTNodes.h"
#include "Any.h"

#include <list>
#include <map>

class BTBlackboard;


class BTExpert
{
public:

	virtual float insistence(BTBlackboard*) = 0;
	virtual void run(BTBlackboard*) = 0;
	virtual std::string name() = 0;
};



class BTBlackboard
{
public:
	BTBlackboard(std::string name) : m_Name(name) {}


	void iterate(std::vector<BTExpert*> experts)
	{
		BTExpert* bestExpert = nullptr;
		float maxInsistence = 0.0f;

		for (auto& x : experts)
		{
			auto i = x->insistence(this);

			if (i > maxInsistence)
			{
				maxInsistence = i;
				bestExpert = x;
			}
		}



		if (bestExpert)
		{
			bestExpert->run(this);
		}

	}


	void set(std::string name, Any value)
	{
		m_Data[name].setData(value.data());
	}


	std::map<std::string, Any>& data()
	{
		return m_Data;
	}


	std::list<BTAction*> actions()
	{
		return m_PassedActions;
	}

private:

	std::string m_Name;

	std::map<std::string, Any> m_Data;

	std::list<BTAction*> m_PassedActions;
};