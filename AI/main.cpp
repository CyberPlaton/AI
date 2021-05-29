#include "test_suite_alpha_one.h"

#include <random>
#include <chrono>
#include <thread>


class PrintToConsole : public BTAction
{
public:
	PrintToConsole(std::string name, std::string message) : BTAction(name), m_Message(message){}


	BTNodeResult command() override final
	{
		using namespace std;

		cout << color(colors::GREEN) << endl;
		cout << "\"" << name() << "\"->command() = SUCCESS -- \"" << m_Message << "\"" << white << endl;

		return BTNodeResult::SUCCESS;
	}

	std::string m_Message;
};



int main()
{
	using namespace std;

	BehaviorTree* tree = new BehaviorTree("Behavior Tree");

	PrintToConsole* print = new PrintToConsole("Print Node", "Heavy Computation...");
	BTSequence* seq = new BTSequence("Sequence");
	BTTimer* timer = new BTTimer("Timer", BTTimer::Granularity::Milliseconds, BTTimer::Policy::Smaller, 60.0);

	seq->addChild(timer);
	seq->addChild(print);

	tree->setRoot(seq);



	while (true)
	{
		tree->update();


		std::this_thread::sleep_for(16ms);
		system("cls");
	}


	return 0;
}