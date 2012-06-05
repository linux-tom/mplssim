#ifndef SIMHANDLER_H
#define SIMHANDLER_H

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/applications-module.h>
#include <ns3/mpls-module.h>

#include <string>
#include <iostream>
#include <sstream>

#include "mpls_node.h"
#include "client_node.h"
#include "server_node.h"
#include "ip_router.h"
#include "conn.h"
#include "handler.h"

using namespace ns3;
using namespace mpls;

class simHandler
{
public:
	simHandler(std::string Id);
	~simHandler();
	std::string handleCommand(std::string command);
	std::string getID();
	bool getSimStatus();
private:
	Handler* lotr;
	std::string ID;
	bool simStatus;
};
/*
InternetStackHelper IProuter::IPstack ;
PointToPointHelper createConn::PtoPConn ; 
NetDeviceContainer createConn::devices ;
MplsNetworkConfigurator MPLSnode::network ;
InternetStackHelper serverNode::IPstack;
InternetStackHelper clientNode::IPstack;**/
#endif
