#include "ColorConsole.h"
#include "BTNodes.h"
#include "BTBlackboard.h"
#include "Any.h"

#include "App.h"

#include <random>
#include <thread>
#include <conio.h>


class BTActionGetLocation : public BTAction
{
public:
	BTActionGetLocation(std::string name, BTBlackboard* blackboard) : BTAction(name), m_Blackboard(blackboard) {}

	BTNodeResult command() override final
	{
		using namespace std;

		int x = rand() % 100;
		int y = rand() % 100;
		int z = rand() % 100;



		int xx = m_Blackboard->data().at("location_x").as<int>();
		int yy = m_Blackboard->data().at("location_y").as<int>();
		int zz = m_Blackboard->data().at("location_z").as<int>();


		if (rand() % 2 == 0)
		{
			Any anyx(x + xx, AnyType::ANY_INT);
			Any anyy(y + yy, AnyType::ANY_INT);
			Any anyz(z + zz, AnyType::ANY_INT);


			m_Blackboard->set("location_x", anyx);
			m_Blackboard->set("location_y", anyy);
			m_Blackboard->set("location_z", anyz);

		}
		else
		{

			Any anyx(-x + xx, AnyType::ANY_INT);
			Any anyy(-y + yy, AnyType::ANY_INT);
			Any anyz(-z + zz, AnyType::ANY_INT);


			m_Blackboard->set("location_x", anyx);
			m_Blackboard->set("location_y", anyy);
			m_Blackboard->set("location_z", anyz);
		}

		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS" << white << endl;

		return BTNodeResult::SUCCESS;
	}


	BTBlackboard* m_Blackboard = nullptr;
};




class BTConditionCheckLocation : public BTCondition
{
public:
	BTConditionCheckLocation(std::string name, BTBlackboard* blackboard) : BTCondition(name), m_Blackboard(blackboard) {}


	BTNodeResult checkCondition() override final
	{
		using namespace std;

		int x = m_Blackboard->data()["location_x"].as<int>();
		int y = m_Blackboard->data()["location_y"].as<int>();
		int z = m_Blackboard->data()["location_z"].as<int>();




		if (x < 100 && x > -100 ||
			y < 100 && y > -100 ||
			z < 100 && z > -100)
		{
			cout << color(colors::GREEN) << endl;
			cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;

			return SUCCESS;
		}

		return FAILURE;
	}

	BTBlackboard* m_Blackboard = nullptr;
};



class BTMovementExpert : public BTExpert
{
public:
	BTMovementExpert(std::string name) : m_Name(name) {}

	float insistence(BTBlackboard* bb)
	{
		return 1.0f;
	}


	void run(BTBlackboard* bb)
	{
		Any x(0, AnyType::ANY_INT);
		Any y(0, AnyType::ANY_INT);
		Any z(0, AnyType::ANY_INT);

		bb->set("location_x", x);
		bb->set("location_x", x);
		bb->set("location_x", x);
	}


	std::string name()
	{
		return m_Name;
	}


private:
	std::string m_Name;
};



bool App::OnUserCreate()
{

	BTBlackboard* blackboard = new BTBlackboard("BB");
	BTMovementExpert* expert = new BTMovementExpert("MExpert");

	Any anyx(12, AnyType::ANY_INT);
	Any anyy(91, AnyType::ANY_INT);
	Any anyz(-10, AnyType::ANY_INT);

	blackboard->set("location_x", anyx);
	blackboard->set("location_y", anyy);
	blackboard->set("location_z", anyz);

	std::vector<BTExpert*> experts;
	experts.push_back(expert);

	m_BBs.push_back(blackboard);
	m_Experts.push_back(expert);




	BTFallback* fallback = new BTFallback("F");
	BTActionGetLocation* action2 = new BTActionGetLocation("AA", blackboard);
	BTSequence* sequence = new BTSequence("S");
	BTConditionCheckLocation* condition = new BTConditionCheckLocation("C", blackboard);

	fallback->addChild(sequence);

	sequence->addChild(condition);
	sequence->addChild(action2);

	m_Nodes.push_back(fallback);

	return true;
}


bool App::OnUserUpdate(float fElapsedTime)
{
	using namespace std;


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
		cout << "X " << x << endl;
		cout << "Y " << y << endl;
		cout << "Z " << z << white << endl;


		bb->iterate(m_Experts);
	}


	std::this_thread::sleep_for(16ms);
	system("cls");

	return true;
}


int main()
{


	App app;
	if (app.Construct(600, 480, 1, 1))
	{
		app.Start();
	}

	return 0;
}