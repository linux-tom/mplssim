#ifndef MPLS_PARSER_H
#define MPLS_PARSER_H
#include <QFile>
#include <QString>
#include <QTextStream>
#include <sstream>
#include <vector>
#include "mplsPacket.h"
#include "topology.h"
#include "threader.h"
#include <QStringList>

class MplsParser : public Threader
{
	public: 
		MplsParser(std::string filename);
		MplsParser(std::string filename, TopologyMap* topologymap);
		~MplsParser();
		//void internalThreadEntry();
		void InternalThreadEntry();
		std::string simID;
		QFile* resultFile;
		QFile* simFile;
		std::stringstream s;
		void insertJunk(QTextStream *stream, unsigned int count);
		void skipLine(QTextStream* in, uint16_t count);
		std::string getNodeNameFromID(uint16_t ID);
		std::string getNodeNameFromIP(std::string IP);
		std::string getNodeNameFromMac(std::string mac);
		void classifyMPLSPackets();
		void createObjects(); 
		void theRealStuff();
		void followToEnd(MplsPacket *packet,int index);
		std::string getSelectedPolicy(int& index);

		struct nodeIDStruct
		{
			std::string nodeName;
			uint16_t ID;
			std::vector<std::string> macAddressList;
			std::string IPaddress;
		};

		std::vector<nodeIDStruct*> nodeIDVector;
		std::vector<MplsPacket*> mplsPacketVector;
		int nextTime;
	private:
		QStringList* fileLines;
		TopologyMap* topologymap;
		void parseStuff();
};
#endif
