#include "simRun.h"
#include <unistd.h>
#include <iostream>
#include <sstream>

SimRun::SimRun(ClientSocket *client_socket,TopologyMap* topologymap ) : Threader()
{
	this->clientSocket=client_socket;
	this->topologymap=topologymap;

}

SimRun::~SimRun()
{
}

void SimRun::InternalThreadEntry()
{
	std::stringstream s;
	s.str("");
	s<<this->topologymap->getSimID()<<" "<<"SIMSTART";
	if ( topologymap->connStatus == 1 )
	{	
		try
		{
			clientSocket->transmit(s.str());
		}
		catch ( SocketException& e) 
		{
			topologymap->connStatus = 0;
		}
	}
	std::string socket_reply;

	if ( topologymap->connStatus == 1 )
	{	
		try
		{
			clientSocket->receive(socket_reply);
		}
		catch ( SocketException& e) 
		{
			topologymap->connStatus = 0;
		}
	}

	s.str("");
	s<<"DesTroY "<<topologymap->getSimID();

	if ( topologymap->connStatus == 1 )
	{	
		try
		{
			clientSocket->transmit(s.str());
		}
		catch ( SocketException& e) 
		{
			topologymap->connStatus = 0;
		}
	}

	topologymap->setNs3SimStatus(1);
	topologymap->setSimProgress(50);
	topologymap->parseResult();	
}
