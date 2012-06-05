#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/applications-module.h>
#include <ns3/mpls-module.h>

#include <string>
#include <iostream>
#include "mpls_node.h"
#include "client_node.h"
#include "server_node.h"
#include "ip_router.h"
#include "conn.h"
#include "csma_conn.h"
#include "handler.h"
#include <vector>
#include <sstream>

using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("MPLS_SIM_SERVER");


Handler::Handler(std::string simID)
{
	LogComponentEnable ( "MPLS_SIM_SERVER", LOG_LEVEL_INFO ); 
	network=new MplsNetworkConfigurator();
	this->simID=simID;
}

Handler::~Handler()
{
	delete network;
	for ( size_t i=0; i<nodesVector.size(); i++ )
	{
		delete nodesVector[i];
	}
	nodesVector.clear();	
}

static void setLinkDown(uint32_t* lineIndex,Ipv4InterfaceContainer* interfaces)
{
	Ptr<Ipv4> iface1=(interfaces->Get(0)).first;
	unsigned int ifNo=(interfaces->Get(0)).second;
	Ptr<Ipv4> iface2=(interfaces->Get(1)).first;
	unsigned int if2No=(interfaces->Get(1)).second;

	iface1->SetDown(ifNo);
	iface2->SetDown(if2No);
	std::stringstream ss;
	ss<<"link " << *lineIndex <<" SET DOWN";
	NS_LOG_INFO(ss.str());
}

static void setLinkUp(uint32_t* lineIndex,Ipv4InterfaceContainer* interfaces)
{
	Ptr<Ipv4> iface1=(interfaces->Get(0)).first;
	unsigned int ifNo=(interfaces->Get(0)).second;
	Ptr<Ipv4> iface2=(interfaces->Get(1)).first;
	unsigned int if2No=(interfaces->Get(1)).second;

	iface1->SetUp(ifNo);
	iface2->SetUp(if2No);
	std::stringstream ss;
	ss<<"link " << *lineIndex <<" SET UP";
	NS_LOG_INFO(ss.str());
}

