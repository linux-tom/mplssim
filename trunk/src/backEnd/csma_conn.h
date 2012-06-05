#ifndef CREATECSMA_H
#define CREATECSMA_H

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include "ns3/csma-module.h"
#include <vector>
#include "genericNode.h"
#include "mpls_node.h"
#include "client_node.h"
#include "server_node.h"
#include "ip_router.h"


using namespace ns3;

class createCsmaConn
{
public:
	CsmaHelper csmaConn;
	NetDeviceContainer devices;
	Ipv4AddressHelper address;
	createCsmaConn ( std::vector <genericNode*> csmaNodes, Ipv4Address base,std::string linkBw, std::string linkDelay, std::string simID ); 
private:

};
#endif
