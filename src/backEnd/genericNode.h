#ifndef GENERICNODE_H
#define GENERICNODE_H

#include <string>
#include <iostream>

class genericNode
{
	public:
		std::string name;
		std::string type;
		genericNode(std::string name, std::string type);
		~genericNode();
};
#endif
