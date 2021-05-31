
#include <conio.h>
#include "AIEngine.h"

int main()
{
	using namespace std;
	AIEngine aiEngine;



	for (int i = 0; i < 10000000; i++)
	{
		BehaviorTree* tree = new BehaviorTree("Behavior Tree");

		BTSequence* seq = new BTSequence("Normal Sequence");
		BTFallback* fb = new BTFallback("Fallback");
		BTFallback* fb2 = new BTFallback("Second Fallback");
		BTTimer* timer2 = new BTTimer("Timer", BTTimer::Granularity::Milliseconds, BTTimer::Policy::Smaller, 100.0);

		seq->addChild(fb);
		seq->addChild(fb2);
		fb->addChild(timer2);

		tree->setRoot(seq);
		aiEngine.addBT(tree);
	}


	while (true)
	{
		aiEngine.update();

		if (_kbhit()) break;
		std::this_thread::sleep_for(16ms);
		system("cls");
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