void Handler::parseCommand(std::string command)
{
	//Load into vector
	std::vector <std::string> splitCommand;
	
	std::istringstream iss(command);

	std::stringstream s("");

	std::string sub;

	while (iss >> sub)
	{
		splitCommand.push_back(sub);
	}
  
	//Now lets parse
	//Valid verbs are : CREATE DELETE CONNECT START

	std::string verb=splitCommand[0];

	if ( verb == "CREATE" ) 
	{
		std::string nodeType=splitCommand[1];
		std::string nodeName=splitCommand[2];
		if ( nodeType == "MPLS" )
		{
			this->nodesVector.push_back(new MPLSnode(nodeName,nodeType,network));	
			s<<nodeName<<" "<<"Created";
			this->commandOutput=s.str();				
		}
		else if ( nodeType == "ROUTER" )
		{
			this->nodesVector.push_back(new IProuter(nodeName,nodeType));					
			s<<nodeName<<" "<<"Created";
			this->commandOutput=s.str();				
		}
		else if ( nodeType == "CLIENT" )
		{
			this->nodesVector.push_back(new clientNode(nodeName,nodeType));					
			s<<nodeName<<" "<<"Created";
			this->commandOutput=s.str();				
		}
		else if (nodeType == "SERVER" )
		{
			this->nodesVector.push_back(new serverNode(nodeName,nodeType));					
			s<<nodeName<<" "<<"Created";
			this->commandOutput=s.str();				
		}
		else
		{		
			s<< "No such object";
			this->commandOutput=s.str();				
		}	
	}	
	
	else if ( verb == "CONNECT" )
	{
		std::string node1Name=splitCommand[1];
		std::string node2Name=splitCommand[2];
		Ipv4Address baseIP=splitCommand[3].c_str();
		std::string linkBW=splitCommand[4].c_str();
		std::string linkDelay=splitCommand[5].c_str();

		uint32_t connIndex;
		std::istringstream buffer(splitCommand[6]);
		buffer>>connIndex;		
		
		genericNode* node1=this->getNodeFromName(node1Name);
		genericNode* node2=this->getNodeFromName(node2Name);

		//Connect actualNodes

		this->connVector.push_back(new createConn ( node1, node2, baseIP, linkBW,linkDelay,connIndex, this->simID )); 
						
		s<<node1Name<<" connected to "<<node2Name;
		this->commandOutput=s.str();				
			
		
	}
	
	else if ( verb ==  "ADD" ) //ADD LSR0 FTN 192.168.1.0/24 192.168.4.5 192.168.2.1 100
	{
		std::string nodeName=splitCommand[1];
		genericNode* netnode=this->getNodeFromName(nodeName);
		
		if ( netnode->type == "MPLS" )
		{
			MPLSnode* mplsnode=static_cast<MPLSnode*>(netnode);
			if ( splitCommand[2] == "FTN" )
			{
				//const ns3::mpls::Fec* fec;

				  //template<class T>   const T &fec;

				std::string fectype=splitCommand[3];
				std::string sourceIP=splitCommand[4];
				std::string destinationIP=splitCommand[5];
				uint16_t destPort;
				std::istringstream buffer(splitCommand[7]);
				buffer>>destPort;		
				buffer.clear();

				uint16_t srcPort;
				buffer.str(splitCommand[6]);
				buffer>>srcPort;		
				buffer.clear();

				Ipv4Address nextHop=splitCommand[8].c_str();

				uint16_t label;
				buffer.str(splitCommand[9]);
				buffer>>label;		

				//ADD LSR_0 FTN UP .../ .../ 0 80 192.168.2.2 100 

				mplsnode->AddFecToNhlfe(sourceIP,destinationIP,srcPort,destPort,fectype,nextHop,label);
				//mplsnode->AddFecToNhlfe(fec,nextHop,label);
				this->commandOutput="OK";				
				//s<<"ADD "<<ftnVector[i]->nodeName<<" FTN "<<ftnVector[i]->fec<<" "<<ftnVector[i]->sourceIP<<" "<<ftnVector[i]->destinationIP<<" "<<ftnVector[i]->destinationPort<<" "<<neighIP<<" "<<ftnVector[i]->label;
			}
				
			else if ( splitCommand[2] == "ILM" )
			{
				uint16_t label_in;
				uint16_t label_out;
				
				std::istringstream buffer(splitCommand[3]);
				buffer>>label_in;
				buffer.clear();
				buffer.str(splitCommand[4]);	
				buffer>>label_out;
				Ipv4Address nextHop=splitCommand[5].c_str();

				mplsnode->AddIncomingLabelMap(label_in, label_out, nextHop);
				this->commandOutput="OK";				

			}
			else if ( splitCommand[2] == "POP" ) // ADD LSR0 POP 200 
			{
				std::istringstream buffer(splitCommand[3]);
				uint16_t label_in;
				buffer>>label_in;
				mplsnode->AddIncomingLabelMap(label_in);
				this->commandOutput="OK";				
			}
			else if ( splitCommand[2] == "FASTREROUTE" )
			{
				if ( splitCommand[3] == "ILM" )
				{
					uint16_t label_in;
					std::istringstream buffer(splitCommand[4]);
					buffer >> label_in;
					buffer.clear();
					uint16_t label1_out;
					buffer.str(splitCommand[5]);
					buffer >> label1_out;
					buffer.clear();
					Ipv4Address nextHop1=splitCommand[6].c_str();
					uint16_t label2_out;
					buffer.str(splitCommand[7]);
					buffer >> label2_out;
					buffer.clear();
					Ipv4Address nextHop2=splitCommand[8].c_str();
					uint16_t policyIndex;
					buffer.str(splitCommand[9]);
					buffer >> policyIndex;
				
					mplsnode->AddIncomingLabelMap(label_in, label1_out, nextHop1, label2_out, nextHop2, policyIndex);
					//s<<"ADD "<<ilmVector[i]->nodeName<<" FASTREROUTE ILM "<<ilmVector[i]->incomingLabel<<" "<<ilmVector[i]->outgoingLabel<<" "<<neigh1IP<<" "<<ilmVector[i]->outgoingLabel2<<" "<<neigh2IP<<" "<<ilmVector[i]->policyIndex;
					this->commandOutput="OK";				
				}
				else if ( splitCommand[3] == "FTN" )
				{

					std::string sourceIP=splitCommand[4];
					std::string destinationIP=splitCommand[5];
					Ipv4Address nextHop=splitCommand[6].c_str();
					uint16_t label;
					std::istringstream buffer(splitCommand[7]);
					buffer>>label;		
					Ipv4Address nextHop2=splitCommand[8].c_str();
					uint16_t label2;
					buffer.clear();
					buffer.str(splitCommand[9]);
					buffer>>label2;		
					uint16_t policyIndex;
					buffer.clear();
					buffer.str(splitCommand[10]);
					buffer>>policyIndex;		
					
					//ADD LSR_0 FASTREROUTE FTN 192.168.001.001/32 192.168.002.002/32 192.168.3.2 100 192.168.5.2 200 0 

					mplsnode->AddFecToNhlfe(sourceIP,destinationIP,nextHop,label,nextHop2,label2,policyIndex);
					this->commandOutput="OK";				
				}

				else{}

			}
			else if ( splitCommand[2] == "POLICY" )
			{
				//ADD LSR0 POLICY DEFAULT 0  BYTES 10000
				if ( splitCommand[3] == "DEFAULT" )
				{
					uint16_t index;
					std::istringstream buffer(splitCommand[4]);
					buffer >> index;
					buffer.clear();
					std::string BorP=splitCommand[5];
					uint32_t attr;
					buffer.str(splitCommand[6]);	
					buffer >> attr;
					
					mplsnode->AddNhlfePolicy(index,BorP,attr);
				}
				else if ( splitCommand[3] == "WEIGHTED" )
				{
					
					uint16_t index;
					std::istringstream buffer(splitCommand[4]);
					buffer >> index;
					buffer.clear();
					std::string BorP=splitCommand[5];
					uint32_t attr;
					buffer.str(splitCommand[6]);
					buffer >> attr;
					buffer.clear();
					float weight1;
					buffer.str(splitCommand[7]);
					buffer >> weight1;
					buffer.clear();
					float weight2;
					buffer.str(splitCommand[8]);
					buffer >> weight2;
					buffer.clear();
					mplsnode->AddWeightedPolicy(index,BorP, attr,weight1,weight2);
				}

				else{}

			}
			else{}
		}
		else if ( netnode->type == "CLIENT" )
		{ 
			clientNode* clientnode=static_cast<clientNode*>(netnode);
			if ( splitCommand[2] == "APP" )
			{
				if ( splitCommand[3] == "UDP" )
				{
					Ipv4Address serverIP=splitCommand[4].c_str();
					uint16_t port;
					std::istringstream buffer(splitCommand[5]);
					buffer>>port;
					buffer.clear();
					float startTime;
					buffer.str(splitCommand[6]);
					buffer>>startTime;
					buffer.clear();
					float stopTime;
					buffer.str(splitCommand[7]);
					buffer>>stopTime;
					buffer.clear();
					uint16_t packNum;
					buffer.str(splitCommand[8]);
					buffer>>packNum;
					buffer.clear();
					uint16_t packSize;
					buffer.str(splitCommand[9]);
					buffer>>packSize;
					buffer.clear();
					float interval;
					buffer.str(splitCommand[10]);
					buffer>>interval;
					buffer.clear();
					
					clientnode->addUDPApp(serverIP, port, startTime, stopTime, packNum, packSize, interval);	

				}
				else if ( splitCommand[3] == "TCP" )
				{
					Ipv4Address serverIP=splitCommand[4].c_str();
					uint16_t port;
					std::istringstream buffer(splitCommand[5]);
					buffer>>port;
					buffer.clear();
					float startTime;
					buffer.str(splitCommand[6]);
					buffer>>startTime;
					buffer.clear();
					float stopTime;
					buffer.str(splitCommand[7]);
					buffer>>stopTime;
					buffer.clear();
					clientnode->addOnOffApp(serverIP, port, startTime, stopTime);	
					this->commandOutput="OK";				

				}

				else if ( splitCommand[3] == "BURST" )
				{
					Ipv4Address sinkIP=splitCommand[4].c_str();
					uint16_t sinkPort;
					std::istringstream buffer(splitCommand[5]);
					buffer>>sinkPort;
					buffer.clear();
					float startTime;
					buffer.str(splitCommand[6]);
					buffer>>startTime;
					buffer.clear();
					float stopTime;
					buffer.str(splitCommand[7]);
					buffer>>stopTime;
					buffer.clear();

					uint16_t packNum;
					buffer.str(splitCommand[8]);
					buffer>>packNum;
					buffer.clear();
					uint16_t packSize;
					buffer.str(splitCommand[9]);
					buffer>>packSize;
					buffer.clear();
					//float interval;
					//buffer.str(splitCommand[10]);
					//buffer>>interval;
					//buffer.clear();
					
					//clientnode->addOnOffApp(serverIP, port, startTime, stopTime);	
					this->commandOutput="OK";				
					clientnode->addBurstyApp(sinkIP, sinkPort, startTime,stopTime, packSize, packNum );
				}
				
//void clientNode::addBurstyApp(Ipv4Address sinkIP, uint16_t sinkPort, uint16_t startTime,uint16_t stopTime, uint16_t packetSize, uint16_t packetNum )
				//void clientNode::addBurstyApp(Ipv4Address sinkIP, uint16_t sinkPort, uint16_t startTime,uint16_t stopTime, string datarate)
				else
				{
				}
			}
		
		}
		else if ( netnode->type == "SERVER" )
		{
			serverNode* servernode=static_cast<serverNode*>(netnode);
			if ( splitCommand[2] == "APP" )
			{
				if ( splitCommand[3] == "UDP" )
				{	
					std::istringstream buffer(splitCommand[4]);
					uint16_t port;
					buffer>>port;
					buffer.clear();
					uint16_t startTime;
					buffer.str(splitCommand[5]);
					buffer>>startTime;
					buffer.clear();
					uint16_t stopTime;
					buffer.str(splitCommand[6]);
					buffer>>stopTime;
					buffer.clear();

					servernode->addUDPApp(port, startTime, stopTime);

					this->commandOutput="OK";				
	
				}
				else if ( splitCommand[3] == "TCP" )
				{	
					std::istringstream buffer(splitCommand[4]);
					uint16_t port;
					buffer>>port;
					buffer.clear();
					uint16_t startTime;
					buffer.str(splitCommand[5]);
					buffer>>startTime;
					buffer.clear();
					uint16_t stopTime;
					buffer.str(splitCommand[6]);
					buffer>>stopTime;
					buffer.clear();

					servernode->addOnOffSinkApp(port, startTime, stopTime);

					this->commandOutput="OK";				
				}

				else
				{
				}
			}	
		}
		else
		{
		}

		
	}

	else if ( verb ==  "DELETE" )
	{

	}

	else if ( verb == "CREATECSMA" )
	{
		Ipv4Address baseIP=splitCommand[1].c_str();
		std::string linkBw=splitCommand[2].c_str();
		std::string linkDelay=splitCommand[3].c_str();

		std::vector <genericNode*> csmaNodesVector;	
		for ( unsigned int i=4; i<splitCommand.size(); i++ ) 
		{
			genericNode* node1=this->getNodeFromName(splitCommand[i]);
			//Force some typecasts :-)

			csmaNodesVector.push_back(node1);		
		}	
		
		this->csmaConnVector.push_back(new createCsmaConn ( csmaNodesVector, baseIP, linkBw, linkDelay, this->simID )); 
		this->commandOutput=s.str();				
	}

	else if ( verb == "SIMSTART" )
	{
		std::stringstream ss;
		ss<<"\n\n"<<simID<<" STARTED\n\n";
		NS_LOG_INFO(ss.str());

		Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
		network->DiscoverNetwork ();
		Simulator::Run ();
		Simulator::Destroy ();
		this->commandOutput="SimSuccess";				

		ss.str("");
		ss<<"\n\n"<<simID<<" STOPPED\n\n";
		NS_LOG_INFO(ss.str());
		//MPLSnode::network.Reset ();
	}

	else if ( verb == "SCHEDULE" )
	{
		if ( splitCommand[1] == "P2PLINKFAILURE" )
		{
			uint32_t lineIndex;
			float failTime;
			bool resume;
			float resumeTime;

			std::istringstream buffer;
			buffer.clear();
			buffer.str(splitCommand[2]);
			buffer>>lineIndex;
			buffer.clear();
			buffer.str(splitCommand[3]);
			buffer>>failTime;
			buffer.clear();
			buffer.str(splitCommand[4]);
			buffer>>resume;
			buffer.clear();
			buffer.str(splitCommand[5]);
			buffer>>resumeTime;
			
			this->scheduleFailure(lineIndex,failTime,resume,resumeTime);
			std::cout << lineIndex << " "<<failTime<<" "<<resume<<" "<<resumeTime<<std::endl;
			//SCHEDULE P2PLINKFAILURE 12 15.6 1 30
			//SCHEDULE P2PLINKFAILURE 0 5 1 15

		}

	}

	else
	{

		s<<"Command Not Understood, ignoring"<<std::endl;
		this->commandOutput=s.str();				
		return;
	}
}

