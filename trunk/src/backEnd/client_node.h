#ifndef CLIENTNODE_H
#define CLIENTNODE_H


#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/applications-module.h>
#include <ns3/mpls-module.h>
#include "genericNode.h"

using namespace ns3;

class clientNode : public genericNode
{

public:
	clientNode(std::string name, std::string type);
	void setClientApp(Ipv4Address serverIP);
	void addUDPApp(Ipv4Address serverIP, uint16_t port, uint16_t startTime, uint16_t stopTime, uint16_t packNum,  uint16_t packSize, float interval);

	void addOnOffApp(Ipv4Address serverIP, uint16_t port, uint16_t startTime, uint16_t stopTime );
	void addBurstyApp(Ipv4Address sinkIP, uint16_t sinkPort, uint16_t startTime,uint16_t stopTime, uint16_t packetSize, uint16_t packetNum );
	~clientNode();
	NodeContainer node;
	InternetStackHelper IPstack;
	ApplicationContainer App;
	//UdpEchoClientHelper *clientApp;
	void restartApp();
	bool serverSet;

private:
	//uint16_t port;
	//Ipv4Address serverIP;	

};
#endif
