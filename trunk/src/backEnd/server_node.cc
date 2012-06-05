#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/applications-module.h>
#include "server_node.h"

using namespace ns3;

serverNode::serverNode(std::string name, std::string type) : genericNode(name,type)
{
	this->node.Create(1);
	IPstack.Install(this->node);
}

void serverNode::addUDPApp(uint16_t port, float startTime, float stopTime)
{
	UdpEchoServerHelper *serverApp=new UdpEchoServerHelper(port);
	ApplicationContainer App= serverApp->Install(this->node.Get(0));
	App.Start (Seconds (startTime));
	App.Stop (Seconds (stopTime));
}

void serverNode::addOnOffSinkApp(uint16_t port,float startTime, float stopTime )
{
	PacketSinkHelper *sinkHelper= new PacketSinkHelper ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
	ApplicationContainer App = sinkHelper->Install (this->node.Get(0));
	App.Start (Seconds (startTime));
	App.Stop (Seconds (stopTime));
}

/*
  uint16_t sinkPort = 8080;
  Address sinkAddress (InetSocketAddress (interfaces.GetAddress (1), sinkPort));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer sinkApps = packetSinkHelper.Install (nodes.Get (1));
  sinkApps.Start (Seconds (0.));
  sinkApps.Stop (Seconds (20.));
*/

serverNode::~serverNode()
{
//	delete serverApp;
}

void serverNode::restartApp()
{
	std::cout<<this->name<<" restarted app"<<std::endl;
}

/*void serverNode::setPort(uint16_t port)
{
	this->port=port;
}*/
