#ifndef MPLS_PACKET
#define MPLS_PACKET
#include <vector>
#include <string>
#include <stdint.h>
#include "topology.h"

class MplsPacket : public QObject
{
	Q_OBJECT

public:
	MplsPacket(std::string source, std::string destination,uint32_t startTime,TopologyMap* topologymap);
	~MplsPacket();

	bool updated;

	std::string currentNode;
	std::string nextHop;	

	std::string source;	
	std::string destination;	

	int currentY;
	
	void moveToNext();
	uint32_t startTime;

	void reset();
	void Run();
	void stop();

	struct line
	{
		uint16_t startX;
		uint16_t stopX;
		uint16_t startY;
		uint16_t stopY;
		uint32_t label;
		float grad;
		float intercept;
	};

	line currentLine;

	struct activity
	{
		std::string nextHop;
		std::string currentNode;
		uint32_t currentTime;
		uint32_t label;
		bool done;
		bool mpls;
	};

	
	std::vector<activity*> activityVector;
	bool stopped;

private:
	TopologyMap *topologymap;
	QTimer *timer;
	int counter;
	std::vector<TopologyMap::line*>* packetLinesVector;
	unsigned int lineIndex;
	float currentX;
	int delay;
	void getNextLine();
	bool started;
	bool firstRun;
};
#endif
