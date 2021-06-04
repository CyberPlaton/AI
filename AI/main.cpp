
#include <conio.h>
#include "AIEngine.h"
#include "Timer.h"
#include <random>


/*
* Example impl. of an action which returns running if not success..
*/
class BTWalkToDestination : public BTAction
{
public:
	BTWalkToDestination(std::string name, BTBlackboard* b) : BTAction(name), m_Blackboard(b) { }


	BTNodeResult command() override final
	{

#ifdef _DEBUG_OUT

		using namespace std;
		cout << color(colors::MAGENTA);
		cout << name() << "->command()" << white << endl;
#endif

		int dest[3] = { 0, 0, 0 };
		int location[3] = { 0, 0, 0 };


		dest[0] = m_Blackboard->getData<int>("destination_x");
		dest[1] = m_Blackboard->getData<int>("destination_y");
		dest[2] = m_Blackboard->getData<int>("destination_z");

		location[0] = m_Blackboard->getData<int>("location_x");
		location[1] = m_Blackboard->getData<int>("location_y");
		location[2] = m_Blackboard->getData<int>("location_z");

		int move[3] = { 0, 0, 0 };
		if (dest[0] < location[0])
		{
			move[0] = -1;
		}
		else if (dest[0] > location[0])
		{
			move[0] = 1;
		}

		if (dest[1] < location[1])
		{
			move[1] = -1;
		}
		else if (dest[1] > location[1])
		{
			move[1] = 1;
		}

		if (dest[2] < location[2])
		{
			move[2] = -1;
		}
		else if (dest[2] > location[2])
		{
			move[2] = 1;
		}


		m_Blackboard->set("location_x", Any(location[0] + move[0], AnyType::ANY_INT));
		m_Blackboard->set("location_y", Any(location[1] + move[1], AnyType::ANY_INT));
		m_Blackboard->set("location_z", Any(location[2] + move[2], AnyType::ANY_INT));

		if (move[0] != 0 ||
			move[1] != 0 ||
			move[2] != 0)
		{
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->command() = RUNNING" << white << endl;

			return BTNodeResult::RUNNING;
		}
		else
		{
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

			return BTNodeResult::SUCCESS;
		}
	}

	BTBlackboard* m_Blackboard = nullptr;
};



class BTSetDestination : public BTAction
{
public:
	BTSetDestination(std::string name, BTBlackboard* b) : BTAction(name), m_Blackboard(b) { }


	BTNodeResult command() override final
	{

#ifdef _DEBUG_OUT

		using namespace std;
		cout << color(colors::MAGENTA);
		cout << name() << "->command()" << white << endl;
#endif

		int location[3] = { 0, 0, 0 };
		location[0] = rand() % 20;
		location[1] = rand() % 20;
		location[2] = rand() % 20;

		m_Blackboard->set("destination_x", Any(location[0], AnyType::ANY_INT));
		m_Blackboard->set("destination_y", Any(location[1], AnyType::ANY_INT));
		m_Blackboard->set("destination_z", Any(location[2], AnyType::ANY_INT));


		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;


		return BTNodeResult::SUCCESS;
	}

	BTBlackboard* m_Blackboard = nullptr;
};





class BTMemorize : public BTAction
{
public:
	BTMemorize(std::string name, BTBlackboard* b) : BTAction(name), m_Blackboard(b) { m_Timer = new Timer(); m_Timer->startTimer(); }


	BTNodeResult command() override final
	{

#ifdef _DEBUG_OUT

		using namespace std;
		cout << color(colors::MAGENTA);
		cout << name() << "->command()" << white << endl;
#endif



		return BTNodeResult::SUCCESS;
	}

	Timer* m_Timer = nullptr;
	BTBlackboard* m_Blackboard = nullptr;
};


class BTComplexNode : public BTAction
{
public:
	BTComplexNode(std::string name, BTBlackboard* b, BTNode* p, double d, Any other) : BTAction(name) { }


	BTNodeResult command() override final
	{

#ifdef _DEBUG_OUT

		using namespace std;
		cout << color(colors::MAGENTA);
		cout << name() << "->command()" << white << endl;
#endif
		return BTNodeResult::SUCCESS;
	}
};


#include "BTFactory.h"


int main()
{

	using namespace std;
	AIEngine aiEngine;
	Timer timer;

	/*
	* Start up lua and its binding LuaBridge.
	*/
	LuaBinding::get()->getLua();


	/*
	* How to build a tree with the factory.
	*/
	BTFactory factory("Test Tree");
	BehaviorTree* new_tree = factory.add<BTSequence>("Root Sequence")
										.add<BTTimer>("Timer", (int)BTTimer::Granularity::Milliseconds, (int)BTTimer::Policy::Greater, 100.0)
										.end()
									.add<BTMemorize>("Memorizer", new BTBlackboard("Memory"))
								.build();

	new_tree->update();

	/*
	* Building a tree at runtimer could be done through exposing the BTFactory functions to lua
	* and writing a script, which in turn defines the Tree in itself.
	* 
	* Reading the script would be just like the execution of above factory functions,
	* resulting in a working tree.
	* 
	* 
	* Reading Tree from file...
	*/
	BehaviorTree* new_lua_tree = factory.readFromFile("lua_tree.lua");


	aiEngine.addBT(new_lua_tree);
	aiEngine.addBT(new_tree);


	timer.startTimer();
	while (true)
	{
		timer.startTimer();
		
		aiEngine.update();

		cout << endl;
		cout << endl;
		cout << "Update took: " << timer.getElapsedMilliseconds() << "ms" << endl;

		if (_kbhit()) break;
		std::this_thread::sleep_for(16ms);
		system("cls");
	}

	return 0;
}