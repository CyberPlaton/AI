#ifndef SCRIPT_H
#define SCRIPT_H
#pragma once

#include"LuaBinding.h"

/*
 * A script is a wrapper class for a lua script.
 */
class Script
{
public:
	Script(const std::string& filePath)
	{
		m_FilePath = filePath;

		// Load Lua Script and Check for errors..
		int result = luaL_loadfile(LuaBinding::getLua(), m_FilePath.c_str());
		if (result != LUA_OK)
		{
			std::string errormsg = lua_tostring(LuaBinding::getLua(), -1);
			std::cout << errormsg << std::endl;

			using namespace std;
			cout << color(colors::RED);
			cout << "[C++] Severe Script Loading Error in \"" << m_FilePath << "\": " << errormsg << "\"" << white << endl;


			throw std::runtime_error("Script Invalid.");
		}

	}


	// Push function to be executed next.
	void pushFunction(const std::string& funcName)
	{
		m_CurrentFunction = funcName;
	}


	// Execute the last pushed function. Here the user only specifies the parameters for it.
	//
	// If the parameters are false an error will occur but the program will move on.
	// The return values are "LUA_OK"(or another string from the lua script function) or "LUA_ERROR" on error.
	//
	template <class... Args>
	std::string execute(Args... args) {

		// Errors in scripts are not lethal.
		// We only return a string indicating the error and let it silently move on...
		//
		try {

			// Load script..
			int result = lua_pcall(LuaBinding::getLua(), 0, LUA_MULTRET, 0);
			if (result != LUA_OK) return nullptr;

			luabridge::LuaRef luaFunc = luabridge::getGlobal(LuaBinding::getLua(), m_CurrentFunction.c_str());

			m_CurrentFunction = "";


			return luaFunc(args...).cast<std::string>();
		}
		catch (luabridge::LuaException& e) {


			using namespace std;
			cout << color(colors::RED);
			cout << "[C++] Severe Script Error in \"" << m_FilePath << "\"::\"" << m_CurrentFunction << "\" :: " << e.what() << white << endl;


			m_CurrentFunction = "";

			return "LUA_ERROR";
		}
	}




	template <typename ReturnType>
	ReturnType* execute() {

		// Errors in scripts are not lethal.
		// We only return a string indicating the error and let it silently move on...
		//
		try {

			// Load script..
			int result = lua_pcall(LuaBinding::getLua(), 0, LUA_MULTRET, 0);
			if (result != LUA_OK) return nullptr;

			// Get reference to lua function
			luabridge::LuaRef luaFunc = luabridge::getGlobal(LuaBinding::getLua(), m_CurrentFunction.c_str());

			m_CurrentFunction = "";

			// Execute it and return casted to the type which we expect.
			return luaFunc().cast<ReturnType*>();
		}
		catch (luabridge::LuaException& e) {


			using namespace std;
			cout << color(colors::RED);
			cout << "[C++] Severe Script Error in \"" << m_FilePath << "\"::\"" << m_CurrentFunction << "\" :: " << e.what() << white << endl;


			m_CurrentFunction = "";

			return nullptr;
		}
	}




	std::string execute()
	{
		// Errors in scripts are not lethal.
		// We only return a string indicating the error and let it silently move on...
		//
		try {

			// Load script..
			int result = lua_pcall(LuaBinding::getLua(), 0, LUA_MULTRET, 0);
			if (result != LUA_OK) return nullptr;


			luabridge::LuaRef luaFunc = luabridge::getGlobal(LuaBinding::getLua(), m_CurrentFunction.c_str());

			m_CurrentFunction = "";


			return luaFunc().cast<std::string>();
		}
		catch (luabridge::LuaException& e) {


			using namespace std;
			cout << color(colors::RED);
			cout << "[C++] Severe Script Error in \"" << m_FilePath << "\"::\"" << m_CurrentFunction << "\" :: " << e.what() << white << endl;

			m_CurrentFunction = "";

			return "LUA_ERROR";
		}
	}




private:

	std::string m_CurrentFunction;
	std::string m_FilePath;
};

#endif