
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/applications-module.h>
#include "client_node.h"
#include "burstyApp.h"

using namespace ns3;

clientNode::clientNode(std::string name, std::string type) : genericNode(name,type)
{
	this->node.Create(1);
	IPstack.Install(this->node);
	this->serverSet=0;
}

clientNode::~clientNode()
{
}

void clientNode::addUDPApp(Ipv4Address serverIP, uint16_t port, uint16_t startTime, uint16_t stopTime, uint16_t packNum,  uint16_t packSize, float interval )
{
	ApplicationContainer App;
	UdpEchoClientHelper clientApp(serverIP, port);
	clientApp.SetAttribute ("MaxPackets", UintegerValue (packNum));
	clientApp.SetAttribute ("Interval", TimeValue (MilliSeconds (interval)));
	clientApp.SetAttribute ("PacketSize", UintegerValue (packSize));
	App.Add(clientApp.Install (this->node));
	App.Start (Seconds (startTime));
	App.Stop (Seconds (stopTime));
}

void clientNode::restartApp()
{
	if ( this->serverSet ==1 )
	{
	//	this->setClientApp(this->serverIP);
		std::cout<<"Client App Re-inserted"<<std::endl;
	}

}

void clientNode::addOnOffApp(Ipv4Address serverIP, uint16_t port, uint16_t startTime, uint16_t stopTime )
{
	OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());
	clientHelper.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
	clientHelper.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));

	ApplicationContainer App;
	AddressValue remoteAddress (InetSocketAddress (serverIP, port));
	clientHelper.SetAttribute ("Remote", remoteAddress);
	App.Add (clientHelper.Install (this->node));
	App.Start (Seconds (startTime));
	App.Stop (Seconds (stopTime));
}

void clientNode::addBurstyApp(Ipv4Address sinkIP, uint16_t sinkPort, uint16_t startTime,uint16_t stopTime, uint16_t packetSize, uint16_t packetNum )
{
	Address sinkAddress (InetSocketAddress (sinkIP, sinkPort));
	Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (this->node.Get(0), TcpSocketFactory::GetTypeId ());

	Ptr<BurstyApp> app = CreateObject<BurstyApp> ();
	app->Setup (ns3TcpSocket, sinkAddress, packetSize, packetNum);
	this->node.Get(0)->AddApplication (app);
	app->SetStartTime (Seconds (startTime));
	app->SetStopTime (Seconds (stopTime));	
}
