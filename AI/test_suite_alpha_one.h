#pragma once
#include "BehaviorTree.h"


#include <random>
#include <thread>
#include <chrono>



int agent_location[3] = { -200, 221, -1200 };

class BTMovementExpert : public BTExpert
{
public:
	BTMovementExpert(std::string name) : m_Name(name) {}


	float insistence(BTBlackboard* bb)
	{
		using namespace std;

		cout << color(colors::RED);
		cout << "\"" << name() << "\"->insistence() = " << 1.0f << white << endl;


		return 1.0f;
	}


	void run(BTBlackboard* bb)
	{
		Any x(0, AnyType::ANY_INT);
		Any y(0, AnyType::ANY_INT);
		Any z(0, AnyType::ANY_INT);

		bb->set("location_x", x);
		bb->set("location_y", y);
		bb->set("location_z", z);
	}


	std::string name()
	{
		return m_Name;
	}


private:
	std::string m_Name;
};




class BTLocationBetweeZeroAndHundred : public BTCondition
{
public:
	BTLocationBetweeZeroAndHundred(std::string name, BTBlackboard* blackboard) : BTCondition(name), m_Blackboard(blackboard) {}


	BTNodeResult checkCondition() override final
	{
		using namespace std;


		int x = m_Blackboard->data()["location_x"].as<int>();
		int y = m_Blackboard->data()["location_y"].as<int>();
		int z = m_Blackboard->data()["location_z"].as<int>();




		if (x < 100 && x > 0 &&
			y < 100 && y > 0 &&
			z < 100 && z > 0)
		{
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;

			return SUCCESS;
		}

		cout << color(colors::RED) << endl;
		cout << "\"" << name() << "\"->checkCondition() = FAILURE" << white << endl;

		return FAILURE;
	}

	BTBlackboard* m_Blackboard = nullptr;

};


class BTGetLocation : public BTAction
{
public:
	BTGetLocation(std::string name, BTBlackboard* blackboard) : BTAction(name), m_Blackboard(blackboard) {}

	BTNodeResult command() override final
	{
		using namespace std;

		m_Blackboard->set("location_x", Any(agent_location[0], ANY_INT));
		m_Blackboard->set("location_y", Any(agent_location[1], ANY_INT));
		m_Blackboard->set("location_z", Any(agent_location[2], ANY_INT));


		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

		return BTNodeResult::SUCCESS;
	}


	BTBlackboard* m_Blackboard = nullptr;

};


class BTHigherThanHundred : public BTCondition
{
public:
	BTHigherThanHundred(std::string name, BTBlackboard* blackboard) : BTCondition(name), m_Blackboard(blackboard) {}


	BTNodeResult checkCondition() override final
	{
		using namespace std;

		int x = m_Blackboard->data()["location_x"].as<int>();
		int y = m_Blackboard->data()["location_y"].as<int>();
		int z = m_Blackboard->data()["location_z"].as<int>();




		if (x > 100 ||
			y > 100 ||
			z > 100)
		{
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;

			return SUCCESS;
		}

		cout << color(colors::RED) << endl;
		cout << "\"" << name() << "\"->checkCondition() = FAILURE" << white << endl;

		return FAILURE;
	}

	BTBlackboard* m_Blackboard = nullptr;
};




class BTSmallerThanZero : public BTCondition
{
public:
	BTSmallerThanZero(std::string name, BTBlackboard* blackboard) : BTCondition(name), m_Blackboard(blackboard) {}


	BTNodeResult checkCondition() override final
	{
		using namespace std;

		int x = m_Blackboard->data()["location_x"].as<int>();
		int y = m_Blackboard->data()["location_y"].as<int>();
		int z = m_Blackboard->data()["location_z"].as<int>();




		if (x < 0 ||
			y < 0 ||
			z < 0)
		{
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;

			return SUCCESS;
		}

		cout << color(colors::RED) << endl;
		cout << "\"" << name() << "\"->checkCondition() = FAILURE" << white << endl;

		return FAILURE;
	}

	BTBlackboard* m_Blackboard = nullptr;
};




class BTDecrement : public BTAction
{
public:
	BTDecrement(std::string name, BTBlackboard* blackboard) : BTAction(name), m_Blackboard(blackboard) {}

	BTNodeResult command() override final
	{
		using namespace std;

		int xbefore = m_Blackboard->data()["location_x"].as<int>();
		int ybefore = m_Blackboard->data()["location_y"].as<int>();
		int zbefore = m_Blackboard->data()["location_z"].as<int>();

		agent_location[0] = xbefore - 100;
		agent_location[1] = ybefore - 100;
		agent_location[2] = zbefore - 100;

		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

		return BTNodeResult::SUCCESS;
	}


	BTBlackboard* m_Blackboard = nullptr;

};





class BTIncrement : public BTAction
{
public:
	BTIncrement(std::string name, BTBlackboard* blackboard) : BTAction(name), m_Blackboard(blackboard) {}

