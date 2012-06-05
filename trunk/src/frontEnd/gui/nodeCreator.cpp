#include "nodeCreator.h"

NodeCreator::NodeCreator(std::string name, std::string command, unsigned int index)
{
	this->nodeName=name;
	this->command=command;
	this->index=index;
}

unsigned int NodeCreator::getIndex()
{
	return this->index;
}

std::string NodeCreator::getNodeName()
{
	return this->nodeName;
}

std::string NodeCreator::getCommand()
{
	return this->command;
}

NodeCreator::~NodeCreator()
{

}
