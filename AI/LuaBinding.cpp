#include"LuaBinding.h"

LuaBinding* LuaBinding::g_pLuaBinding = nullptr;
lua_State* LuaBinding::g_pLua = nullptr;

void LuaBinding::_initLuaBinding()
{
	using namespace luabridge;

	/*
	* Fallback node exposing to Lua.
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.beginClass<BTNode>("BTNode")
		.endClass()
		.deriveClass<BTFallback, BTNode>("BTFallback")
		.addConstructor<void(*)(std::string)>()
		.addProperty<BTNode*>("m_Parent", &BTFallback::m_Parent)
		.addProperty<std::map<int, BTNode*>>("m_Children", &BTFallback::m_Children)
		.addProperty<int>("m_NextNodeIndex", &BTFallback::m_NextNodeIndex)
		.addProperty<std::string>("m_Name", &BTFallback::m_Name)
		.addFunction("addChild", &BTFallback::addChild)
		.endClass()
		.endNamespace();


	/*
	* Sequence node...
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.deriveClass<BTSequence, BTNode>("BTSequence")
		.addConstructor<void(*)(std::string)>()
		.addProperty<BTNode*>("m_Parent", &BTSequence::m_Parent)
		.addProperty<std::map<int, BTNode*>>("m_Children", &BTSequence::m_Children)
		.addProperty<int>("m_NextNodeIndex", &BTSequence::m_NextNodeIndex)
		.addProperty<std::string>("m_Name", &BTSequence::m_Name)
		.addFunction("addChild", &BTSequence::addChild)
		.endClass()
		.endNamespace();


	/*
	* Random Sequence node...
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.deriveClass<BTRandomSequence, BTNode>("BTRandomSequence")
		.addConstructor<void(*)(std::string)>()
		.addProperty<BTNode*>("m_Parent", &BTRandomSequence::m_Parent)
		.addProperty<std::map<int, BTNode*>>("m_Children", &BTRandomSequence::m_Children)
		.addProperty<int>("m_NextNodeIndex", &BTRandomSequence::m_NextNodeIndex)
		.addProperty<std::string>("m_Name", &BTRandomSequence::m_Name)
		.addFunction("addChild", &BTRandomSequence::addChild)
		.endClass()
		.endNamespace();



	/*
	* Action node...
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.deriveClass<BTAction, BTNode>("BTAction")
		.addConstructor<void(*)(std::string)>()
		.addProperty<BTNode*>("m_Parent", &BTAction::m_Parent)
		.addProperty<std::string>("m_Name", &BTAction::m_Name)
		.endClass()
		.endNamespace();



	/*
	* Condition node...
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.deriveClass<BTCondition, BTNode>("BTCondition")
		.addConstructor<void(*)(std::string)>()
		.addProperty<BTNode*>("m_Parent", &BTCondition::m_Parent)
		.addProperty<std::string>("m_Name", &BTCondition::m_Name)
		.endClass()
		.endNamespace();



	/*
	* Decorator node...
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.deriveClass<BTDecorator, BTNode>("BTDecorator")
		.addConstructor<void(*)(std::string)>()
		.addFunction("addChild", &BTDecorator::addChild)
		.addProperty<BTNode*>("m_Parent", &BTDecorator::m_Parent)
		.addProperty<BTNode*>("m_Child", &BTDecorator::m_Child)
		.addProperty<std::string>("m_Name", &BTDecorator::m_Name)
		.endClass()
		.endNamespace();



	/*
	* Inverter node...
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.deriveClass<BTInverter, BTDecorator>("BTInverter")
		.addConstructor<void(*)(std::string)>()
		.addFunction("addChild", &BTInverter::addChild)
		.addProperty<BTNode*>("m_Parent", &BTDecorator::m_Parent)
		.addProperty<BTNode*>("m_Child", &BTDecorator::m_Child)
		.addProperty<std::string>("m_Name", &BTDecorator::m_Name)
		.endClass()
		.endNamespace();


	/*
	* Parallel node...
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.deriveClass<BTParallel, BTNode>("BTParallel")
		.addConstructor<void(*)(std::string, BTParallel::Policy, BTParallel::Policy)>()
		.addFunction("addChild", &BTParallel::addChild)
		.addProperty<BTParallel::Policy>("m_FailPolicy", &BTParallel::m_FailPolicy)
		.addProperty<BTParallel::Policy>("m_SuccessPolicy", &BTParallel::m_SuccessPolicy)
		.addProperty<std::string>("m_Name", &BTParallel::m_Name)
		.addProperty<BTNode*>("m_Parent", &BTParallel::m_Parent)
		.addProperty<std::map<int, BTNode*>>("m_Children", &BTParallel::m_Children)
		.addProperty<std::string>("m_Name", &BTParallel::m_Name)
		.addProperty<int>("m_NextNodeIndex", &BTParallel::m_NextNodeIndex)
		.endClass()
		.endNamespace();





	/*
	* Timer node...
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.deriveClass<BTTimer, BTCondition>("BTTimer")
		.addConstructor<void(*)(std::string, int, int, double)>()
		.addProperty<Timer>("m_Timer", &BTTimer::m_Timer)
		.addProperty<BTTimer::Policy>("m_Policy", &BTTimer::m_Policy)
		.addProperty<BTTimer::Granularity>("m_Granularity", &BTTimer::m_Granularity)
		.addProperty<double>("m_Condition", &BTTimer::m_Condition)
		.addProperty<BTNode*>("m_Parent", &BTTimer::m_Parent)
		.addProperty<std::string>("m_Name", &BTTimer::m_Name)
		.endClass()
		.endNamespace();



	/*
	* Expose the behavior tree, which is needed to be constructed with a root.
	*/
	getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("ai")
		.beginClass<BehaviorTree>("BehaviorTree")
		.addConstructor<void(*)(std::string)>()
		.addProperty<std::string>("m_Name", &BehaviorTree::m_Name)
		.addProperty<BTNode*>("m_Root", &BehaviorTree::m_Root)
		.addFunction("setRoot", &BehaviorTree::setRoot)
		.endClass()
		.endNamespace();

	/*
	luabridge::getGlobalNamespace(LuaBinding::g_pLua)
		.beginNamespace("Engine")
		.beginClass<CEntity>("CEntity")
		.addProperty<entt::entity>("entityHandle", &CEntity::m_EntityHandle)
		.addProperty<CScene*>("scene", &CEntity::m_Scene)
		.endClass()
		.deriveClass<CSprite, CEntity>("CSprite")
		.addFunction("setPosition", &CSprite::setPosition)
		.addFunction("setScale", &CSprite::setScale)
		.addFunction("setColor", &CSprite::setColor)
		.addFunction("setRotation", &CSprite::setRotation)
		.addFunction("setBrightness", &CSprite::setBrightness)
		.addFunction("incrementPosition", &CSprite::incrementPosition)
		.addFunction("incrementScale", &CSprite::incrementScale)
		.addFunction("incrementColor", &CSprite::incrementColor)
		.addFunction("incrementRotation", &CSprite::incrementRotation)
		.addFunction("decrementPosition", &CSprite::decrementPosition)
		.addFunction("decrementScale", &CSprite::decrementScale)
		.addFunction("decrementColor", &CSprite::decrementColor)
		.addFunction("decrementRotation", &CSprite::decrementRotation)
		.addFunction("getScale", &CSprite::getScale)
		.addFunction("getColor", &CSprite::getColor)
		.addFunction("getRotation", &CSprite::getRotation)
		.addFunction("getBrightness", &CSprite::getBrightness)
		.addFunction("getPosition", &CSprite::getPosition)
		.endClass()
		.endNamespace();
		*/
}
