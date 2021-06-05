#pragma once

#include "ColorConsole.h"
#include <string>
#include <iostream>
#include <any>

#define COMPARE_STRINGS(x, y) strcmp(x.c_str(), y) // Utility. Comparing strings.
#define COMPARE_STRINGS_2(x, y) strcmp(x.c_str(), y.c_str()) // Utility. Comparing strings.

enum AnyType {
	ANY_INT = 0,
	ANY_DOUBLE,
	ANY_FLOAT,
	ANY_STRING
};

class LuaBinding;

class Any {
	friend class LuaBinding;
public:
	Any(std::any value, AnyType type) : m_Value(value), m_Type(type) {}
	Any(std::any* value, int type) : m_Value(std::move(value)) {m_Type = AnyType(type);}
	Any(){}



	// Compares two "Any" values for equality.
	// Returns false on comparison of different types and
	// if types are equal but values are not.
	bool operator==(const Any& rhs) { return this->_equal(rhs.m_Value); }
	bool operator<(const Any& rhs) { return this->_smaller(rhs.m_Value); }
	bool operator>(const Any& rhs) { return this->_greater(rhs.m_Value); }
	
	Any& operator=(const Any&rhs)
	{
		Any a;
		a.m_Type = rhs.m_Type;
		a.m_Value = rhs.m_Value;
		return a;
	}



	// Return the type as unsigned int.
	AnyType type() const { return m_Type; }


	// Returns the data as std::any.
	std::any data() const { return m_Value; }


	// Use to access the value.
	// You have to know the type in order to convert and access it.
	//
	// So a naive solution would be a switch on "type()" of value...
	// 
	// Converting to string is somewhat special:
	// std::string s = other.as< const char* >();
	template < typename T >
	T as() {

		try {

			return std::any_cast<T>(m_Value);

		}
		catch (const std::bad_any_cast& e) {

			using namespace std;
			cout << color(colors::RED);
			cout << "Any: Bad Cast! From \"" << m_Value.type().name() << "\" to \"" << typeid(T).name() << "\"" << white << endl;

			throw runtime_error("Fatal bad cast!");
		}

	}


	void setType(int t)
	{
		m_Type = AnyType(t);
	}

	void setData(std::any d)
	{
		m_Value = d;
	}


private:

	std::any m_Value;
	AnyType m_Type;

private:

	bool _equal(std::any data);
	bool _smaller(std::any data);
	bool _greater(std::any data);
};

#pragma endregion any