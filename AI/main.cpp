#include "ColorConsole.h"
#include "BTNodes.h"
#include "BTBlackboard.h"
#include "Any.h"

#include <random>
#include <thread>
#include <conio.h>


int agent_location[3] = { -200, 221, -1200 };

class BTMovementExpert : public BTExpert
{
public:
	BTMovementExpert(std::string name) : m_Name(name) {}


	float insistence(BTBlackboard* bb)
	{
		using namespace std;

		cout << color(colors::RED);
		cout << "\""<< name() << "\"->insistence() = " << 1.0f << white << endl;


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







int main()
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

	/*
	std::vector<BTExpert*> experts;
	experts.push_back(expert);
	*/
	m_BBs.push_back(blackboard);
	//m_Experts.push_back(expert);




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
		/*
		* Simulate agent movement in random location.
		*/
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
			node->tick();
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

			//bb->iterate(m_Experts);
		}


		cout << color(colors::CYAN);
		cout << "Agent X " << agent_location[0] << endl;
		cout << "Agent Y " << agent_location[1] << endl;
		cout << "Agent Z " << agent_location[1] << white << endl;

		std::this_thread::sleep_for(16ms);
		system("cls");
	}

	return 0;
}