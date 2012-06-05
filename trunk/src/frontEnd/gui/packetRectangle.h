#ifndef PACKET_RECTANGLE
#define PACKET_RECTANGLE

#include "topology.h" 

class TopologyMap;
class PacketRectangle : public QObject
{
	Q_OBJECT

	public:
		PacketRectangle(TopologyMap* topologymap,std::vector<TopologyMap::line*>* packetLinesVector, int delay);
		~PacketRectangle();
		void stop();
		void Run();


	private:
		QTimer *timer;
		TopologyMap* topologymap;
		int counter;
		std::vector<TopologyMap::line*>* packetLinesVector;
		unsigned int lineIndex;
		double currentX;
		int delay;

	private slots:
		void mover();

};
#endif
