
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include "ns3/csma-module.h"
#include <sstream>
#include <vector>
#include "csma_conn.h"

using namespace ns3;

createCsmaConn::createCsmaConn ( std::vector <genericNode*> csmaNodeVector, Ipv4Address base,std::string linkBw, std::string linkDelay, std::string simID )
{
	NodeContainer csmaNodes;
	std::vector <NodeContainer*> nodeVector;
	NodeContainer* aNode1; 

	for ( unsigned int i=0; i<csmaNodeVector.size(); i++ )
	{
		if ( csmaNodeVector[i]->type == "MPLS" )
		{
			MPLSnode* actualNode1=static_cast<MPLSnode*>(csmaNodeVector[i]);
			aNode1=&actualNode1->node;
		}
		else if ( csmaNodeVector[i]->type == "CLIENT" )
		{
			clientNode* actualNode1=static_cast<clientNode*>(csmaNodeVector[i]);
			aNode1=&actualNode1->node;
		}
		
		else if ( csmaNodeVector[i]->type == "SERVER" )
		{
			serverNode* actualNode1=static_cast<serverNode*>(csmaNodeVector[i]);
			aNode1=&actualNode1->node;
		}

		else //cast into a standard router
		{
			IProuter* actualNode1=static_cast<IProuter*>(csmaNodeVector[i]);
			aNode1=&actualNode1->node;
		}
		nodeVector.push_back(aNode1);
		csmaNodes.Add (aNode1->Get (0));
	}

	csmaConn.SetChannelAttribute ("DataRate", StringValue (linkBw));
	//csmaConn.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
	csmaConn.SetChannelAttribute ("Delay", StringValue (linkDelay) );


  	devices = csmaConn.Install ( csmaNodes );

	AsciiTraceHelper asciiTraceHelper;
  	this->address.SetBase (base, "255.255.255.0");
  	this->address.Assign (devices);
//	this->PtoPConn.EnablePcapAll ("doubt");
	std::stringstream s;
	for ( unsigned int i=0; i<csmaNodeVector.size(); i++ )
	{
		s.str("");
		s<<"/tmp/"<<simID<<"/"<<csmaNodeVector[i]->name;
		//this->csmaConn.EnableAscii (csmaNodeVector[i]->name, *nodeVector[i]);
		this->csmaConn.EnableAscii (asciiTraceHelper.CreateFileStream (s.str()), *nodeVector[i]);
	}
}
