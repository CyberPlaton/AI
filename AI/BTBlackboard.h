#pragma once

#include "BTNodes.h"
#include "Any.h"

#include <list>
#include <map>
#include <mutex>


class LuaBinding;
class BTBlackboard
{
	friend class LuaBinding;
public:
	BTBlackboard(std::string name) : m_Name(name) {}

	
	void set(std::string name, Any value)
	{
		std::lock_guard lock(*m_Mutex);

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
		std::lock_guard lock(*m_Mutex);

		return m_Data[name].as< Type >();
	}



private:

	std::string m_Name;

	std::map<std::string, Any> m_Data;

	mutable std::mutex* m_Mutex = new std::mutex();
};