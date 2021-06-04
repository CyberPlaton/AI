#pragma once

#include "lua.hpp"
#pragma comment(lib, "lua54.lib")
#include "LuaBridge.h"

#include "ColorConsole.h"
#include <tuple>
#include <mutex>


#include "BTNodes.h"
#include "BehaviorTree.h"


class Script;

class LuaBinding {
	friend class Script;

public:
	static lua_State* getLua() {

		if (!g_pLua) {

			g_pLua = luaL_newstate();

			luaL_openlibs(g_pLua);

			g_pLuaBinding->_initLuaBinding();
		}

		if (!g_pLuaBinding) LuaBinding::get();

		return g_pLua;
	}

	static LuaBinding* get() {

		if (!g_pLuaBinding) {

			g_pLuaBinding = new LuaBinding();
		}

		return g_pLuaBinding;
	}

	static void del() {

		if (g_pLua) {

			lua_close(g_pLua);
		}

		// Lua
		g_pLua = nullptr;

		// Self
		delete g_pLuaBinding;
		g_pLuaBinding = nullptr;
	}


	// If the last executed script had an error, we will return it.
	//
	static std::string getLuaError()
	{
		// Thread safe lua binding.
		std::scoped_lock lock(m_Mutex);

		return(lua_tostring(g_pLuaBinding->g_pLua, -1));
	}



	// Checks for the return value of "lua_dofile" or "lua_dostring" and
	// if there was an error it prints that error and returns false.	
	//
	// Example: "LuaBinding::check(	 luaL_dofile(LuaBinding::getLua(), "myScript.lua")	);"
	//
	static std::tuple<bool, std::string> check(int return_value) {

		using namespace std;

		// Thread safe lua binding.
		std::scoped_lock lock(m_Mutex);


		if (return_value) {

			std::string error = LuaBinding::getLuaError();


			cout << color(colors::RED);
			cout << "[C++] Lua Error: " << error << white << endl;

			return std::make_tuple(false, error);
		}

		return std::make_tuple(true, "OK");
	}

private:
	static LuaBinding* g_pLuaBinding;
	static lua_State* g_pLua;


	static std::mutex m_Mutex;

private:

	void _initLuaBinding();

};