	BTNodeResult command() override final
	{
		using namespace std;

		int xbefore = m_Blackboard->data()["location_x"].as<int>();
		int ybefore = m_Blackboard->data()["location_y"].as<int>();
		int zbefore = m_Blackboard->data()["location_z"].as<int>();


		agent_location[0] = xbefore + 100;
		agent_location[1] = ybefore + 100;
		agent_location[2] = zbefore + 100;


		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

		return BTNodeResult::SUCCESS;
	}


	BTBlackboard* m_Blackboard = nullptr;

};




bool test_suite_alpha_one()
{
	using namespace std;


	std::vector<BTNode*> m_Nodes;
	std::vector<BTBlackboard*> m_BBs;
	std::vector<BTExpert*> m_Experts;


	BTBlackboard* blackboard = new BTBlackboard("BB");
	BTMovementExpert* expert = new BTMovementExpert("MExpert");

	Any anyx(agent_location[0], AnyType::ANY_INT);
	Any anyy(agent_location[1], AnyType::ANY_INT);
	Any anyz(agent_location[2], AnyType::ANY_INT);

	blackboard->set("location_x", anyx);
	blackboard->set("location_y", anyy);
	blackboard->set("location_z", anyz);

	m_BBs.push_back(blackboard);


	BTFallback* root = new BTFallback("Root");
	BTSequence* seq1 = new BTSequence("Seq1");
	BTFallback* f1 = new BTFallback("F1");
	BTSequence* seq2 = new BTSequence("Seq2");
	BTSequence* seq3 = new BTSequence("Seq3");


	BTLocationBetweeZeroAndHundred* locationTest = new BTLocationBetweeZeroAndHundred("LocationTest", blackboard);
	BTGetLocation* getLocation = new BTGetLocation("GetLocation", blackboard);

	BTHigherThanHundred* cond1 = new BTHigherThanHundred("HigherThanHundred", blackboard);
	BTSmallerThanZero* cond2 = new BTSmallerThanZero("SmallerThanZero", blackboard);

	BTDecrement* dec = new BTDecrement("Decrement", blackboard);
	BTIncrement* inc = new BTIncrement("Increment", blackboard);

	root->addChild(locationTest);
	root->addChild(seq1);

	seq1->addChild(getLocation);
	seq1->addChild(f1);

	f1->addChild(seq2);
	f1->addChild(seq3);

	seq2->addChild(cond1);
	seq2->addChild(dec);

	seq3->addChild(cond2);
	seq3->addChild(inc);

	m_Nodes.push_back(root);


	while (true)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		int z = rand() % 100;

		if (rand() % 2 == 0)
		{
			agent_location[0] += x;
			agent_location[1] += y;
			agent_location[2] += z;
		}
		else
		{
			agent_location[0] -= x;
			agent_location[1] -= y;
			agent_location[2] -= z;
		}



		for (auto& node : m_Nodes)
		{
			if (node->tick() == BTNodeResult::SUCCESS)
				return true;
		}


		for (auto& bb : m_BBs)
		{

			int x = bb->data()["location_x"].as<int>();
			int y = bb->data()["location_y"].as<int>();
			int z = bb->data()["location_z"].as<int>();

			cout << color(colors::YELLOW);
			cout << "Blackboard X " << x << endl;
			cout << "Blackboard Y " << y << endl;
			cout << "Blackboard Z " << z << white << endl;
		}


		cout << color(colors::CYAN);
		cout << "Agent X " << agent_location[0] << endl;
		cout << "Agent Y " << agent_location[1] << endl;
		cout << "Agent Z " << agent_location[1] << white << endl;

		std::this_thread::sleep_for(16ms);
		system("cls");
	}

	return false;
}














class InsideBox : public BTCondition
{
public:
	InsideBox(std::string name, BTBlackboard* bb) : BTCondition(name), m_Blackboard(bb) {}


	BTNodeResult checkCondition() override final
	{
		using namespace std;

		int x = m_Blackboard->data()["location_x"].as<int>();
		int y = m_Blackboard->data()["location_y"].as<int>();
		int z = m_Blackboard->data()["location_z"].as<int>();


		if ((x >= 0 && x <= 10) &&
			(y >= 0 && y <= 10) &&
			(z >= 0 && z <= 10))
		{
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;

			return SUCCESS;
		}

		cout << color(colors::RED) << endl;
		cout << "\"" << name() << "\"->checkCondition() = FAILURE" << white << endl;

		return FAILURE;
	}





	BTBlackboard* m_Blackboard = nullptr;

};


class GetCurrentLocation : public BTAction
{
public:

	GetCurrentLocation(std::string name, BTBlackboard* bb) : BTAction(name), m_Blackboard(bb) {}



	BTNodeResult command() override final
	{
		using namespace std;

		m_Blackboard->set("location_x", Any(agent_location[0], ANY_INT));
		m_Blackboard->set("location_y", Any(agent_location[1], ANY_INT));
		m_Blackboard->set("location_z", Any(agent_location[2], ANY_INT));

		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

		return BTNodeResult::SUCCESS;
	}



	BTBlackboard* m_Blackboard = nullptr;
};


