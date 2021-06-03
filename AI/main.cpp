
#define _DEBUG_OUT
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


		std::string entry;

		double t = m_Timer->getElapsedMilliseconds();
		entry = "mem_" + std::to_string(t);
		

		m_Blackboard->set(entry, Any(t, AnyType::ANY_DOUBLE));

		return BTNodeResult::SUCCESS;
	}

	Timer* m_Timer = nullptr;
	BTBlackboard* m_Blackboard = nullptr;
};



int main()
{
	using namespace std;
	AIEngine aiEngine;
	Timer timer;

	BTBlackboard* bb1 = new BTBlackboard("Memory of George");

	BehaviorTree* tree = new BehaviorTree("Behavior Tree");

	BTSequence* seq = new BTSequence("Normal Sequence");
	BTFallback* fb = new BTFallback("Fallback");
	BTTimer* tim = new BTTimer("Timer", BTTimer::Granularity::Seconds, BTTimer::Policy::Greater, 5.0);
	BTMemorize* mem = new BTMemorize("Memory", bb1); // Initialy zero, will be set on each iteration with new blackboard representing another entity...

	BTWalkToDestination* walk = new BTWalkToDestination("Walking", bb1);
	BTSetDestination* setDest = new BTSetDestination("Set Destination", bb1);


	fb->addChild(seq);
	fb->addChild(walk);
	seq->addChild(tim);
	seq->addChild(setDest);
	seq->addChild(mem);


	tree->setRoot(fb);
	aiEngine.addBT(tree);

	// Define initial position of entity and no dest.
	bb1->set("location_x", Any(0, AnyType::ANY_INT));
	bb1->set("location_y", Any(0, AnyType::ANY_INT));
	bb1->set("location_z", Any(0, AnyType::ANY_INT));
	bb1->set("destination_x", Any(0, AnyType::ANY_INT));
	bb1->set("destination_y", Any(0, AnyType::ANY_INT));
	bb1->set("destination_z", Any(0, AnyType::ANY_INT));




	timer.startTimer();
	while (true)
	{
		timer.startTimer();
		
		// First entity update.
		mem->m_Blackboard = bb1;
		tree->update();


		cout << "Update took: " << timer.getElapsedMilliseconds() << "ms" << endl;

		if (_kbhit()) break;
		std::this_thread::sleep_for(16ms);
		system("cls");
	}


	cout << "Memory of George: " << endl;
	for (auto& it : bb1->data())
	{
		double n = it.second.as<double>();

		cout << color(colors::CYAN);
		cout << it.first << " : " << n << white << endl;
	}


	return 0;
}


/*
int main()
{
	using namespace vgjs;
	using namespace std;
	AIEngine aiEngine;
	JobSystem jSystem;


	BehaviorTree* tree = new BehaviorTree("Behavior Tree");
	BehaviorTree* second_tree = new BehaviorTree("Second Behavior Tree");


	BTRandomSequence* random = new BTRandomSequence("Random Sequence");
	BTParallel* parallel = new BTParallel("Parallel", BTParallel::Require_All, BTParallel::Require_One);
	BTTimer* timer = new BTTimer("Timer", BTTimer::Granularity::Seconds, BTTimer::Policy::Greater, 3.0);
	PrintToConsole* print = new PrintToConsole("Print Node", "Hello World");
	PrintToConsole* print_2 = new PrintToConsole("Print Node 2", "Half Life is deprecated");
	PrintToConsole* print_3 = new PrintToConsole("Print Node 3", "Long Live Unreal");

	BTSequence* seq = new BTSequence("Normal Sequence");
	BTFallback* fb = new BTFallback("Fallback");
	BTFallback* fb2 = new BTFallback("Second Fallback");
	BTTimer* timer2 = new BTTimer("Timer", BTTimer::Granularity::Milliseconds, BTTimer::Policy::Smaller, 100.0);
	PrintToConsole* print3 = new PrintToConsole("Some Debug Print Node", "ZZZzzzZZZzzz");
	PrintToConsole* print4 = new PrintToConsole("Fourth Print", "Four");
	PrintToConsole* print5 = new PrintToConsole("Fifth Print", "Five");


	// First tree
	random->addChild(timer);
	random->addChild(parallel);
	timer->addChild(print);
	parallel->addChild(print_2);
	parallel->addChild(print_3);

	tree->setRoot(random);

	// Second tree
	seq->addChild(fb);
	seq->addChild(fb2);
	fb->addChild(timer2);
	timer2->addChild(print3);
	fb2->addChild(print4);
	fb2->addChild(print5);

	second_tree->setRoot(seq);

	aiEngine.addBT(tree);
	aiEngine.addBT(second_tree);


	while (true)
	{
		jSystem.schedule([&]() { aiEngine.update(); });
	

		if (_kbhit()) break;
		std::this_thread::sleep_for(16ms);
		system("cls");
	}


	jSystem.terminate();
	return 0;
}
*/