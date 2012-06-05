#ifndef SERVERNODE_H
#define SERVERNODE_H

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/applications-module.h>
#include "genericNode.h"

using namespace ns3;

class serverNode : public genericNode
{

public:
	serverNode(std::string name, std::string type);
	~serverNode();
	NodeContainer node;
	InternetStackHelper IPstack;
	//ApplicationContainer App;
	//UdpEchoServerHelper *serverApp;
	void restartApp();
	void addUDPApp(uint16_t port, float startTime, float stopTime);
	void addOnOffSinkApp(uint16_t port, float startTime, float stopTime);

private:


};
#endif
