#pragma once

#include "BTNodes.h"
#include "Any.h"

#include <list>
#include <map>
#include <mutex>


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
		std::lock_guard lock(m_Mutex);

		m_Data[name].setData(value.data());
	}

	
	/*
	* Function should not be used.
	* An will be not available later.
	* 
	* For now we let it be for tests sake...
	*/
	std::map<std::string, Any>& data()
	{
		return m_Data;
	}



	template < typename Type >
	Type getData(std::string name)
	{
		std::lock_guard lock(m_Mutex);

		return m_Data[name].as< Type >();
	}




	std::list<BTAction*> actions()
	{
		return m_PassedActions;
	}

private:

	std::string m_Name;

	std::map<std::string, Any> m_Data;

	std::list<BTAction*> m_PassedActions;

	mutable std::mutex m_Mutex{};
};