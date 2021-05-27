#pragma once

#include "BTNode.h"

class BTFallback : public BTNode
{
public:

	BTFallback(std::string name) : m_Name(name)
	{

	}

	BTNodeResult tick() override
	{
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
			if (strcmp(name.c_str(), kid.first.c_str()) == 0)
			{
				return kid.second;
			}
		}

		return nullptr;
	}

	std::map<std::string, BTNode*>  children() override
	{
		return m_Children;
	}


	void addChild(BTNode* node) override
	{
		m_Children.try_emplace(node->name(), node);

		node->setParent(this);
	}


	void removeChild(std::string name) override
	{
		for (auto& kid : m_Children)
		{
			if (strcmp(kid.first.c_str(), name.c_str()) == 0)
			{
				m_Children.erase(name);
				return;
			}
		}
	}


	std::string name() override
	{
		return m_Name;
	}


private:

	BTNode* m_Parent = nullptr;
	std::map<std::string, BTNode*> m_Children;


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
			if (strcmp(name.c_str(), kid.first.c_str()) == 0)
			{
				return kid.second;
			}
		}

		return nullptr;
	}


	std::map<std::string, BTNode*>  children() override
	{
		return m_Children;
	}


	void addChild(BTNode* node) override
	{
		m_Children.try_emplace(node->name(), node);

		node->setParent(this);
	}


	void removeChild(std::string name) override
	{
		for (auto& kid : m_Children)
		{
			if (strcmp(kid.first.c_str(), name.c_str()) == 0)
			{
				m_Children.erase(name);
				return;
			}
		}
	}

	std::string name() override
	{
		return m_Name;
	}


private:


	BTNode* m_Parent = nullptr;
	std::map<std::string, BTNode*>  m_Children;


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


	std::map<std::string, BTNode*> children() override
	{
		std::map<std::string, BTNode*> v;
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


	std::map<std::string, BTNode*>  children() override
	{
		std::map<std::string, BTNode*> v;
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


	std::map<std::string, BTNode*>  children() override
	{
		std::map<std::string, BTNode*> v;
		if(m_Child)
		{
			v.emplace(m_Child->name(), m_Child);
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