#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <QFrame>
#include <QLabel>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <sstream>
#include <string>
#include "nodeIcon.h"
#include "connLine.h"
#include "../client/ClientSocket.h"
#include "../client/SocketException.h"
#include "nodeCreator.h"
#include <QStatusBar>
//#include "packetRectangle.h"

class QDragEnterEvent;
class QDropEvent;
class NodeIcon;
class AppStore;
class NodeCreator;
class ThreadedParser;
class PacketRectangle;
class MplsPacket;
//class ConnLine;

class TopologyMap : public QFrame
//class TopologyMap : public QWidget
{
	Q_OBJECT  

public:
	TopologyMap(QStatusBar *statusBar,QAction *startsim,QWidget *parent=0);
	~TopologyMap();
	void showMeTheStateOfThings();
	void startMplsSimulation();
	void stopMplsSimulation();
	void showLSP();
	void stopLSP();
	void parseMPLS();
	bool getLspMode();
	void selfDestruct();
	void incrementParsed();
	QPoint getNodeIconPos(std::string nodeName);
	uint16_t getNodeIconHeight(std::string nodeName);
	uint16_t getNodeIconWidth(std::string nodeName);
	std::string getSimID();
	void paintEvent(QPaintEvent *event);
	bool enableLine;
	bool deleteNode;
	bool connStatus;
	void parseResult();
	void drawPacketRectangle();
	//ConnLine currentLine;

	int32_t animationFinished;
	int32_t animationStarted;

	QAction *simStart;
	
	void setSimStatus(bool s);
	void setNs3SimStatus(bool s);
	void setSimRunning(bool s);
	void setSimProgress(unsigned short s);
	bool terminateNS3();
	std::string status;
	std::string simFile;

	void initializeButtons(QAction* addlsr, QAction* addclient, QAction* addserver,QAction*	addip,QAction* addSwitch,QAction*		 build,QAction * addline, QAction* remove, QAction* cont, QAction* startsim);
		
	struct nodeGraph
	{
		std::string nodeName;
		std::string graphName;
		std::string fileName;
		unsigned int index;
	};

	std::vector<nodeGraph*> graphsVector;

	struct ilmStruct
	{
		std::string nodeName;
		unsigned int incomingLabel;
		unsigned int outgoingLabel;
		std::string nextHop;
		std::string operation;
		bool fastreroute;
		unsigned int outgoingLabel2;
		std::string nextHop2;
		int index;
		unsigned int policyIndex;
	};

	struct ftnStruct
	{
		std::string nodeName;
		unsigned int label;
		unsigned int label2;
		std::string sourceIP;
		std::string destinationIP;
		uint16_t sourcePort;
		uint16_t destinationPort;
		std::string fec;
		std::string nextHop;
		std::string nextHop2;
		bool fastreroute;
		int index;
		unsigned int policyIndex;
	};
	struct clientAppStruct
	{
		std::string nodeName;
		std::string appType;
		std::string destServer;
		unsigned int destPort;
		unsigned int packetSize;
		unsigned int packetNum;
		float startTime;
		float stopTime;
		float interval;
		unsigned int index;
	};

	struct serverAppStruct
	{
		std::string nodeName;
		std::string appType;
		unsigned int port;
		float startTime;
		float stopTime;
		unsigned int index;
	};

	struct line
	{
		float startX;
		float stopX;
		float startY;
		float stopY;
		unsigned int label;
	};

	struct lsp
	{
		unsigned int label;
		std::vector<line*> linesVector;
		unsigned int index;
	};
	std::vector<lsp*> lspVector;

	struct labelColour
	{
		unsigned int label;
		QColor* colour;	
	};

	std::vector<QColor> colourVector;	
	std::vector<PacketRectangle*> packetRectangleVector;
	std::vector<labelColour*> labelColourVector;
	unsigned int labelColourIndex;

	void AddClientNode();
	void AddServerNode();
	void AddStandardRouterNode();
	void AddMplsNode();
	void AddSwitchNode();
	void createConnection(std::string name);
	void configureClientApp(std::string name);
	void configureServerApp(std::string name);
	void showInstalledClientApps(std::string name);
	void showInstalledServerApps(std::string name);
	void RunSimulation();
	void displaySimResults();
	void setServerIP(std::string name);
	void changeCSMALinkParams(std::string switchName);
	void popLabel(std::string name);
	void addFTN(std::string name);
	void addILM(std::string name);
	std::string getNodeFromIP(std::string IPaddress );
	std::string sendCommand(std::string command);
	QColor getLabelColour(unsigned int label);

	NodeIcon* getNodeIconFromName(std::string name);	
	std::string getNeighIpAddress(std::string node1, std::string neigh1);	
	std::vector <NodeIcon*> nodesVector;
	std::vector <ConnLine*> linesVector;
	std::vector <NodeCreator*> nodeCreateVector;
	std::vector <std::string> instructions;
	std::vector <std::string> serverList;
	std::vector <std::string> clientList;
	std::vector <std::string> routerList;
	std::vector <std::string> lsrList;
	std::vector <std::string> switchList;
	void showGraphs(std::string nodeName);
	void deleteNodeLinks(std::string name);
	void resetAll();
	unsigned int vectorIndex;
	unsigned int linesVectorIndex;
	void continueSimulation();
	void showLsrDatabase(std::string nodeName);
	std::vector<MplsPacket*> mplsPacketVector;
	void mplsSimulation();
private:
	ClientSocket *client_socket;
	std::string socket_reply;
	std::string simulationID;
	std::string currentClient;
	unsigned int lsrIndex;
	unsigned int switchIndex;
	unsigned int ipIndex;
	unsigned int appsIndex;
	unsigned int createNodeIndex;
	unsigned int clientIndex;
	unsigned int serverIndex;
	unsigned int lsrDbIndex;
	unsigned int mpls_server_port;
	std::stringstream s;
	std::string linkStart;
	std::string linkEnd;
	QPoint lastPosition;
	std::string baseip;
	unsigned int ip3;
	void updateSwitchIndices(NodeIcon* switchNode);
	std::string createCsmaNodeString(NodeIcon* switchNode);
	void createCsmaConn(NodeIcon* switchNode, std::string bw, std::string latency);
	void installApplications();
	void createNodes();
	void createPtPConns();
	void ilmAdder();
	void ftnAdder();
	void clientAppInstaller();
	void serverAppInstaller();
	bool removeDir(const QString &dirName);
	bool makeFreshDir(std::string dirName);
	void insertJunk(QTextStream* stream, unsigned int count );


	std::vector <clientAppStruct*> clientAppVector; 
	std::vector <serverAppStruct*> serverAppVector; 

	std::vector <ftnStruct*> ftnVector;
	std::vector <ilmStruct*> ilmVector;

	void newSimulation();
	void simulationDone();
	void simulationRunning();
	unsigned int currentLine;
	void lineContextMenuEvent();
	void dillyDally();
	std::string defaultBW;
	std::string defaultDelay;
	bool simStatus;
	bool ns3SimStatus;
	bool simRunning;
	QStatusBar *statusBar;
	unsigned short simProgress;
	unsigned short clientAppIndex;
	unsigned short serverAppIndex;
	unsigned int totalParse;
	unsigned int parsed;
	QAction* addlsr;
	QAction* addclient;
	QAction* addserver;
	QAction* addip;
	QAction* addSwitch;
	QAction* build;
	QAction* addline;
	QAction* remove;
	QAction* cont;
	void disableIcons();
	void updateSwitchNeighbours(std::string nodeName);
	size_t simTime;

	void updateSwitchNeighbours(std::string switchName,std::string newNode);
	std::vector <ThreadedParser*> parserVector;

	struct csmaConn
	{
		std::string switchName;
		std::string bandwidth;
		std::string delay;
	};

	std::vector <csmaConn*> csmaVector;
	bool lspMode;
	
	struct linkFail
	{
		uint32_t lineIndex;
		float failTime;
		bool resume;
		float resumeTime;
	};

	std::vector<linkFail*> linkFailVector;
	void createLinkFailures();

	struct policyStruct
	{
		uint16_t index;
		std::string nodeName;
		std::string type;
		std::string attribute;
		unsigned int attrValue;
		float weight1;
		float weight2;
	};
	std::vector <policyStruct*> policyStructVector;
	uint16_t policyIndex;
	void policyAdder();
	QTimer* simTimer;

private slots:
	void deleteLink();
	void changeLinkParameters();
	void createSimRunDialog();
	void scheduleLinkFailure();
	void updateSimTime();
	
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
};

#endif
