#ifndef IPROUTER_H
#define IPROUTER_H

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include "genericNode.h"

using namespace ns3;

class IProuter : public genericNode
{
public:
	NodeContainer node;
	InternetStackHelper IPstack;
	IProuter(std::string name, std::string type);
	~IProuter();

private:

};

#endif
