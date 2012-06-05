#ifndef SIMRUN
#define SIMRUN

#include "threader.h"
#include "topology.h"
#include "../client/ClientSocket.h"
#include "../client/SocketException.h"

class SimRun : public Threader
{
	public:
		SimRun(ClientSocket *client_socket, TopologyMap* topologymap );
		~SimRun();
		void InternalThreadEntry();
	private:
		ClientSocket *clientSocket;
		TopologyMap* topologymap;

}; 
#endif
