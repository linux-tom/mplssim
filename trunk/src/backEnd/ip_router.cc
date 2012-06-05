#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include "ip_router.h"

using namespace ns3;

IProuter::IProuter(std::string name, std::string type) : genericNode(name,type)
{

	node.Create(1);
	IPstack.Install(node);

}
IProuter::~IProuter()
{
}
