#include "sim_handler.h"
#include <string>
#include <iostream>
#include <sstream>


simHandler::simHandler(std::string Id)
{
	this->ID=Id;
	lotr=new Handler(this->ID);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("mpls::MplsProtocol", LOG_LEVEL_INFO);
//	LogComponentEnable ("mpls::Ipv4Routing", LOG_LEVEL_INFO);
	LogComponentEnable ("MplsNetworkDiscoverer", LOG_LEVEL_INFO);  
	//NS_LOG_APPEND_TIME_PREFIX;
	//NS_LOG_APPEND_NODE_PREFIX;    
}

simHandler::~simHandler()
{
	delete lotr ;
}

std::string simHandler::handleCommand(std::string command)
{

	std::stringstream s;
	s.str("");
	std::cout<<command<<std::endl;
	if ( command == "SIMSTART" )
	{
		Ipv4GlobalRoutingHelper::PopulateRoutingTables ();	
		//MPLSnode::network.DiscoverNetwork ();
		Simulator::Run ();
		Simulator::Destroy ();
		this->lotr->resetAll();
		this->simStatus = 1;
		return ( "success" );
	}
	else
	{
		this->lotr->parseCommand(command);
		command=this->lotr->commandOutput;
		if ( command == "SimSuccess" )
		{
			this->simStatus = 1;
		}
		s<<"MplsSIM: "<< command;	
		return ( s.str() );
	}
}

std::string simHandler::getID()
{
	return this->ID;
}
bool simHandler::getSimStatus()
{
	return this->simStatus;
}
