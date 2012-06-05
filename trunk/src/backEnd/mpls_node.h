#ifndef MPLSNODE_H
#define MPLSNODE_H

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/mpls-module.h>
#include "genericNode.h"
#include <ns3/names.h>
#include <vector>


using namespace ns3;
using namespace mpls;

class MPLSnode : public genericNode
{
public:
	NodeContainer node;
	MplsNetworkConfigurator* network;
	MPLSnode(std::string name, std::string type,MplsNetworkConfigurator * network );
	~MPLSnode();
	void AddFecToNhlfe();
	void AddIncomingLabelMap();		
	void AddFecToNhlfe(std::string sourceIP, std::string destinationIP,uint16_t sourcePort,uint16_t destPort,std::string fecState, Ipv4Address nextHop, uint32_t label );
	void AddIncomingLabelMap(uint32_t label_in, uint32_t label_out, Ipv4Address nextHop);//Swap or forward
	void AddIncomingLabelMap(uint32_t label_in); //pop
	void AddIncomingLabelMap(uint32_t label_in, uint32_t label_out1, Ipv4Address nextHop1, uint32_t label_out2, Ipv4Address nextHop2, uint16_t policyIndex);
	void AddNhlfePolicy(uint16_t index,std::string BorP,uint32_t attr);
	void AddWeightedPolicy(uint16_t index,std::string BorP, uint32_t attr,float weight1, float weight2);
	void AddFecToNhlfe(std::string sourceIP,std::string destinationIP,Ipv4Address nextHop,uint32_t label,Ipv4Address nextHop2,uint32_t label2,uint16_t policyIndex);

private:
	uint16_t policyIndex;
	struct policyStruct
	{
		uint16_t index;
		NhlfeSelectionPolicyHelper policy;
	};
	std::vector<policyStruct*> policyVector;

	struct weightedPolicyStruct
	{
		uint16_t index;
		WeightedPolicyHelper policy;
	};

	std::vector<weightedPolicyStruct*> weightedPolicyVector;

	MplsSwitch *mplsswitch;
};
#endif
