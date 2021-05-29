#pragma once


#include "BTNode.h"
#include "Timer.h"



class BTFallback : public BTNode
{
public:

	BTFallback(std::string name) : m_Name(name)
	{

	}

	BTNodeResult tick() override
	{
		using namespace std;
		cout << color(colors::MAGENTA);
		cout << name() << "->tick()" << white << endl;


		for (auto& kid : m_Children)
		{
			BTNodeResult result = kid.second->tick();

			if (result == BTNodeResult::RUNNING)
			{
				return BTNodeResult::RUNNING;
			}
			else if (result == BTNodeResult::SUCCESS)
			{
				return BTNodeResult::SUCCESS;
			}
		}

		return BTNodeResult::FAILURE;
	}


	BTNode* parent() override
	{
		return m_Parent;
	}


	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string name) override
	{
		for (auto& kid : m_Children)
		{
			if (strcmp(name.c_str(), kid.second->name().c_str()) == 0)
			{
				return kid.second;
			}
		}

		return nullptr;
	}

	std::map<int, BTNode*>  children() override
	{
		return m_Children;
	}


	void addChild(BTNode* node) override
	{
		m_Children.try_emplace(m_NextNodeIndex, node);

		m_NextNodeIndex++;

		node->setParent(this);
	}


	void removeChild(std::string name) override
	{
		int location = 0;

		for (auto& kid : m_Children)
		{
			if (strcmp(kid.second->name().c_str(), name.c_str()) == 0)
			{
				m_Children.erase(location);
				m_NextNodeIndex--;
				return;
			}

			location++;
		}
	}


	std::string name() override
	{
		return m_Name;
	}


private:

	BTNode* m_Parent = nullptr;

	/*
	* The first number indicates the positional value of the node,
	* where 0 means this node is executed first and n-1 is the last node.
	*/
	std::map<int, BTNode*> m_Children;
	int m_NextNodeIndex = 0;

	std::string m_Name;
};












class BTSequence : public BTNode
{
public:

	BTSequence(std::string name) : m_Name(name)
	{

	}

	BTNodeResult tick() override
	{
		using namespace std;
		cout << color(colors::MAGENTA);
		cout << name() << "->tick()" << white << endl;




		for (auto& kid : m_Children)
		{
			BTNodeResult result = kid.second->tick();

			if (result == BTNodeResult::RUNNING)
			{
				return BTNodeResult::RUNNING;
			}
			else if (result == BTNodeResult::FAILURE || result == BTNodeResult::INVALID)
			{
				return BTNodeResult::FAILURE;
			}
		}

		return BTNodeResult::SUCCESS;
	}


	BTNode* parent() override
	{
		return m_Parent;
	}

	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string name) override
	{
		for (auto& kid : m_Children)
		{
			if (strcmp(name.c_str(), kid.second->name().c_str()) == 0)
			{
				return kid.second;
			}
		}

		return nullptr;
	}


	std::map<int, BTNode*>  children() override
	{
		return m_Children;
	}


	void addChild(BTNode* node) override
	{
		m_Children.try_emplace(m_NextNodeIndex, node);

		m_NextNodeIndex++;

		node->setParent(this);
	}


	void removeChild(std::string name) override
	{
		int location = 0;

		for (auto& kid : m_Children)
		{
			if (strcmp(kid.second->name().c_str(), name.c_str()) == 0)
			{
				m_Children.erase(location);
				m_NextNodeIndex--;
				return;
			}

			location++;
		}
	}

	std::string name() override
	{
		return m_Name;
	}


private:


	BTNode* m_Parent = nullptr;

	/*
	* The first number indicates the positional value of the node,
	* where 0 means this node is executed first and n-1 is the last node.
	*/
	std::map<int, BTNode*> m_Children;
	int m_NextNodeIndex = 0;

	std::string m_Name;
};















class BTAction : public BTNode
{
public:

	BTAction(std::string name) : m_Name(name)
	{

	}


	virtual BTNodeResult command()
	{
		using namespace std;
		cout << color(colors::RED) << endl;
		cout << "\"" << m_Name << "\"->command() = Invalid" << white << endl;

		return INVALID;
	}


	BTNodeResult tick() override
	{
		BTNodeResult result = command();
		return result;
	}


	BTNode* parent() override
	{
		return m_Parent;
	}


	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string) override
	{
		return nullptr;
	}


	std::map<int, BTNode*> children() override
	{
		std::map<int, BTNode*> v;
		return v;
	}


	void addChild(BTNode*) override
	{
		return;
	}


	void removeChild(std::string) override
	{
		return;
	}


	std::string name() override
	{
		return m_Name;
	}