class AdjustX : public BTAction
{
public:
	AdjustX(std::string name, BTBlackboard* bb) : BTAction(name), m_Blackboard(bb) {}


	BTNodeResult command() override final
	{
		using namespace std;

		int x = m_Blackboard->data()["location_x"].as<int>();

		if (x > 0)
		{
			agent_location[0] = x - 1;
		}
		else
		{
			agent_location[0] = x + 1;
		}


		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

		return BTNodeResult::SUCCESS;
	}



	BTBlackboard* m_Blackboard = nullptr;

};



class AdjustY : public BTAction
{
public:
	AdjustY(std::string name, BTBlackboard* bb) : BTAction(name), m_Blackboard(bb) {}


	BTNodeResult command() override final
	{
		using namespace std;

		int y = m_Blackboard->data()["location_y"].as<int>();

		if (y > 0)
		{
			agent_location[1] = y - 1;
		}
		else
		{
			agent_location[1] = y + 1;
		}


		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

		return BTNodeResult::SUCCESS;
	}

	BTBlackboard* m_Blackboard = nullptr;
};




class AdjustZ : public BTAction
{
public:
	AdjustZ(std::string name, BTBlackboard* bb) : BTAction(name), m_Blackboard(bb) {}


	BTNodeResult command() override final
	{
		using namespace std;

		int z = m_Blackboard->data()["location_z"].as<int>();

		if (z > 0)
		{
			agent_location[2] = z - 1;
		}
		else
		{
			agent_location[2] = z + 1;
		}


		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

		return BTNodeResult::SUCCESS;
	}


	BTBlackboard* m_Blackboard = nullptr;
};



bool test_suite_alpha_two()
{
	using namespace std;

	BTBlackboard* bb = new BTBlackboard("Blackboard");
	BehaviorTree* tree = new BehaviorTree("Behavior Tree");


	BTFallback* fallback = new BTFallback("Fallback");
	BTSequence* sequence = new BTSequence("Sequence");
	BTParallel* parallel = new BTParallel("Parallel", BTParallel::Require_All, BTParallel::Require_One);


	// We define a Box for x, y, z as between 0 and 10.
	InsideBox* cond = new InsideBox("InsideBox", bb);

	GetCurrentLocation* loc = new GetCurrentLocation("GetLocation", bb);
	AdjustX* adjustX = new AdjustX("Adjust X", bb);
	AdjustY* adjustY = new AdjustY("Adjust Y", bb);
	AdjustZ* adjustZ = new AdjustZ("Adjust Z", bb);


	// Create tree structure.
	fallback->addChild(cond);
	fallback->addChild(sequence);

	sequence->addChild(loc);
	sequence->addChild(parallel);

	parallel->addChild(adjustX);
	parallel->addChild(adjustY);
	parallel->addChild(adjustZ);



	// Set initial data.
	tree->setRoot(fallback);
	bb->set("location_x", Any(agent_location[0], ANY_INT));
	bb->set("location_y", Any(agent_location[1], ANY_INT));
	bb->set("location_z", Any(agent_location[2], ANY_INT));


	while (true)
	{
		cout << color(colors::YELLOW);
		cout << "X " << bb->data()["location_x"].as<int>() << endl;
		cout << "Y " << bb->data()["location_y"].as<int>() << endl;
		cout << "Z " << bb->data()["location_z"].as<int>() << white << endl;


		if (tree->update() == BTNodeResult::SUCCESS)
			return true;

		std::this_thread::sleep_for(16ms);
		system("cls");
	}

	return false;
}












class PrintToConsole : public BTAction
{
public:
	PrintToConsole(std::string name, std::string message) : BTAction(name), m_Message(message) {}


	BTNodeResult command() override final
	{
		using namespace std;

		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS -- \"" << m_Message << "\"" << white << endl;

		return BTNodeResult::SUCCESS;
	}

	std::string m_Message;
};



bool test_suite_alpha_three()
{
	using namespace std;

	BehaviorTree* tree = new BehaviorTree("Behavior Tree");

	BTSequence* root = new BTSequence("Root Sequence");

	PrintToConsole* print = new PrintToConsole("Print Node", "Heavy Computation...");
	PrintToConsole* print2 = new PrintToConsole("Second Print Node", "Executing AI...");
	PrintToConsole* print3 = new PrintToConsole("Third Print Node", "Executing Graphics...");
	PrintToConsole* print4 = new PrintToConsole("Fourth Print Node", "Sending Inputs...");

	BTRandomSequence* seq = new BTRandomSequence("Random Sequence");
	BTTimer* timer = new BTTimer("Timer", BTTimer::Granularity::Seconds, BTTimer::Policy::Greater, 5.0);

	root->addChild(seq);

	seq->addChild(timer);
	seq->addChild(print);
	seq->addChild(print2);
	seq->addChild(print3);
	seq->addChild(print4);


	tree->setRoot(root);


	int count = 0;
	while (true)
	{
		tree->update();


		std::this_thread::sleep_for(16ms);
		system("cls");
		count++;

		if (count > 100) return true;
	}


	return false;
}