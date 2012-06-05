#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/mpls-module.h>
#include <iostream>
#include "mpls_node.h"

using namespace ns3;
using namespace mpls;

MPLSnode::MPLSnode (std::string name, std::string type, MplsNetworkConfigurator * network ) : genericNode(name,type)
{
	this->policyIndex=0;
	this->network=network;
	node = network->CreateAndInstall (1);
	mplsswitch = new MplsSwitch(node.Get(0)); 
	//Names::Add(name,node.Get(0));
}

MPLSnode::~MPLSnode ()
{
	delete mplsswitch;
}

void MPLSnode::AddFecToNhlfe()
{
  mplsswitch->AddFtn (Ipv4Source ("192.168.1.0/24") && Ipv4Destination ("192.168.5.0/24"),Nhlfe (Swap (100), Ipv4Address ("10.1.1.2")));
}

void MPLSnode::AddIncomingLabelMap()
{
	mplsswitch->AddIlm (300,Nhlfe(Swap (400), Ipv4Address ("10.1.1.1")));
}

void MPLSnode::AddFecToNhlfe(std::string sourceIP, std::string destinationIP,uint16_t sourcePort,uint16_t destPort,std::string fectype, Ipv4Address nextHop, uint32_t label )
{
			//	mplsnode->AddFecToNhlfe(sourceIP,destinationIP,destinationPort,fectype,nextHop,label);
	if ( fectype == "SIP" )
	{
		//ns3::mpls::Fec fec(Ipv4Source ("192.168.1.1") && Ipv4Destination ("192.168.4.2"));
		//fec=&(Ipv4Source ("192.168.1.1") && Ipv4Destination ("192.168.4.2"));
		mplsswitch->AddFtn (Ipv4Source (sourceIP.c_str()) ,Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "SIPSUP" )
	{

		mplsswitch->AddFtn (UdpSourcePort(sourcePort) && Ipv4Source(destinationIP.c_str()) , Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "SIPSTP" )
	{

		mplsswitch->AddFtn (TcpSourcePort(sourcePort) && Ipv4Source(destinationIP.c_str()) , Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "SIPUP" )
	{

		mplsswitch->AddFtn (UdpDestinationPort(destPort) && Ipv4Source(destinationIP.c_str()) , Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "SIPTP" )
	{
		mplsswitch->AddFtn (TcpDestinationPort(destPort) && Ipv4Source(destinationIP.c_str()) , Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "SIPDIP" )
	{
		mplsswitch->AddFtn (Ipv4Source (sourceIP.c_str()) && Ipv4Destination (destinationIP.c_str()),Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "SIPDIPTP" )
	{
		mplsswitch->AddFtn (Ipv4Source (sourceIP.c_str()) && Ipv4Destination (destinationIP.c_str()),Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "SIPDIPUP" )
	{
		mplsswitch->AddFtn (Ipv4Source (sourceIP.c_str()) && Ipv4Destination (destinationIP.c_str()),Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "DIP" )
	{
		mplsswitch->AddFtn (Ipv4Destination (destinationIP.c_str()) ,Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "DIPTP" )
	{
		mplsswitch->AddFtn (TcpDestinationPort(destPort) && Ipv4Destination(destinationIP.c_str()) , Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "DIPUP" )
	{
		mplsswitch->AddFtn (UdpDestinationPort(destPort) && Ipv4Destination(destinationIP.c_str()) , Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "DIPSUP" )
	{
		mplsswitch->AddFtn (UdpSourcePort(sourcePort) && Ipv4Destination(destinationIP.c_str()) , Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "DIPSTP" )
	{
		mplsswitch->AddFtn (TcpSourcePort(sourcePort) && Ipv4Destination(destinationIP.c_str()) , Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "UP" )
	{
		mplsswitch->AddFtn (UdpDestinationPort(destPort), Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "TP" )
	{
		mplsswitch->AddFtn (TcpDestinationPort(sourcePort), Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "SUP" )
	{
		mplsswitch->AddFtn (UdpSourcePort(sourcePort), Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else if ( fectype == "STP" )
	{
		mplsswitch->AddFtn (TcpSourcePort(sourcePort), Nhlfe (Swap (label), Ipv4Address (nextHop)));
		std::cout<<fectype<<std::endl;
	}
	else{}
}


void MPLSnode::AddIncomingLabelMap(uint32_t label_in, uint32_t label_out, Ipv4Address nextHop)
{
	mplsswitch->AddIlm (label_in,Nhlfe(Swap (label_out), Ipv4Address (nextHop)));
}
void MPLSnode::AddIncomingLabelMap(uint32_t label_in)
{
	mplsswitch->AddIlm (label_in,Nhlfe(Pop ()));
}

void MPLSnode::AddFecToNhlfe(std::string sourceIP,std::string destinationIP,Ipv4Address nextHop,uint32_t label,Ipv4Address nextHop2,uint32_t label2,uint16_t policyIndex)
{

	uint16_t i;
	for ( i=0; i<policyVector.size(); i++ )
	{
		if ( policyVector[i]->index == policyIndex )
		{
			mplsswitch->AddFtn (Ipv4Source (sourceIP.c_str()) && Ipv4Destination (destinationIP.c_str()),Nhlfe (Swap (label), Ipv4Address (nextHop)),Nhlfe (Swap (label2), Ipv4Address (nextHop2)),policyVector[i]->policy );

			break;
		}
	}
	
	if ( i == policyVector.size() )
	{

		for ( uint16_t j=0; j<weightedPolicyVector.size(); j++ )
		{
			if ( weightedPolicyVector[j]->index == policyIndex )
			{
				mplsswitch->AddFtn (Ipv4Source (sourceIP.c_str()) && Ipv4Destination (destinationIP.c_str()),Nhlfe (Swap (label), Ipv4Address (nextHop)),Nhlfe (Swap (label2), Ipv4Address (nextHop2)), weightedPolicyVector[j]->policy );
				break;
			}
		}
	}

}

void MPLSnode::AddIncomingLabelMap(uint32_t label_in, uint32_t label_out1, Ipv4Address nextHop1, uint32_t label_out2, Ipv4Address nextHop2, uint16_t policyIndex)
{
	uint16_t i;
	for ( i=0; i<policyVector.size(); i++ )
	{
		if ( policyVector[i]->index == policyIndex )
		{
			mplsswitch->AddIlm (label_in, Nhlfe(Swap (label_out1), Ipv4Address (nextHop1)), Nhlfe(Swap (label_out2), Ipv4Address (nextHop2)),policyVector[i]->policy );
			break;
		}
	}
	
	if ( i == policyVector.size() )
	{

		for ( uint16_t j=0; j<weightedPolicyVector.size(); j++ )
		{
			if ( weightedPolicyVector[j]->index == policyIndex )
			{
				mplsswitch->AddIlm (label_in, Nhlfe(Swap (label_out1), Ipv4Address (nextHop1)), Nhlfe(Swap (label_out2), Ipv4Address (nextHop2)),weightedPolicyVector[j]->policy );
				break;
			}
		}
	}
}

void MPLSnode::AddNhlfePolicy(uint16_t index,std::string BorP,uint32_t attr)
{
	policyStruct *pol=new policyStruct;
	pol->index=index;
	NhlfeSelectionPolicyHelper polici;
	if ( BorP == "PACKETS" )
	{
		polici.SetAttribute ("MaxPacketsInTxQueue", IntegerValue (attr));
	}
	else if ( BorP == "BYTES" )
	{
		polici.SetAttribute ("MaxBytesInTxQueue", IntegerValue (attr));
	}
	else
	{
		return;
	}
	pol->policy=polici;
	policyVector.push_back(pol);	
}

void MPLSnode::AddWeightedPolicy(uint16_t index, std::string BorP, uint32_t attr,float weight1, float weight2)
{

	weightedPolicyStruct *pol=new weightedPolicyStruct;
	pol->index=index;	
	WeightedPolicyHelper polici;
	polici.AddWeight(weight1,weight2);
	if ( BorP == "packets" )
	{
		polici.SetAttribute ("MaxPacketsInTxQueue", IntegerValue (attr));
	}
	else if ( BorP == "bytes" )
	{
		polici.SetAttribute ("MaxBytesInTxQueue", IntegerValue (attr));
	}
	else
	{
		return;
	}
	pol->policy=polici;
	weightedPolicyVector.push_back(pol);	
}
