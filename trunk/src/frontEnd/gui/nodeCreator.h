#ifndef NODECREATOR_H
#define NODECREATOR_H
#include <iostream>

class NodeCreator
{
	public:
		NodeCreator(std::string name, std::string command, unsigned int index);
		~NodeCreator();
		unsigned int getIndex();
		std::string getCommand();
		std::string getNodeName();
	private:		
		unsigned int index;
		std::string nodeName;
		std::string command;
};
#endif