genericNode* Handler::getNodeFromName(std::string name)
{
	for ( unsigned int i=0; i<this->nodesVector.size(); i++ )
	{
		if ( this->nodesVector[i]->name == name )
		{
			return this->nodesVector[i];
		}	
	}
	return NULL;
}

void Handler::restartApps()
{

	for ( unsigned int i=0; i<this->nodesVector.size(); i++ )
	{
		
		if ( this->nodesVector[i]->type == "SERVER" )
		{

			serverNode* actualNode1=static_cast<serverNode*>(nodesVector[i]);
			actualNode1->restartApp();
		}
		else if ( this->nodesVector[i]->type == "CLIENT" )
		{

			clientNode* actualNode1=static_cast<clientNode*>(nodesVector[i]);
			actualNode1->restartApp();
		}
		else
		{

		}
	}

}

void Handler::resetAll()
{
	for (unsigned int i=0; i<nodesVector.size(); i++ )
	{
		delete nodesVector[i];
		this->nodesVector.erase(this->nodesVector.begin()+i);
		i--;
	}
	for (unsigned int i=0; i<connVector.size(); i++ )
	{
		delete connVector[i];
		this->connVector.erase(this->connVector.begin()+i);
		i--;
	}
	for (unsigned int i=0; i<csmaConnVector.size(); i++ )
	{
		delete csmaConnVector[i];
		this->csmaConnVector.erase(this->csmaConnVector.begin()+i);
		i--;
	}
}

createConn* Handler::getConnFromIndex(uint32_t index)
{
	for ( uint32_t i=0; i<connVector.size(); i++ )
	{
		if ( index == connVector[i]->getIndex() )
		{
			return connVector[i];
		}
	}
	return NULL;
}

void Handler::scheduleFailure(uint32_t lineIndex, float failTime, bool resume, float resumeTime)
{
	createConn* connline=getConnFromIndex(lineIndex);
	Ipv4InterfaceContainer* interfaces=&connline->interfaces ;
	uint32_t* index=&connline->index;
		
	Simulator::Schedule (Seconds (failTime), &setLinkDown,index, interfaces);

	if ( resume )
	{
		Simulator::Schedule (Seconds (resumeTime), &setLinkUp,index ,interfaces);
	}
}
