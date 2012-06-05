
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>

#include <sstream>
#include "conn.h"

using namespace ns3;

//createConn::createConn ( NodeContainer* node1,  NodeContainer* node2, Ipv4Address base )
createConn::createConn ( genericNode* node1, genericNode* node2, Ipv4Address base,std::string linkBW, std::string linkDelay,uint32_t connIndex,std::string simID ) 
{
	NodeContainer *aNode1;
	NodeContainer *aNode2;
	this->index=connIndex;
	if ( node1->type == "MPLS" )
	{
		MPLSnode* actualNode1=static_cast<MPLSnode*>(node1);
		aNode1=&actualNode1->node;
	}
	else if ( node1->type == "CLIENT" )
	{
		clientNode* actualNode1=static_cast<clientNode*>(node1);
		aNode1=&actualNode1->node;
	}
	
	else if ( node1->type == "SERVER" )
	{
		serverNode* actualNode1=static_cast<serverNode*>(node1);
		aNode1=&actualNode1->node;
	}

	else //cast into a standard router
	{
		IProuter* actualNode1=static_cast<IProuter*>(node1);
		aNode1=&actualNode1->node;
	}

	if ( node2->type == "MPLS" )
	{
		MPLSnode* actualNode2=static_cast<MPLSnode*>(node2);
		aNode2=&actualNode2->node;
	}
	else if ( node2->type == "CLIENT" )
	{
		clientNode* actualNode2=static_cast<clientNode*>(node2);
		aNode2=&actualNode2->node;
	}
	
	else if ( node2->type == "SERVER" )
	{
		serverNode* actualNode2=static_cast<serverNode*>(node2);
		aNode2=&actualNode2->node;
	}

	else //cast into a standard router
	{
		IProuter* actualNode2=static_cast<IProuter*>(node2);
		aNode2=&actualNode2->node;
	}

	//PtoPConn.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
	//PtoPConn.SetChannelAttribute ("Delay", StringValue ("5ms") );
	PtoPConn.SetDeviceAttribute ("DataRate", StringValue (linkBW.c_str()));
	PtoPConn.SetChannelAttribute ("Delay", StringValue (linkDelay.c_str()) );

  	devices = PtoPConn.Install ( aNode1->Get(0), aNode2->Get(0) );

  	this->address.SetBase (base, "255.255.255.0");
  	this->interfaces = this->address.Assign (devices);



	

	std::stringstream s;
	s<<"/tmp/"<<simID<<"/"<<node1->name;
	AsciiTraceHelper asciiTraceHelper;
	if ( node1->type != "MPLS" )
	{
		this->PtoPConn.EnableAscii (asciiTraceHelper.CreateFileStream (s.str()), *aNode1);
		//this->PtoPConn.EnableAscii (asciiTraceHelper.CreateFileStream (node1->name), *aNode1);
	}
	//this->PtoPConn.EnableAscii (asciiTraceHelper.CreateFileStream (s.str()), *aNode1);
	s.str("");
	s<<"/tmp/"<<simID<<"/"<<node2->name;
	if ( node2->type != "MPLS" )
	{
		this->PtoPConn.EnableAscii (asciiTraceHelper.CreateFileStream (s.str()), *aNode2);
	}
}

uint32_t createConn::getIndex()
{
	return this->index;
}
