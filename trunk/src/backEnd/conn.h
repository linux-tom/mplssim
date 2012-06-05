#ifndef CREATECONN_H
#define CREATECONN_H

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include "genericNode.h"
#include "mpls_node.h"
#include "client_node.h"
#include "server_node.h"
#include "ip_router.h"

using namespace ns3;

class createConn
{
public:
	PointToPointHelper PtoPConn; // The point to point connection creator
	NetDeviceContainer devices;
	Ipv4AddressHelper address;
	Ipv4InterfaceContainer interfaces ;
	uint32_t index;
	//createConn ( NodeContainer* node1, NodeContainer* node2, Ipv4Address base ); 
	createConn ( genericNode* node1, genericNode* node2, Ipv4Address base,std::string linkBW, std::string linkDelay,uint32_t connIndex,std::string simID ); 
	//createConn ( genericNode* node1, genericNode* node2, Ipv4Address base,std::string linkBW, std::string linkDelay, std::string simID, bool fail, float failTime, bool resume, float resumeTime ); 
	uint32_t getIndex();
private:

};
#endif
