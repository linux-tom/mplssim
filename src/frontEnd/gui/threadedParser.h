#ifndef THREADED_PARSER
#define THREADED_PARSER

#include "topology.h"
#include "threader.h"
#include <vector> 
#include <sstream>

class TopologyMap;
class ThreadedParser : public Threader
{

public:
	ThreadedParser(std::string nodeName,TopologyMap* topologymap);
	~ThreadedParser();
	void InternalThreadEntry();
	static pthread_mutex_t incrementMutex;
	static pthread_mutex_t vectorMutex;
private:
	TopologyMap *topologymap;
	std::string simulationID;
	void parseStuff();
	void generateTrafficProfile(std::string pair, unsigned int max_y, unsigned int max_t, unsigned int min_t);
	void generateHistogram(std::string nodeName,int y_axis);
	void insertJunk(QTextStream* stream, unsigned int count );
	std::stringstream s;
	std::string nodeName;
	struct senderNode
	{
		std::string nodeIP;
		std::string nodeName;
		int receivedPackets;
		int droppedPackets;
		int sentPackets;
	};

	std::vector <senderNode*> senderVector;

	struct packetStruct
	{
		unsigned int milliseconds;
		unsigned int receivedPackets;
		unsigned int droppedPackets;
		unsigned int sentPackets;
		std::string nodeName;
	};
	std::vector <packetStruct*> packetsVector;

	struct packetStruct2
	{
		unsigned int seconds;
		unsigned int receivedPackets;
		unsigned int droppedPackets;
		unsigned int sentPackets;
		std::string nodeName;
	};
	
	void updateStatus();
	void pushToVector(TopologyMap::nodeGraph* newGraph);

	std::vector <packetStruct2*> packetsVector2;
};
#endif
