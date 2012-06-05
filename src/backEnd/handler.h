#ifndef HANDLER_H
#define HANDLER_H


#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/applications-module.h>
#include <ns3/mpls-module.h>

#include <string>
#include <vector>

#include "mpls_node.h"
#include "client_node.h"
#include "server_node.h"
#include "ip_router.h"
#include "conn.h"
#include "csma_conn.h"
#include "genericNode.h"

class Fec;
class Handler
{
	public:
		Handler (std::string simID);
		~Handler ();
		std::string commandOutput;
		std::vector <genericNode*> nodesVector;	
		std::vector <createConn*> connVector;	
		std::vector <createCsmaConn*> csmaConnVector;	
		std::string simID;
		void parseCommand(std::string command);		
		genericNode* getNodeFromName(std::string name);
		createConn* getConnFromIndex(uint32_t index);
		void deleteNode(std::string name);
		void restartApps();
		void resetAll();
	private:
		void scheduleFailure(uint32_t lineIndex, float failTime, bool resume, float resumeTime);
		MplsNetworkConfigurator * network ;
};
#endif
