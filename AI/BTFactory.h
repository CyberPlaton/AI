#pragma once

#include "BTNode.h"
#include "BehaviorTree.h"
#include "LuaBinding.h"
#include "Script.h"


class BTFactory
{
public:
	BTFactory(const std::string& tree_name) : m_Tree(new BehaviorTree(tree_name)){}

	BehaviorTree* readFromFile(std::string path)
	{
		/*
		std::ifstream in_config(path);
		if (!in_config.is_open()) return false;
		*/

		Script script(path);
		script.pushFunction("main");

		return script.execute<BehaviorTree>();

	}

	/*
	template < typename NodeType, class...Args >
	NodeType* add(const std::string& node_type_name, const std::string& node_name, Args... args)
	{
		using namespace std;

		cout << "Creating \"" << node_type_name << "\"" << endl;

		NodeType* node = new NodeType(node_name, args...);


		return node;
	}
	*/


	template < typename NodeType, class... Args >
	BTFactory& add(const std::string& node_name, Args... args)
	{
		using namespace std;

		NodeType* node = new NodeType(node_name, args...);

		if (has_root == false)
		{
			m_Tree->setRoot(node);
			has_root = true;
			last_parent = node;
		}
		else
		{
			last_parent->addChild(node);
		}

		return *this;
	}



	BTFactory& end()
	{
		if (last_parent->parent())
		{
			last_parent = last_parent->parent();
		}

		return *this;
	}


	BehaviorTree* build()
	{
		return m_Tree;
	}


private:
	
	BTNode* last_parent = nullptr;
	BehaviorTree* m_Tree = nullptr;

	bool has_root = false;
};