private:


	BTNode* m_Parent = nullptr;

	std::string m_Name;
};










class BTCondition : public BTNode
{
public:

	BTCondition(std::string name) : m_Name(name)
	{

	}

	virtual BTNodeResult checkCondition()
	{
		using namespace std;
		cout << color(colors::RED) << endl;
		cout << "\""<< m_Name << "\"->checkCondition() = Invalid" << white << endl;

		return INVALID;
	}

	BTNodeResult tick() override
	{
		if (checkCondition() == BTNodeResult::SUCCESS)
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
	}


	BTNode* parent() override
	{
		return m_Parent;
	}


	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string) override
	{
		return nullptr;
	}


	std::map<int, BTNode*>  children() override
	{
		std::map<int, BTNode*> v;
		return v;
	}


	void addChild(BTNode*) override
	{
		return;
	}


	void removeChild(std::string) override
	{
		return;
	}

	std::string name() override
	{
		return m_Name;
	}


private:


	BTNode* m_Parent = nullptr;
	
	std::string m_Name;
};




class BTTimer : public BTCondition
{
public:
	enum class Policy
	{
		Invalid = -1,
		Smaller = 0,
		Greater = 1
	};

	enum class Granularity
	{
		Invalid = -1,
		Seconds = 0,
		Milliseconds = 1,
		Microseconds = 2
	};
public:

	/*
	* The first timer test will be unsuccessful, as creation time and first tick time are
	* probably with a huge difference.
	* 
	* Thus the working will normalize with first several tree ticks.
	*/
	BTTimer(std::string name, Granularity g, Policy p, double condition) : BTCondition(name), m_Policy(p), m_Condition(condition), m_Granularity(g) { m_Timer.startTimer(); }


	BTNodeResult checkCondition() override final
	{
		using namespace std;

		if (m_Policy == Policy::Invalid) return BTNodeResult::INVALID;


		double dt = m_Timer.getElapsedSeconds();
		double dtms = m_Timer.getElapsedMilliseconds();
		double dtus = m_Timer.getElapsedMicroseconds();

		cout << color(colors::WHITE) << endl;
		cout << name() << " elapsedTime = "<< dt << "s,    " << dtms << "ms,    " << dtus << "us" << white << endl;


		// Do timer checks based on the required granularity of time.
		switch (m_Granularity)
		{
		case Granularity::Seconds:
			if (m_Policy == Policy::Smaller)
			{

				if (dt <= m_Condition)
				{
					m_Timer.startTimer();


					cout << color(colors::GREEN) << endl;
					cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;


					return BTNodeResult::SUCCESS;
				}
			}
			else
			{
				if (dt >= m_Condition)
				{
					m_Timer.startTimer();


					cout << color(colors::GREEN) << endl;
					cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;


					return BTNodeResult::SUCCESS;
				}
			}
			break;





		case Granularity::Milliseconds:
			if (m_Policy == Policy::Smaller)
			{

				if (dtms <= m_Condition)
				{
					m_Timer.startTimer();


					cout << color(colors::GREEN) << endl;
					cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;


					return BTNodeResult::SUCCESS;
				}
			}
			else
			{
				if (dtms >= m_Condition)
				{
					m_Timer.startTimer();


					cout << color(colors::GREEN) << endl;
					cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;


					return BTNodeResult::SUCCESS;
				}
			}
			break;




		case Granularity::Microseconds:
			if (m_Policy == Policy::Smaller)
			{

				if (dtus <= m_Condition)
				{
					m_Timer.startTimer();


					cout << color(colors::GREEN) << endl;
					cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;


					return BTNodeResult::SUCCESS;
				}
			}
			else
			{
				if (dtus >= m_Condition)
				{
					m_Timer.startTimer();


					cout << color(colors::GREEN) << endl;
					cout << "\"" << name() << "\"->checkCondition() = SUCCESS" << white << endl;


					return BTNodeResult::SUCCESS;
				}
			}
			break;
		}




		cout << color(colors::RED) << endl;
		cout << "\"" << name() << "\"->checkCondition() = FAILURE" << white << endl;

		m_Timer.startTimer();
		return BTNodeResult::FAILURE;
	}


private:

	Timer m_Timer;
	Policy m_Policy = Policy::Invalid;
	Granularity m_Granularity = Granularity::Invalid;
	double m_Condition = 0.0;
};





class BTDecorator : public BTNode
{
public:

	BTDecorator(std::string name) : m_Name(name)
	{

	}


	virtual BTNodeResult tick(){ return INVALID; }


	BTNode* parent() override
	{
		return m_Parent;
	}


	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string) override
	{
		return m_Child;
	}


	std::map<int, BTNode*>  children() override
	{
		std::map<int, BTNode*> v;
		if(m_Child)
		{
			v.emplace(0, m_Child);
		}

		return v;
	}


	void addChild(BTNode* node) override
	{
		if (!m_Child)
		{
			m_Child = node;

			node->setParent(this);
		}
	}


	void removeChild(std::string) override
	{
		if (m_Child)
		{
			delete m_Child;
			m_Child = nullptr;
		}
	}

	std::string name() override
	{
		return m_Name;
	}


private:


	BTNode* m_Parent = nullptr;
	BTNode* m_Child = nullptr;

	std::string m_Name;
};







class BTInverter : public BTDecorator
{
public:

	BTInverter(std::string name) : BTDecorator(name)
	{

	}


	BTNodeResult tick() override final
	{
		BTNodeResult result = m_Child->tick();

		if (result == BTNodeResult::SUCCESS)
		{
			return FAILURE;
		}
		else
		{
			return SUCCESS;
		}
	}


	BTNode* parent() override
	{
		return m_Parent;
	}


	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string) override
	{
		return m_Child;
	}


	std::map<int, BTNode*>  children() override
	{
		std::map<int, BTNode*> v;
		if (m_Child)
		{
			v.emplace(0, m_Child);
		}

		return v;
	}


	void addChild(BTNode* node) override
	{
		if (!m_Child)
		{
			m_Child = node;

			node->setParent(this);
		}
	}


	void removeChild(std::string) override
	{
		if (m_Child)
		{
			delete m_Child;
			m_Child = nullptr;
		}
	}

	std::string name() override
	{
		return m_Name;
	}


private:


	BTNode* m_Parent = nullptr;
	BTNode* m_Child = nullptr;

	std::string m_Name;
};







class BTParallel : public BTNode
{
public:

	enum Policy
	{
		Invalid = 0,
		Require_All = 1,
		Require_One = 2
	};


public:

	BTParallel(std::string name, Policy success, Policy fail) : m_Name(name), m_FailPolicy(fail), m_SuccessPolicy(success)
	{

	}

	BTNodeResult tick() override
	{
		using namespace std;
		cout << color(colors::MAGENTA);
		cout << name() << "->tick()" << white << endl;

		if (m_FailPolicy == Policy::Invalid) return INVALID;
		if (m_SuccessPolicy == Policy::Invalid) return INVALID;

		int successful_ticks = 0, failed_ticks = 0;


		for (auto& kid : m_Children)
		{

			BTNodeResult result = kid.second->tick();

			if (result == SUCCESS)
			{
				successful_ticks++;

				if (m_SuccessPolicy == Require_One)
				{
					return SUCCESS;
				}
			}
			else if (result == FAILURE)
			{
				failed_ticks++;

				if (m_FailPolicy == Require_One)
				{
					return SUCCESS;
				}
			}
			else
			{
				return INVALID;
			}
		}



		if (m_SuccessPolicy == Require_All &&
			successful_ticks == m_Children.size())
		{
			return SUCCESS;
		}

		if (m_FailPolicy == Require_All &&
			failed_ticks == m_Children.size())
		{
			return FAILURE;
		}


		// Should never reach.
		return INVALID;
	}


	BTNode* parent() override
	{
		return m_Parent;
	}

	void setParent(BTNode* node) override
	{
		m_Parent = node;
	}


	BTNode* child(std::string name) override
	{
		for (auto& kid : m_Children)
		{
			if (strcmp(name.c_str(), kid.second->name().c_str()) == 0)
			{
				return kid.second;
			}
		}

		return nullptr;
	}


	std::map<int, BTNode*>  children() override
	{
		return m_Children;
	}


	void addChild(BTNode* node) override
	{
		m_Children.try_emplace(m_NextNodeIndex, node);

		m_NextNodeIndex++;

		node->setParent(this);
	}


	void removeChild(std::string name) override
	{
		int location = 0;

		for (auto& kid : m_Children)
		{
			if (strcmp(kid.second->name().c_str(), name.c_str()) == 0)
			{
				m_Children.erase(location);
				m_NextNodeIndex--;
				return;
			}

			location++;
		}
	}

	std::string name() override
	{
		return m_Name;
	}


private:

	Policy m_FailPolicy = Policy::Invalid;
	Policy m_SuccessPolicy = Policy::Invalid;


	BTNode* m_Parent = nullptr;

	/*
	* The first number indicates the positional value of the node,
	* where 0 means this node is executed first and n-1 is the last node.
	*/
	std::map<int, BTNode*> m_Children;
	int m_NextNodeIndex = 0;

	std::string m_Name;
};



