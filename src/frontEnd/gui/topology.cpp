#include <QtGui>
#include <QDir>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QLabel>
#include "topology.h"
#include "main_window.h"
#include <QMenu>
#include <QAction>
#include <iostream>
#include <sstream>
#include "../client/ClientSocket.h"
#include "../client/SocketException.h"
#include "add_ilm.h"
#include "add_ftn.h"
#include "client_app.h"
#include "server_app.h"
#include "linkParam.h"
#include "linkFail.h"
#include "superThreader.h" 
#include "lsrdatabase.h" 
#include "simRun.h"
#include "showGraphs.h"
#include <QProgressDialog>
#include "showClientApps.h"
#include "showServerApps.h"
#include <unistd.h>
#include <stdlib.h>
#include "lsp_buttons.h"
#include "threadedParser.h"
#include "packetRectangle.h"
#include "parseMpls.h"
#include "mplsPacket.h"

//TopologyMap::TopologyMap(QWidget *parent) : QWidget(parent)
TopologyMap::TopologyMap(QStatusBar* statusBar,QAction *startsim,QWidget *parent) : QFrame(parent)
{
	setMinimumSize(300, 400);
	setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
	setAcceptDrops(1);
	setMouseTracking(true);
	enableLine=0;

	this->animationFinished=-4;
	this->animationStarted=-1;
	this->labelColourIndex=0;
	this->simRunning=0;
	this->simStart=startsim;
	this->lspMode=0;
	this->statusBar=statusBar;
	this->simulationID="NULL";
	this->enableLine=0;
	this->deleteNode=0;
	this->vectorIndex=0;	
	this->clientAppIndex=0;	
	this->serverAppIndex=0;	
	this->linesVectorIndex=0;	
	this->connStatus=1;
	this->simStatus=0;
	this->ns3SimStatus=0;
	this->policyIndex=0;

	this->lsrDbIndex=0;

	this->defaultBW="10Mbps";
	this->defaultDelay="1ms";
	
	this->simFile="/tmp/simResults.txt";
	this->baseip="192.168";
	this->ip3=1;
	
	this->ipIndex=0;
	this->appsIndex=0;
	this->createNodeIndex=0;
	this->lsrIndex=0;
	this->switchIndex=0;
	this->clientIndex=0;
	this->serverIndex=0;
	this->socket_reply=" ";
	this->mpls_server_port = 8888 ;
	s.str("");	
	try
	{
		client_socket = new ClientSocket ( "localhost", mpls_server_port );
		this->statusBar->showMessage("Ready");
		this->simulationID=sendCommand("getNewID");
		s.str("");
		s<<"/tmp/"<<this->simulationID;
		this->makeFreshDir(s.str());
	}
	catch ( SocketException& e )
	{
		s << "ERROR: " << e.description() << " Try restarting the MPLS Server" ;
		this->connStatus=0;
		statusBar->showMessage(s.str().c_str());
		std::cout << s.str() << "\n";
		
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText("Cannot Connect To NS3 SERVER");
		msgBox.exec();
	
	}

	//populate label colour vector
	
	colourVector.push_back(Qt::gray);
	colourVector.push_back(Qt::green);
	colourVector.push_back(Qt::blue);
	colourVector.push_back(Qt::darkRed);
	colourVector.push_back(Qt::darkYellow);
	colourVector.push_back(Qt::cyan);
	colourVector.push_back(Qt::magenta);
	colourVector.push_back(Qt::darkBlue);
	colourVector.push_back(Qt::darkMagenta);
	colourVector.push_back(Qt::darkCyan);
	colourVector.push_back(Qt::red);
	colourVector.push_back(Qt::yellow);
	colourVector.push_back(Qt::darkGray);
	colourVector.push_back(Qt::lightGray);
	colourVector.push_back(Qt::black);
	colourVector.push_back(Qt::white);

	connect(simStart, SIGNAL(triggered()), this, SLOT(createSimRunDialog()));

	this->simTime=0;
	simTimer=new QTimer(this);
	connect(simTimer,SIGNAL(timeout()),this,SLOT(updateSimTime()));


	//Add colour for unlabelled packets
	labelColour* newLabCol2=new labelColour;
	newLabCol2->colour=&colourVector[labelColourIndex++];
	newLabCol2->label=0;
	this->labelColourVector.push_back(newLabCol2);
}

TopologyMap::~TopologyMap()
{
	//	boost::thread::~thread();
}
  
void TopologyMap::AddClientNode()
{
	this->s.str("");
	this->s<<"CLIENT_"<<clientIndex++;
	NodeIcon* node=new NodeIcon(this->vectorIndex,this->s.str(),"CLIENT",this);
	this->nodesVector.push_back(node);
	this->vectorIndex++;
	std::string name=this->s.str();
	this->s.str("");
	this->clientList.push_back(name);
	s<<"CREATE CLIENT "<<name;
	this->instructions.push_back(s.str());
	this->nodeCreateVector.push_back(new NodeCreator(name, s.str(), createNodeIndex++ ));
	s.str("");
	s<<name<<" Created";
	statusBar->showMessage(s.str().c_str());
}

void TopologyMap::AddServerNode()
{

	this->s.str("");
	this->s<<"SERVER_"<<serverIndex++;
	NodeIcon* node=new NodeIcon(this->vectorIndex, this->s.str(),"SERVER",this);
	this->nodesVector.push_back(node);
	this->vectorIndex++;
	std::string name=this->s.str();
	this->s.str("");
	this->s<<"CREATE SERVER "<<name;	

	this->serverList.push_back(name);
	this->instructions.push_back(s.str());
	this->nodeCreateVector.push_back(new NodeCreator(name, s.str(), createNodeIndex++ ));
	s.str("");
	s<<name<<" Created";
	statusBar->showMessage(s.str().c_str());
}

void TopologyMap::AddStandardRouterNode()
{

	this->s.str("");
	this->s<<"ROUTER_"<<ipIndex++;
	NodeIcon* node=new NodeIcon(this->vectorIndex, this->s.str(),"IP_ROUTER",this);
	this->nodesVector.push_back(node);
	this->vectorIndex++;
	std::string name=this->s.str();
	this->s.str("");
	this->s<<"CREATE ROUTER "<<name;	

	this->routerList.push_back(name);
	this->instructions.push_back(s.str());
	this->nodeCreateVector.push_back(new NodeCreator(name, s.str(), createNodeIndex++ ));
	s.str("");
	s<<name<<" Created";
	statusBar->showMessage(s.str().c_str());
}

void TopologyMap::AddMplsNode()
{

	this->s.str("");
	this->s<<"LSR_"<<lsrIndex++;
	NodeIcon* node=new NodeIcon(this->vectorIndex,this->s.str(),"MPLS",this );
	this->nodesVector.push_back(node);
	this->vectorIndex++;
	
	std::string name=this->s.str();
	this->s.str("");
	this->s<<"CREATE MPLS "<<name;	
	this->lsrList.push_back(name);

	this->instructions.push_back(s.str());
	this->nodeCreateVector.push_back(new NodeCreator(name, s.str(), createNodeIndex++ ));
	s.str("");
	s<<name<<" Created";
	statusBar->showMessage(s.str().c_str());
}

void TopologyMap::AddSwitchNode()
{
	this->s.str("");
	this->s<<"SWITCH_"<<switchIndex++;
	NodeIcon* node=new NodeIcon(this->vectorIndex,this->s.str(),"SWITCH",this );
	this->nodesVector.push_back(node);
	this->vectorIndex++;
	
	std::string name=this->s.str();
	this->switchList.push_back(name);

	csmaConn* csmaconn=new csmaConn;
	csmaconn->switchName=name;
	csmaconn->bandwidth=this->defaultBW;
	csmaconn->delay=this->defaultDelay;
	this->csmaVector.push_back(csmaconn);

	s.str("");
	s<<name<<" Created";
	statusBar->showMessage(s.str().c_str());
}


void TopologyMap::RunSimulation()
{
	this->simulationRunning();
	this->simStatus=0;
	this->simRunning=1;

	this->createNodes();
	this->createPtPConns();	

	//Create CSMA Conn
	for ( unsigned int i=0; i<this->switchList.size(); i++ )
	{
		std::string bw="NULL";
		std::string latency="NULL";
		NodeIcon *switchNode=getNodeIconFromName(switchList[i]);
			
		for ( unsigned int j=0; j<this->csmaVector.size(); j++ )
		{
			if ( csmaVector[i]->switchName == switchList[i] )
			{
				bw=csmaVector[i]->bandwidth;
				latency=csmaVector[i]->delay;
			}
		}

		if ( bw != "NULL" && latency != "NULL" )
		{
			this->createCsmaConn(switchNode, bw, latency);
		}
	}	
	this->simProgress=10;

	this->policyAdder();

	this->ftnAdder();
	this->ilmAdder();

	this->simProgress=18;
	this->serverAppInstaller();
	this->simProgress=20;
	this->clientAppInstaller();

	this->createLinkFailures();
	
	this->simProgress=25;

	SimRun *simRun=new SimRun(this->client_socket, this);
	simRun->Run();	
   // createSimRunDialog();
}

void TopologyMap::dillyDally()
{
	s.str("");
	s<<"DesTroY "<<this->simulationID;
	this->sendCommand(s.str());	
}

void TopologyMap::resetAll()
{
	this->linkStart="";
	this->linkEnd="";

}


void TopologyMap::deleteNodeLinks(std::string name)
{

	if ( this->linkStart == name )
	{
		this->linkStart="";
	}
	
	NodeIcon *suspect=getNodeIconFromName(name);

	for (unsigned int m=0; m<suspect->neighList.size(); m++ ) //Delete from other's neighLists
	{
		NodeIcon *neighNode=getNodeIconFromName(suspect->neighList[m]);
		for (unsigned int n=0; n<neighNode->neighList.size(); n++ )
		{
			if ( neighNode->neighList[n] == suspect->getName() )
			{
				neighNode->neighList.erase(neighNode->neighList.begin()+n);
				break;
			}
		}


	}
	
	for (unsigned int q=0; q<this->nodeCreateVector.size(); q++ )
	{
		if ( nodeCreateVector[q]->getNodeName() == name )
		{
			delete nodeCreateVector[q];
			nodeCreateVector[q]=NULL;
			nodeCreateVector.erase(nodeCreateVector.begin()+q);
			break;
		}
	}

	for (unsigned int k=0; k<this->serverList.size(); k++ )
	{
		if ( serverList[k] == name )
		{
			serverList.erase(serverList.begin()+k);
			break;
		}
	}

	for (unsigned int h=0; h<this->clientList.size(); h++ )
	{
		if ( clientList[h] == name )
		{
			clientList.erase(clientList.begin()+h);
			break;
		}
	}

	for (unsigned int h=0; h<this->lsrList.size(); h++ )
	{
		if ( lsrList[h] == name )
		{
			lsrList.erase(lsrList.begin()+h);
			break;
		}
	}

	for (unsigned int h=0; h<this->routerList.size(); h++ )
	{
		if ( routerList[h] == name )
		{
			routerList.erase(routerList.begin()+h);
			break;
		}
	}

	for (unsigned int p=0; p<this->serverAppVector.size(); p++ )
	{
		if ( serverAppVector[p]->nodeName == name )
		{
			delete serverAppVector[p];
			serverAppVector[p]=NULL;
			serverAppVector.erase(serverAppVector.begin()+p);
			p--;
		}
	}

	for (uint16_t z=0; z<policyStructVector.size(); z++ )
	{
		if ( policyStructVector[z]->nodeName == name )
		{
			delete policyStructVector[z];
			policyStructVector.erase(policyStructVector.begin()+z);
			z--;
		}
	}

	for (unsigned int p=0; p<this->clientAppVector.size(); p++ )
	{
		if ( clientAppVector[p]->nodeName == name || clientAppVector[p]->destServer == name )
		{
			delete clientAppVector[p];
			clientAppVector[p]=NULL;
			clientAppVector.erase(clientAppVector.begin()+p);
			p--;
		}
	}

	for (unsigned int p=0; p<this->ilmVector.size(); p++ )
	{
		if ( ilmVector[p]->nodeName == name || ilmVector[p]->nextHop == name )
		{
			delete ilmVector[p];
			ilmVector[p]=NULL;
			ilmVector.erase(ilmVector.begin()+p);
			p--;
		}
	}

	for (unsigned int p=0; p<this->ftnVector.size(); p++ )
	{
		if ( ftnVector[p]->nodeName == name || ftnVector[p]->nextHop == name )
		{
			delete ftnVector[p];
			ftnVector[p]=NULL;
			ftnVector.erase(ftnVector.begin()+p);
			p--;
		}
	}

	for (unsigned int l=0; l<this->switchList.size(); l++ )
	{
		if ( switchList[l] == name )
		{
			switchList.erase(switchList.begin()+l);
			break;
		}
	}
	
	for (unsigned int l=0; l<this->csmaVector.size(); l++ )
	{
		if ( csmaVector[l]->switchName == name )
		{
			csmaVector.erase(csmaVector.begin()+l);
			break;
		}
	}

	for (unsigned int j=0; j<this->linesVector.size(); j++ )
	{
		if ( linesVector[j]->linkStart == name || linesVector[j]->linkEnd == name )
		{
			NodeIcon *node1=getNodeIconFromName(linesVector[j]->linkStart);
			NodeIcon *node2=getNodeIconFromName(linesVector[j]->linkEnd);

			if ( node1->getType() == "SERVER" || node1->getType() == "CLIENT" )
			{
				node1->addressSet=0;
				node1->addressVector.erase(node1->addressVector.begin()+0);
			}

			if ( node2->getType() == "SERVER" || node2->getType() == "CLIENT" )
			{
				node2->addressSet=0;
				node2->addressVector.erase(node2->addressVector.begin()+0);
			}

			for ( uint32_t z=0; z<linkFailVector.size(); z++ )
			{
				if ( linkFailVector[z]->lineIndex == linesVector[j]->getIndex() )	
				{
					delete linkFailVector[z];
					linkFailVector.erase(linkFailVector.begin()+z);
					z--;
				}
			}

			delete linesVector[j];
			linesVector[j]=NULL;
			linesVector.erase(linesVector.begin()+j);
			j--;				
		}	
	}
	this->s.str("");
	this->s<<name<<" deleted";
	statusBar->showMessage(this->s.str().c_str());
}

std::string TopologyMap::getNeighIpAddress(std::string self, std::string neigh)
{

	for (unsigned int j=0; j<this->linesVector.size(); j++ )
	{
		NodeIcon* node1=this->getNodeIconFromName(linesVector[j]->linkStart);
		NodeIcon* node2=this->getNodeIconFromName(linesVector[j]->linkEnd);

		if ( node1->getType() != "SWITCH" && node2->getType() != "SWITCH" )
		{
			if ( linesVector[j]->linkStart == self && linesVector[j]->linkEnd == neigh )
			{
				return linesVector[j]->linkEndIpv4Address;
			}
			else if ( linesVector[j]->linkEnd == self && linesVector[j]->linkStart == neigh )
			{
				return linesVector[j]->linkStartIpv4Address;
			}
			else
			{
			}
		}
		else
		{
			if ( node2->getType() == "SWITCH" )
			{
				std::string sstart="NULL", sstop="NULL";
				for ( unsigned int k=0; k<node2->neighList.size(); k++)
				{
					if (node2->neighList[k] == self )
					{
						sstart=self;
					}
					else if ( node2->neighList[k] == neigh )
					{
						sstop=neigh;
					}
					else
					{}
					if ( sstart != "NULL" && sstop != "NULL " )
					{
						for (unsigned int n=0; n<this->linesVector.size(); n++ )
						{
							if ( linesVector[n]->linkStart == node2->getName() && linesVector[n]->linkEnd == neigh )
							{
								std::cout<<linesVector[n]->linkEndIpv4Address<<std::endl;
								return linesVector[n]->linkEndIpv4Address;
							}
							else if ( linesVector[n]->linkEnd == node2->getName() && linesVector[n]->linkStart == neigh )
							{
								std::cout<<linesVector[n]->linkStartIpv4Address<<std::endl;
								return linesVector[n]->linkStartIpv4Address;
							}
							else
							{
							}
						}
					}

				}


			}

			if ( node1->getType() == "SWITCH" )
			{
				std::string sstart="NULL", sstop="NULL";
				for ( unsigned int k=0; k<node1->neighList.size(); k++)
				{
					if (node1->neighList[k] == self )
					{
						sstart=self;
					}
					else if ( node1->neighList[k] == neigh )
					{
						sstop=neigh;
					}
					else
					{}
					if ( sstart != "NULL" && sstop != "NULL " )
					{
						for (unsigned int n=0; n<this->linesVector.size(); n++ )
						{
							if ( linesVector[n]->linkStart == node1->getName() && linesVector[n]->linkEnd == neigh )
							{
								std::cout<<linesVector[n]->linkEndIpv4Address<<std::endl;
								return linesVector[n]->linkEndIpv4Address;
							}
							else if ( linesVector[n]->linkEnd == node1->getName() && linesVector[n]->linkStart == neigh )
							{
								std::cout<<linesVector[n]->linkStartIpv4Address<<std::endl;
								return linesVector[n]->linkStartIpv4Address;
							}
							else
							{
							}
						}
					}

				}


			}
		}
	}
	return "NO IP";
}	

NodeIcon* TopologyMap::getNodeIconFromName(std::string name)
{

	for (unsigned int i=0; i<this->nodesVector.size(); i++ )
	{
		NodeIcon *child = dynamic_cast<NodeIcon*>(this->nodesVector[i]);

		if( child->getName() == name)
		{
			return child;
		}
	}
	return NULL;
}	


void TopologyMap::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
		if (event->source() == this) 
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
		} 
		else
		{
			event->acceptProposedAction();
		}
	}
	else 
	{
		event->ignore();
	}
}

void TopologyMap::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-dnditemdata")) 
	{
		if (event->source() == this)
		{
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else
		{
			event->acceptProposedAction();
		}
	}
	else
	{
		event->ignore();
	}
}

void TopologyMap::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-dnditemdata"))
	{
		QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QPixmap pixmap;
		QPoint offset;
		dataStream >> pixmap >> offset;

		NodeIcon *child = static_cast<NodeIcon*>(childAt( this->lastPosition ));
		NodeIcon *newIcon = new NodeIcon(child->index,child->getName(),child->getType(), this);
		
		newIcon->addressVector=child->addressVector;		
		newIcon->addressSet=child->addressSet;		
		newIcon->baseIP=child->baseIP;		
		newIcon->switchIndex=child->switchIndex;		
		newIcon->neighList=child->neighList;		
		newIcon->updateToolTip();
		
		newIcon->setPixmap(pixmap);
		newIcon->move(event->pos() - offset);

		if ( newIcon->getName()=="deleted" )
		{
			newIcon->hide();
		}
		else
		{
			newIcon->show();
			newIcon->setAttribute(Qt::WA_DeleteOnClose);
		}	

		nodesVector[child->index]=newIcon;	
			
		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else
		{
			event->acceptProposedAction();
		}
	}
	else
	{
		event->ignore();
	}

}

void TopologyMap::paintEvent(QPaintEvent *)
{
	if ( this->lspMode==1 && mplsPacketVector.size() != 0 )
	{
		this->mplsSimulation();
	}

	int startX=0, startY=0,stopX=0,stopY=0;

	QPainter painter(this);
	QPen pen(Qt::red);
	pen.setWidth(2);
	pen.setCapStyle(Qt::RoundCap);
	painter.setPen(pen);
	
	if ( this->linkStart != "" && this->linkEnd == "" )
	{

		NodeIcon* child=this->getNodeIconFromName(this->linkStart);
		startX=(child->pos().x()+child->width()/2);
		startY =(child->pos().y()+child->height()/2);
		painter.drawLine( startX, startY, mapFromGlobal(QCursor::pos()).x(),  mapFromGlobal(QCursor::pos()).y() )	;
	}

	for (unsigned int i=0; i<this->linesVector.size(); i++)
	{
		NodeIcon* childStart=this->getNodeIconFromName(linesVector[i]->linkStart);
		NodeIcon* childEnd=this->getNodeIconFromName(linesVector[i]->linkEnd);
		startX=(childStart->pos().x()+childStart->width()/2);
		stopX=(childEnd->pos().x()+childEnd->width()/2);
		startY =(childStart->pos().y()+childStart->height()/2);
		stopY =(childEnd->pos().y()+childEnd->height()/2);
		painter.drawLine( startX, startY, stopX, stopY )	;
	}

}

void TopologyMap::mouseMoveEvent(QMouseEvent * )
{
	update();
}


void TopologyMap::mousePressEvent(QMouseEvent *event)
{
	NodeIcon *child = static_cast<NodeIcon*>(childAt(event->pos()));

	if (!child)
	{
		for(size_t i = 0; i < this->linesVector.size(); i++)
		{
			NodeIcon* node1=this->getNodeIconFromName(linesVector[i]->linkStart);
			NodeIcon* node2=this->getNodeIconFromName(linesVector[i]->linkEnd);

			float startX=(node1->pos().x()+node1->width()/2);
			float startY=(node1->pos().y()+node1->width()/2);

			float stopX=(node2->pos().x()+node2->width()/2);
			float stopY=(node2->pos().y()+node2->width()/2);

			double grad=((stopY-startY)/(stopX-startX));
			float intc=(stopY-(grad*stopX));

			float thisX=event->x();
			float thisY=event->y();


			//Check if event position is on line
			int testP= int (( ((grad*thisX) + intc)- thisY));
			
			if ( ( (( stopX-startX) < 5 ) && (( stopX - startX ) > -5 )) )
			{
				testP=0;
			}

			//std::cout << " testP "<<testP<<std::endl;;

			if ( ( ( testP <=  8 )  && ( testP >= -8 ) ) && ( ( ( thisY <= stopY ) && ( thisX <= stopX ) && ( thisY >= startY ) && ( thisX >=startX ) && ( grad >=0) && ( stopY >= startY ) && ( stopX >= startX ) ) || ( (thisY >= stopY ) && ( thisY <= startY ) && ( thisX <= startX )  && ( thisX >= stopX ) && ( grad >=0 ) && ( stopY <= startY ) && ( stopX <= startX ) ) || (( thisY >= stopY ) && ( thisY <= startY ) && ( thisX <= stopX ) && ( thisX >= startX ) && ( grad <= 0 ) && (startY >= stopY) && ( startX <= stopX ) ) || ((thisY<= stopY) && ( thisY >= startY ) && ( thisX >= stopX ) && ( thisX <= startX ) && ( grad <=0 ) && ( stopY >= startY) && ( stopX <= startX )  )  ||( ( (stopY - startY ) <=2 && (stopY - startY ) >= -2) && (( ( thisX < stopX ) && ( thisX > startX )  && (stopX > startX ) ) || ( ( thisX > stopX ) && ( thisX<startX) && ( stopX < startX ) ) ) ) || ( (( stopX-startX ) <= 2 ) && ( (stopX-startX ) >= -2 ) &&   ( (( thisY < startY ) && ( thisY > stopY ) && ( stopY < startY ) ) || ( ( thisY > startY ) && ( thisY < stopY ) && ( stopY > startY)) )  )   )  )
			{
				this->lastPosition = event->pos();
				this->currentLine = this->linesVector[i]->getIndex();
				this->lineContextMenuEvent();
				return;
			}
		}
		return;
	}
		
	if (event->button()==Qt::RightButton)
	{
		return;
	}
	if (event->button()==Qt::LeftButton)
	{
	
		if ( this->enableLine == 1 )
		{
			if ( child->addressSet == 1 ){
				
			}
			else
			{
			if ( linkStart=="" )
			{
				linkStart=child->getName();
			}
			else if ( linkStart != "" && linkEnd=="" )
			{
				linkEnd=child->getName();
			
				if ( linkStart != linkEnd )
				{	
					NodeIcon* node1=this->getNodeIconFromName(linkStart);
					NodeIcon* node2=this->getNodeIconFromName(linkEnd);
					
					//Check For Attempted Repeat Connection and Stop it in its tracks
					for ( unsigned int i=0; i<node1->neighList.size(); i++)
					{
						for ( unsigned int j=0; j<node2->neighList.size(); j++)
						{

							if ( node1->neighList[i] == linkEnd || node2->neighList[j] == linkStart )
							{
								return;
							}
						}
					}					

					update();	
					this->s.str("");
					std::string linkStartIpv4Address;
					std::string linkEndIpv4Address;
					std::string subnetIP;
				
					if (node1->getType() == "SWITCH" && node2->getType() != "SWITCH" )
					{
						if ( node1->baseIP == "NULL" )
						{
							
							std::stringstream baseIpv4Add;
							baseIpv4Add<<this->baseip<<"."<<this->ip3++;
							node1->baseIP=baseIpv4Add.str();
							node1->switchIndex++;
						}			
						
						updateSwitchNeighbours(node1->getName(),node2->getName());

						std::stringstream baseIpv4Address;
						baseIpv4Address<<node1->baseIP<<"."<<node1->switchIndex++;
						node2->addressVector.push_back(baseIpv4Address.str());
						linkEndIpv4Address=baseIpv4Address.str();
						linkStartIpv4Address=node1->getName();
						baseIpv4Address.str("");
						baseIpv4Address<<node1->baseIP<<".0";
						subnetIP=baseIpv4Address.str();
						//this->s.str("");

						node1->neighList.push_back(linkEnd);			
						//node2->neighList.push_back(linkStart);			

				
					}

					else if (node2->getType() == "SWITCH" && node1->getType() != "SWITCH" )
					{
						if ( node2->baseIP == "NULL" )
						{
							std::stringstream baseIpv4Add;
							baseIpv4Add<<this->baseip<<"."<<this->ip3++;
							node2->baseIP=baseIpv4Add.str();
							node2->switchIndex++;
						}			
						
						updateSwitchNeighbours(node2->getName(),node1->getName());

						std::stringstream baseIpv4Address;
						baseIpv4Address<<node2->baseIP<<"."<<node2->switchIndex++;
						node1->addressVector.push_back(baseIpv4Address.str());
						linkStartIpv4Address=baseIpv4Address.str();
						linkEndIpv4Address=node2->getName();
						baseIpv4Address.str("");
						baseIpv4Address<<node1->baseIP<<".0";
						subnetIP=baseIpv4Address.str();
						//this->s.str("");

						//node1->neighList.push_back(linkEnd);			
						node2->neighList.push_back(linkStart);			

				
					}

				
					else if (node1->getType() == "SWITCH" && node2->getType() == "SWITCH" )
					{ // Forbid all switch-switch connections
							linkStart="NULL";
							linkEnd="NULL";
					}

					else
					{

						std::stringstream baseIpv4Address;
						baseIpv4Address<<this->baseip<<"."<<this->ip3<<"."<<"1";
						linkStartIpv4Address=baseIpv4Address.str();
						node1->addressVector.push_back(baseIpv4Address.str());
					
						baseIpv4Address.str("");
						baseIpv4Address<<this->baseip<<"."<<this->ip3<<"."<<"2";
						linkEndIpv4Address=baseIpv4Address.str();
						node2->addressVector.push_back(baseIpv4Address.str());
		
						baseIpv4Address.str("");
						baseIpv4Address<<this->baseip<<"."<<this->ip3<<"."<<"0";
						this->ip3++;	
						subnetIP=baseIpv4Address.str();

						node1->neighList.push_back(linkEnd);			
						node2->neighList.push_back(linkStart);			
					}

					if ( linkStart != "NULL" && linkEnd !="NULL" )
					{

					
						linesVector.push_back(new ConnLine(linkStart,linkEnd,subnetIP,linkStartIpv4Address ,linkEndIpv4Address,this->defaultBW,this->defaultDelay, linesVectorIndex));		
						linesVectorIndex++;

						if (node1->getType() == "CLIENT" || node1->getType()=="SERVER" )
						{
							node1->addressSet=1;
						}
						if (node2->getType() == "CLIENT" || node2->getType()=="SERVER" )
						{
							node2->addressSet=1;
						}
						node1->updateToolTip();
						node2->updateToolTip();
						this->instructions.push_back(s.str());
						s.str("");
						s<<linkStart<<" Connected to "<<linkEnd;
						
						statusBar->showMessage(s.str().c_str());
					}
				
				}
				this->resetAll();
				this->enableLine=0;
			}        
			else
			{
			}
						
		}
	}
	}
	if ( this->deleteNode == 1 ) 
	{
		this->deleteNodeLinks(child->getName());
		child->remove();
		this->deleteNode=0;
	}
	
	else
	{
	}

	this->lastPosition = event->pos();

	QPixmap pixmap = *child->pixmap();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << pixmap << QPoint(event->pos() - child->pos());

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-dnditemdata", itemData);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	drag->setHotSpot(event->pos() - child->pos());

	QPixmap tempPixmap = pixmap;
	QPainter painter;
	painter.begin(&tempPixmap);
	painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
	painter.end();

	child->setPixmap(tempPixmap);

	if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
	{	
		child->close();
	}
	else
	{
		child->show();
		child->setPixmap(pixmap);
	}
	update();
}

/*void TopologyMap::displaySimResults()
{
	SimResultWindow *resultWindow=new SimResultWindow(this->simFile);
	resultWindow->setWindowTitle("SIMULATION RESULTS");
	resultWindow->show();	
}*/

void TopologyMap::createConnection(std::string name){
	this->enableLine=1;
	this->linkStart=name;
	update();
}


void TopologyMap::configureClientApp(std::string name)
{
	clientAppDialog* clientApp= new clientAppDialog(this->serverList);
	s.str("");
	s<<"Configure Client Application : "<<name;
	clientApp->setWindowTitle(s.str().c_str());
	clientApp->exec();	
	if ( clientApp->cancelled == 1 )
	{
		return;
	}
	else
	{
		if ( clientApp->server == "" )
		{
			return;
		}
		else
		{
			clientAppStruct* newClientApp=new clientAppStruct;
			newClientApp->nodeName=name;
			newClientApp->appType=clientApp->appType;
			newClientApp->destServer=clientApp->server;
			newClientApp->destPort=clientApp->port;
			newClientApp->packetSize=clientApp->packetSize;
			newClientApp->packetNum=clientApp->numPackets;
			newClientApp->startTime=clientApp->startTime;
			newClientApp->stopTime=clientApp->stopTime;
			newClientApp->interval=clientApp->interval;
			newClientApp->index=this->clientAppIndex++;

			clientAppVector.push_back(newClientApp);
		}	
	}
}

void TopologyMap::clientAppInstaller()
{
	for ( unsigned int i=0; i<clientAppVector.size(); i++ )
	{
		NodeIcon* appserver=this->getNodeIconFromName(clientAppVector[i]->destServer);
		std::string serverIP=appserver->getMainAddress();
		if ( serverIP == "NO IP SET" )
		{
			std::cout<<"Client App returned, NO IP"<<std::endl;
		}	
		else
		{
			
			if ( clientAppVector[i]->appType == "UDP Echo" )
			{
				s.str("");
				s<<"ADD "<<clientAppVector[i]->nodeName<<" APP"<<" UDP "<<serverIP<<" "<<clientAppVector[i]->destPort<<" "<<clientAppVector[i]->startTime<<" ";
				s<<clientAppVector[i]->stopTime<<" "<<clientAppVector[i]->packetNum<<" "<<clientAppVector[i]->packetSize<<" "<<clientAppVector[i]->interval;
				std::string error=this->sendCommand(s.str());
			}
			else if ( clientAppVector[i]->appType == "Random TCP Bursts" )
			{
				s.str("");
				s<<"ADD "<<clientAppVector[i]->nodeName<<" APP"<<" BURST "<<serverIP<<" "<<clientAppVector[i]->destPort<<" "<<clientAppVector[i]->startTime<<" ";
				s<<clientAppVector[i]->stopTime<<" "<<clientAppVector[i]->packetNum<<" "<<clientAppVector[i]->packetSize<<" "<<clientAppVector[i]->interval;
				s<<clientAppVector[i]->stopTime;
				std::string error=this->sendCommand(s.str());
			}
			else
			{
			}
		}

	}
}

void TopologyMap::configureServerApp(std::string name)
{
	serverAppDialog* serverApp= new serverAppDialog;
	
	s.str("");
	s<<"Configure Server Application : "<<name;
	serverApp->setWindowTitle(s.str().c_str());
	serverApp->exec();
	if ( serverApp->cancelled == 1 )
	{
		return;
	}
	else
	{	
		serverAppStruct *newServerApp=new serverAppStruct;
		newServerApp->nodeName=name;
		newServerApp->appType=serverApp->appType;
		newServerApp->port=serverApp->port;
		newServerApp->startTime=serverApp->startTime;
		newServerApp->stopTime=serverApp->stopTime;
		newServerApp->index=serverAppIndex++;
	
		serverAppVector.push_back(newServerApp);
	}
}


void TopologyMap::serverAppInstaller()
{
	for ( unsigned int i=0; i<serverAppVector.size();i++ )
	{
		if ( serverAppVector[i]->appType == "UDP Echo Server" )
		{
			s.str("");
			s<<"ADD "<<serverAppVector[i]->nodeName<<" APP"<<" UDP "<<serverAppVector[i]->port<<" "<<serverAppVector[i]->startTime<<" "<<serverAppVector[i]->stopTime;
			std::string error=this->sendCommand(s.str());
		}
		else if ( serverAppVector[i]->appType == "TCP Sink" )
		{
			s.str("");
			s<<"ADD "<<serverAppVector[i]->nodeName<<" APP"<<" TCP "<<serverAppVector[i]->port<<" "<<serverAppVector[i]->startTime<<" "<<serverAppVector[i]->stopTime;
			std::string error=this->sendCommand(s.str());

		}
		else
		{

		}
	}
}

void TopologyMap::popLabel(std::string name)
{
	unsigned int label = QInputDialog::getInteger(this, "Pop Label","Enter Label to pop out");

	if ( label != 0 )
	{
		s.str("");
		ilmStruct* newIlm=new ilmStruct;
		newIlm->incomingLabel=label;
		newIlm->outgoingLabel=0;
		newIlm->outgoingLabel2=0;
		newIlm->nodeName=name;
		newIlm->operation="POP";
		newIlm->nextHop="NULL";
		newIlm->nextHop2="NULL";
		newIlm->fastreroute=0;
		newIlm->policyIndex=0;
		newIlm->index=this->lsrDbIndex++;
		this->ilmVector.push_back(newIlm);
	}
		
}

void TopologyMap::addFTN(std::string name)
{
	NodeIcon *node = getNodeIconFromName(name);

	AddFtnDialog *ftnDialog=new AddFtnDialog(node->neighList);
	s.str("");
	s<<"Add FEC to NHLFE : "<<name;
	ftnDialog->setWindowTitle(s.str().c_str());
	ftnDialog->exec();

	if ( ftnDialog->cancelled == 1 )
	{
		return;
	}	

	if ( ftnDialog->fastreroute == 1 )
	{
		ftnStruct* newFtn=new ftnStruct;
		newFtn->nodeName=name;
		newFtn->label=ftnDialog->outLabel;
		newFtn->sourceIP=ftnDialog->sourceAddr;
		newFtn->destinationIP=ftnDialog->destAddr;

		newFtn->destinationPort=ftnDialog->destPort;
		newFtn->sourcePort=ftnDialog->srcPort;
		newFtn->fec=ftnDialog->fec;

		newFtn->nextHop=ftnDialog->nextHopNode;
		newFtn->index=this->lsrDbIndex++;
		newFtn->label2=ftnDialog->outLabel2;
		newFtn->nextHop2=ftnDialog->nextHopNode2;
		newFtn->fastreroute=1;
		newFtn->policyIndex=policyIndex;
		ftnVector.push_back(newFtn);

		std::cout<<ftnDialog->policy<<std::endl;

		policyStruct* ps=new policyStruct;
		ps->index=policyIndex++;
		ps->nodeName=name;
		ps->type=ftnDialog->policy;
		ps->attribute=ftnDialog->policyAttribute;
		ps->attrValue=ftnDialog->policyAttributeValue;
		ps->weight1=ftnDialog->weight1;
		ps->weight2=ftnDialog->weight2;
		policyStructVector.push_back(ps);
	}

	else
	{
		ftnStruct* newFtn=new ftnStruct;
		newFtn->nodeName=name;
		newFtn->label=ftnDialog->outLabel;
		newFtn->sourceIP=ftnDialog->sourceAddr;
		newFtn->label2=0;
		newFtn->nextHop2="NULL";
		newFtn->fastreroute=0;
		newFtn->destinationIP=ftnDialog->destAddr;
		newFtn->nextHop=ftnDialog->nextHopNode;
		newFtn->policyIndex=0;
		newFtn->index=this->lsrDbIndex++;

		newFtn->sourcePort=ftnDialog->srcPort;
		newFtn->destinationPort=ftnDialog->destPort;
		newFtn->fec=ftnDialog->fec;
		ftnVector.push_back(newFtn);
		
	}	
}

void TopologyMap::ftnAdder()
{
	for ( unsigned int i=0; i<this->ftnVector.size(); i++ )
	{
		s.str("");
		if ( ftnVector[i]->fastreroute != 1 )
		{
			std::string neighIP=getNeighIpAddress(ftnVector[i]->nodeName,ftnVector[i]->nextHop);

			s<<"ADD "<<ftnVector[i]->nodeName<<" FTN "<<ftnVector[i]->fec<<" "<<ftnVector[i]->sourceIP<<" "<<ftnVector[i]->destinationIP<<" "<<ftnVector[i]->sourcePort<<" "<<ftnVector[i]->destinationPort<<" "<<neighIP<<" "<<ftnVector[i]->label;
			std::string error=this->sendCommand(s.str());
		}
		else
		{
			s.str("");
			std::string neigh1IP=getNeighIpAddress(ftnVector[i]->nodeName,ftnVector[i]->nextHop);
			std::string neigh2IP=getNeighIpAddress(ftnVector[i]->nodeName,ftnVector[i]->nextHop2);
			s<<"ADD "<<ftnVector[i]->nodeName<<" FASTREROUTE FTN "<<ftnVector[i]->sourceIP<<" "<<ftnVector[i]->destinationIP<<" "<<neigh1IP<<" "<<ftnVector[i]->label<<" "<<neigh2IP<<" "<<ftnVector[i]->label2<<" "<<ftnVector[i]->policyIndex;
			std::string error=this->sendCommand(s.str());
		}
	}
}

void TopologyMap::addILM(std::string name)
{
	NodeIcon *node = getNodeIconFromName(name);
	AddIlmWindow *ilmWindow=new AddIlmWindow(node->neighList);
	s.str("");
	s<<"Add Incoming Label Map : "<<name;
	ilmWindow->setWindowTitle(s.str().c_str());
	ilmWindow->exec();

	if ( ilmWindow->cancelled == 1 )
	{
		return;
	}	

	if (ilmWindow->fastreroute==1)
	{
		ilmStruct* newIlm=new ilmStruct;
		newIlm->incomingLabel=ilmWindow->label1;
		newIlm->outgoingLabel=ilmWindow->label2;
		newIlm->outgoingLabel2=ilmWindow->label3;
		newIlm->nodeName=name;
		newIlm->operation="SWAP";
		newIlm->fastreroute=1;
		newIlm->nextHop=ilmWindow->nextHopNode;
		newIlm->nextHop2=ilmWindow->nextHopNode2;
		newIlm->policyIndex=policyIndex;
		newIlm->index=this->lsrDbIndex++;
		this->ilmVector.push_back(newIlm);

		//Add Policy
		std::cout<<ilmWindow->policy<<std::endl;
		policyStruct* ps=new policyStruct;
		ps->index=policyIndex++;
		ps->nodeName=name;
		ps->type=ilmWindow->policy;
		ps->attribute=ilmWindow->policyAttribute;
		ps->attrValue=ilmWindow->policyAttributeValue;
		ps->weight1=ilmWindow->weight1;
		ps->weight2=ilmWindow->weight2;
		policyStructVector.push_back(ps);

	} 

	else
	{
		ilmStruct* newIlm=new ilmStruct;
		newIlm->incomingLabel=ilmWindow->label1;
		newIlm->outgoingLabel=ilmWindow->label2;
		newIlm->outgoingLabel2=0;
		newIlm->nodeName=name;
		newIlm->operation="SWAP";
		newIlm->fastreroute=0;
		newIlm->nextHop=ilmWindow->nextHopNode;
		newIlm->nextHop2="NULL";
		newIlm->policyIndex=0;
		newIlm->index=this->lsrDbIndex++;
		this->ilmVector.push_back(newIlm);
	}
}

void TopologyMap::policyAdder()
{
	for (uint32_t i=0; i<policyStructVector.size(); i++ )
	{
		if ( policyStructVector[i]->type == "DEFAULT" )
		{//ADD LSR0 POLICY DEFAULT BYTES 10000
			s.str("");
			s<<"ADD "<<policyStructVector[i]->nodeName<<" POLICY DEFAULT "<<policyStructVector[i]->index<<" "<<policyStructVector[i]->attribute<<" "<<policyStructVector[i]->attrValue;
			std::string error=this->sendCommand(s.str());
			std::cout<<s.str()<<std::endl;
		}
		else if ( policyStructVector[i]->type == "WEIGHTED" )
		{
			s.str("");
			s<<"ADD "<<policyStructVector[i]->nodeName<<" POLICY WEIGHTED "<<policyStructVector[i]->index<<" "<<policyStructVector[i]->attribute<<" "<<policyStructVector[i]->attrValue<<" "<<policyStructVector[i]->weight1<<" "<<policyStructVector[i]->weight2;
			std::string error=this->sendCommand(s.str());
			std::cout<<s.str()<<std::endl;
		}
		else{ std::cout<<"no such policy"<<std::endl;  }

	}
}		

void TopologyMap::ilmAdder()
{
	for ( unsigned int i=0; i<this->ilmVector.size(); i++ )
	{
		if ( ilmVector[i]->fastreroute !=1 )
		{

			s.str("");
			if ( ilmVector[i]->operation == "SWAP" )
			{
				std::string neighIP=getNeighIpAddress(ilmVector[i]->nodeName,ilmVector[i]->nextHop);
				s<<"ADD "<<ilmVector[i]->nodeName<<" ILM "<<ilmVector[i]->incomingLabel<<" "<<ilmVector[i]->outgoingLabel<<" "<<neighIP;	
				std::string error=this->sendCommand(s.str());
			}
			else if ( ilmVector[i]->operation == "POP" )
			{
				s<<"ADD "<<ilmVector[i]->nodeName<<" POP "<<ilmVector[i]->incomingLabel;
				std::string error=this->sendCommand(s.str());
			}
			else
			{
			}
		}
		else
		{
			
			std::string neigh1IP=getNeighIpAddress(ilmVector[i]->nodeName,ilmVector[i]->nextHop);
			std::string neigh2IP=getNeighIpAddress(ilmVector[i]->nodeName,ilmVector[i]->nextHop2);
			s.str("");
			s<<"ADD "<<ilmVector[i]->nodeName<<" FASTREROUTE ILM "<<ilmVector[i]->incomingLabel<<" "<<ilmVector[i]->outgoingLabel<<" "<<neigh1IP<<" "<<ilmVector[i]->outgoingLabel2<<" "<<neigh2IP<<" "<<ilmVector[i]->policyIndex;
			std::string error=this->sendCommand(s.str());
		}
		
	}

}

std::string TopologyMap::sendCommand(std::string command)
{
	if ( this->connStatus == 1 )
		{	
			s.str("");
			if ( command != "getNewID" )
			{
				s<<this->simulationID<<" "<<command;
				command=s.str();				
			}

			try
			{
				client_socket->transmit(command);
				client_socket->receive(socket_reply);
			}
			catch ( SocketException& e) 
			{
				this->connStatus = 0;
			}
			this->status=socket_reply;
			return socket_reply;
		}
	return "CONNECTION DOWN";
}

void TopologyMap::updateSwitchIndices(NodeIcon* switchNode)
{
	for (unsigned int j=0; j<switchNode->neighList.size(); j++ )
	{
		NodeIcon *neighNode=getNodeIconFromName(switchNode->neighList[j]);
		if ( neighNode->getType() == "SWITCH" )
		{
			neighNode->switchIndex=switchNode->switchIndex;
		}

	}



}

std::string TopologyMap::createCsmaNodeString(NodeIcon* switchNode)
{
	std::stringstream neighs;

	for (unsigned int j=0; j<switchNode->neighList.size(); j++ )
	{
		neighs<<switchNode->neighList[j]<<" ";
	}
	return neighs.str();
}

void TopologyMap::createCsmaConn(NodeIcon* switchNode, std::string bw, std::string latency)
{
	s.str("");
	s<<"CREATECSMA "<<switchNode->baseIP<<".0 "<<bw<<" "<<latency<<" ";
	s<<this->createCsmaNodeString(switchNode);
	sendCommand(s.str());
	s.str("");
}

void TopologyMap::createNodes()
{
	for (unsigned int j=0; j<this->nodeCreateVector.size(); j++ )
	{
		this->sendCommand(this->nodeCreateVector[j]->getCommand());
	}
}

void TopologyMap::createPtPConns()
{
	for (unsigned int j=0; j<this->linesVector.size(); j++ )
	{
		s.str("");
		NodeIcon *node1=getNodeIconFromName(linesVector[j]->linkStart);
		NodeIcon *node2=getNodeIconFromName(linesVector[j]->linkEnd);
		
		if ( node1->getType() != "SWITCH" && node2->getType() != "SWITCH" )
		{
			this->s<<"CONNECT "<<linesVector[j]->linkStart<<" "<<linesVector[j]->linkEnd<<" "<<linesVector[j]->subnet<<" "<<linesVector[j]->linkBandWidth<<" "<<linesVector[j]->linkDelay<<" "<<linesVector[j]->getIndex();
			this->sendCommand(s.str());	
			this->instructions.push_back(s.str());
		}
	}
}


bool TopologyMap::removeDir(const QString &dirName)
{
	bool result = true;
	QDir dir(dirName);
 
	if (dir.exists(dirName))
	 {
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
		{
			if (info.isDir())
			{
				result = removeDir(info.absoluteFilePath());
			}
			else
			{
				result = QFile::remove(info.absoluteFilePath());
			}
	 
			if (!result)
			{
				return result;
			}
		}
		result = dir.rmdir(dirName);
	}
 
	return result;
}

bool TopologyMap::makeFreshDir(std::string dirName)
{

	QDir dirManager(dirName.c_str());
	if ( QDir(dirName.c_str()).exists() )
	{
		this->removeDir(dirName.c_str());
	}

	bool result=QDir().mkdir(dirName.c_str());
	return result;
}

void TopologyMap::parseResult()
{
	this->totalParse=serverList.size()+clientList.size();

	if ( this->ns3SimStatus !=1 )
	{
		return;
	}
	for ( unsigned int k=0; k<this->serverList.size(); k++)
	{
		//this->parseResult(serverList[k]);
		ThreadedParser *parser=new ThreadedParser(serverList[k],this);
		parser->Run();
		parserVector.push_back(parser);
	}

	for ( unsigned int l=0; l<this->clientList.size(); l++)
	{
		ThreadedParser *parser=new ThreadedParser(clientList[l],this);
		parser->Run();
		parserVector.push_back(parser);
	}

	MplsParser* newP=new MplsParser("/tmp/MPLS_SERVER", this);
	newP->Run();
}

std::string TopologyMap::getNodeFromIP(std::string IPaddress )
{

	for (unsigned int i=0; i<serverList.size(); i++ )
	{		
		NodeIcon *Node=getNodeIconFromName(serverList[i]);
		if (Node->getMainAddress()==IPaddress )
		{
			return Node->getName();
		}
	}

	for (unsigned int i=0; i<clientList.size(); i++ )
	{		
		NodeIcon *Node=getNodeIconFromName(clientList[i]);
		if (Node->getMainAddress()==IPaddress )
		{
			return Node->getName();
		}
	}

	for (unsigned int i=0; i<routerList.size(); i++ )
	{		
		NodeIcon *Node=getNodeIconFromName(routerList[i]);
		for ( unsigned int j=0; j<Node->addressVector.size(); j++ )
		{
			if ( Node->addressVector[j]==IPaddress)
			{
				return Node->getName();
			}

		}
	}

	for (unsigned int i=0; i<lsrList.size(); i++ )
	{		
		NodeIcon *Node=getNodeIconFromName(lsrList[i]);
		for ( unsigned int j=0; j<Node->addressVector.size(); j++ )
		{
			if ( Node->addressVector[j]==IPaddress)
			{
				return Node->getName();
			}

		}
	}

	//std::cout << "returning NULL for 
	return "NULL";	
}

void TopologyMap::showGraphs(std::string nodeName)
{
	ShowGraphs* h=new ShowGraphs(nodeName,&(this->graphsVector));
	h->exec();
}

void TopologyMap::newSimulation()
{
	stopMplsSimulation();
	for ( size_t i=0; i<mplsPacketVector.size(); i++ )
	{
		delete mplsPacketVector[i];
		mplsPacketVector.erase(mplsPacketVector.begin()+i);
		i--;
	}
	for ( size_t i=0; i<graphsVector.size(); i++ )
	{
		delete graphsVector[i];
		graphsVector.erase(graphsVector.begin()+i);
		i--;
	}
	this->simStatus=0;
	this->ns3SimStatus=0;
	this->simulationID=sendCommand("getNewID");
	s.str("");
	s<<"/tmp/"<<this->simulationID;
	this->makeFreshDir(s.str());
}

void TopologyMap::simulationRunning()
{
	for (unsigned int i=0; i<nodesVector.size(); i++ )
	{		
		nodesVector[i]->activeSimulation();
	}
}

void TopologyMap::simulationDone()
{
	for (unsigned int i=0; i<nodesVector.size(); i++ )
	{		
		nodesVector[i]->postSimulation();
	}
	this->statusBar->showMessage("SIMULATION COMPLETE");
}

void TopologyMap::continueSimulation()
{
	this->newSimulation();

	for (unsigned int i=0; i<nodesVector.size(); i++ )
	{		
		nodesVector[i]->preSimulation();
	}
}

void TopologyMap::lineContextMenuEvent()
{
	QMenu *menu=new QMenu(this);
	QAction *changeParams=new QAction(tr("&Change Link &Parameters"), this);
	QAction *setFail=new QAction(tr("&Schedule Link Failure"), this);
	QAction *deleteLine=new QAction(tr("&Delete Connection"), this);

	if ( this->simRunning == 1 || this->simStatus == 1 )	
	{
		changeParams->setDisabled(1);
		setFail->setDisabled(1);
		deleteLine->setDisabled(1);
	}

	connect(changeParams, SIGNAL(triggered()), this, SLOT(changeLinkParameters()));
	connect(setFail, SIGNAL(triggered()), this, SLOT(scheduleLinkFailure()));
	connect(deleteLine, SIGNAL(triggered()), this, SLOT(deleteLink()));
	menu->addAction(changeParams);
	menu->addAction(setFail);
	menu->addAction(deleteLine);
	menu->exec(QCursor::pos());
}

void TopologyMap::changeLinkParameters()
{

	for ( unsigned int i=0; i<linesVector.size(); i++)
	{
		if ( linesVector[i]->getIndex() == this->currentLine)
		{
			std::cout<<linesVector[i]->getIndex()<<std::endl;
			
			if ( (getNodeIconFromName(linesVector[i]->linkStart))->getType() == "SWITCH" || (getNodeIconFromName(linesVector[i]->linkEnd))->getType() == "SWITCH"  )
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("NOTICE");
				msgBox.setText("CSMA Link Parameters Can Changed Only at the Switch");
				msgBox.exec();
				return;
			}
			
			else
			{
				linkDialog* linkApp= new linkDialog(&linesVector[i]->linkBandWidth,&linesVector[i]->linkDelay);
				s.str("");
				s<<"Configure Link Parameters : ";
				linkApp->setWindowTitle(s.str().c_str());
				linkApp->exec();
				delete linkApp;
				linkApp=NULL;
			}
		}	
	}
	
}
void TopologyMap::scheduleLinkFailure()
{

	for ( unsigned int i=0; i<linesVector.size(); i++)
	{
		if ( linesVector[i]->getIndex()==this->currentLine)
		{
			
			if ( (getNodeIconFromName(linesVector[i]->linkStart))->getType() == "SWITCH" || (getNodeIconFromName(linesVector[i]->linkEnd))->getType() == "SWITCH"  )
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("NOTICE");
				msgBox.setText("CSMA Link Parameters Can Changed Only at the Switch");
				msgBox.exec();
				return;
			}
			
			else
			{
				LinkFail* linkf= new LinkFail();
				linkf->exec();

				if ( linkf->cancelled == 1 )
				{
					return;
					delete linkf;
					linkf=NULL;
				}

				linkFail *lf= new linkFail;
				lf->lineIndex=linesVector[i]->getIndex();
				lf->failTime=linkf->failTime;
				lf->resume=linkf->resume;
				lf->resumeTime=linkf->resumeTime;

				linkFailVector.push_back(lf);
				std::cout<<"pushed to fail vector"<<std::endl;

				s.str("");
				
				delete linkf;
				linkf=NULL;
			}
		}	
	}
	
}

void TopologyMap::deleteLink()
{
	for ( unsigned int i=0; i<linesVector.size(); i++)
	{
		if ( linesVector[i]->getIndex()==this->currentLine)
		{
			NodeIcon *startNode=getNodeIconFromName(linesVector[i]->linkStart);
			NodeIcon *endNode=getNodeIconFromName(linesVector[i]->linkEnd);
			if ( startNode->getType() == "SERVER" || startNode->getType() == "CLIENT" )
			{
				startNode->addressSet=0;
			}
			if ( endNode->getType() == "SERVER" || endNode->getType() == "CLIENT" )
			{
				endNode->addressSet=0;
			}

			for ( uint32_t z=0; z<linkFailVector.size(); z++ )
			{
				if ( linkFailVector[z]->lineIndex == linesVector[i]->getIndex() )	
				{
					delete linkFailVector[z];
					linkFailVector.erase(linkFailVector.begin()+z);
					z--;
				}
			}

			for (unsigned int m=0; m<startNode->neighList.size(); m++ ) //Delete from other's neighLists
			{

				if ( startNode->neighList[m] == endNode->getName() )
				{
					startNode->neighList.erase(startNode->neighList.begin()+m);
				}
				for (unsigned int o=0; o<startNode->addressVector.size(); o++ ) 
				{
					if ( linesVector[i]->linkStartIpv4Address == startNode->addressVector[o] )
					{
						startNode->addressVector.erase(startNode->addressVector.begin()+o);
						startNode->updateToolTip();
					}
				}
			}

			for (unsigned int n=0; n<endNode->neighList.size(); n++ ) //Delete from other's neighLists
			{
				if ( endNode->neighList[n] == startNode->getName() )
				{
					endNode->neighList.erase(endNode->neighList.begin()+n);
				}
				for (unsigned int o=0; o<endNode->addressVector.size(); o++ ) 
				{
					if ( linesVector[i]->linkEndIpv4Address == endNode->addressVector[o] )
					{
						endNode->addressVector.erase(endNode->addressVector.begin()+o);
						endNode->updateToolTip();
					}
				}
			}
			s.str("");
			s<<linesVector[i]->linkStart<<" to "<<linesVector[i]->linkEnd<<" link Deleted";
			statusBar->showMessage(s.str().c_str());
			delete linesVector[i];	
			linesVector[i]=NULL;
			linesVector.erase(linesVector.begin()+i);
			break;
		}	
	}
}

void TopologyMap::selfDestruct()
{
	QMessageBox msgBox;
	msgBox.setText("This operation will clear the current topology.");
	msgBox.setInformativeText("Do you wish to continue?");
	msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	
	int ret = msgBox.exec();

	switch (ret)
	{
		case QMessageBox::Yes :
				for ( unsigned int i=0; i<nodesVector.size(); i++)

				{
					this->deleteNodeLinks(nodesVector[i]->getName());
					delete nodesVector[i];
					nodesVector[i]=NULL;
					nodesVector.erase(nodesVector.begin()+i);
					i--;
				}

				this->ip3=1;
				this->ipIndex=0;
				this->appsIndex=0;
				this->createNodeIndex=0;
				this->lsrIndex=0;
				this->switchIndex=0;
				this->clientIndex=0;
				this->serverIndex=0;
				this->enableLine=0;
				this->deleteNode=0;
				this->vectorIndex=0;	
				this->lsrDbIndex=0;
				this->ns3SimStatus=0;
				this->simStatus=0;
				this->linesVectorIndex=0;	
				this->status="Ready";
				update();
				this->newSimulation();
				break;
		
		default:
			return;
			break;
	}
}

void TopologyMap::showLsrDatabase(std::string nodeName)
{
	
	LsrDB *lsrNodeDB=new LsrDB(nodeName,&(this->ilmVector), &(this->ftnVector));
	lsrNodeDB->exec();
	lsrNodeDB=NULL;
}

std::string TopologyMap::getSimID()
{
	return this->simulationID;

}
void TopologyMap::setSimProgress(unsigned short p)
{
	this->simProgress=p;
}
void TopologyMap::setSimRunning(bool p)
{
	this->simRunning=p;
}

void TopologyMap::setSimStatus(bool p)
{
	this->simStatus=p;
}

void TopologyMap::setNs3SimStatus(bool p)
{
	this->ns3SimStatus=p;
}

void TopologyMap::createSimRunDialog()
{
	//this->disableIcons();
	this->setAcceptDrops(0);
	this->statusBar->showMessage("Parsing Results");
	this->RunSimulation();
    

	/*QVBoxLayout* layout = new QVBoxLayout;
	QWidget*  win = new QWidget;
	QProgressDialog *progress=new QProgressDialog();
	progress->setRange(0,100);
	progress->setLabelText("dafaq");
	progress->setWindowModality(Qt::WindowModal);
	layout->addWidget(progress,Qt::AlignCenter);
	win->setLayout(layout);
	win->resize(300,100);

	 

	while ( this->simStatus !=1 )
	{
		//progress->setValue(this->simProgress);
		//progress->setLabelText((this->status).c_str());
		//win->show();
		//if (progress->wasCanceled())
		//{	 break;}
		//usleep(100000);
	}

	for (int i = 0; i < 100; i++)
	{
	//set progress value.
	progress->setValue(i);
	//if user click cancel button of dialog.
	if (progress->wasCanceled())
		 break;
	}
 
	progress->setValue(100);
	win->show();
 */
	this->simulationDone();
}

void TopologyMap::incrementParsed()
{
	if ( this->parsed>=this->totalParse)
	{
		this->simStatus=1;
		this->simulationDone();
	}
	this->parsed++;
	this->simProgress=this->simProgress+int((25*this->parsed/this->totalParse)+0.5);
}

void TopologyMap::disableIcons()
{
	addlsr->setDisabled(1);
	addclient->setDisabled(1);
	addserver->setDisabled(1);
	addip->setDisabled(1); 
	addSwitch->setDisabled(1);
	build->setDisabled(1);
	addline->setDisabled(1);
	remove->setDisabled(1);
	cont->setDisabled(0);
	simStart->setDisabled(1);
}

void TopologyMap::initializeButtons(QAction* addlsr, QAction* addclient, QAction* addserver,QAction*	addip,QAction* addSwitch,QAction*		 build,QAction * addline, QAction* remove, QAction* cont, QAction* startsim)
{
	
	this->addlsr=addlsr;
	this->addclient=addclient;
	this->addserver=addserver;
	this->addip=addip; 
	this->addSwitch=addSwitch;
	this->build=build;
	this->addline=addline;
	this->remove=remove;
	this->simStart=startsim;
	this->cont=cont;
	connect(simStart, SIGNAL(triggered()), this, SLOT(createSimRunDialog()));
}

void TopologyMap::showInstalledClientApps(std::string nodeName)
{

	ClientAppStore *clientAppDB=new ClientAppStore(nodeName,&(this->clientAppVector));
	clientAppDB->exec();
	clientAppDB=NULL;
}

void TopologyMap::showInstalledServerApps(std::string nodeName)
{

	ServerAppStore *serverAppDB=new ServerAppStore(nodeName,&(this->serverAppVector));
	serverAppDB->exec();
	serverAppDB=NULL;
}

void TopologyMap::updateSwitchNeighbours(std::string nodeName)
{
	NodeIcon *switchNode=getNodeIconFromName(nodeName);
	for ( unsigned int i=0; i<switchNode->neighList.size(); i++)
	{
		NodeIcon *neighNode=getNodeIconFromName(switchNode->neighList[i]);
		{
			for ( unsigned int m=0; m<switchNode->neighList.size(); m++)
			{
				bool exists=0;
				for ( unsigned int k=0; k<neighNode->neighList.size(); k++ )
				{
					if ( neighNode->neighList[k] == switchNode->neighList[m] )
					{
						exists=1;
						break;
					}		
				}
				if (exists != 1 )
				{
					neighNode->neighList.push_back(switchNode->neighList[m]);
				}
			}
			

		}
	}

}

void TopologyMap::updateSwitchNeighbours(std::string switchName,std::string newNodeName)
{
	NodeIcon *switchNode=getNodeIconFromName(switchName);
	NodeIcon *newNode=getNodeIconFromName(newNodeName);

	for ( unsigned int i=0; i<switchNode->neighList.size(); i++)
	{
		NodeIcon* neighNode=getNodeIconFromName(switchNode->neighList[i]);
		neighNode->neighList.push_back(newNodeName);	
		newNode->neighList.push_back(switchNode->neighList[i]);	
	}
}

void TopologyMap::changeCSMALinkParams(std::string switchName)
{
	for ( unsigned int i=0; i<csmaVector.size(); i++ )
	{
		if ( csmaVector[i]->switchName == switchName )
		{
			linkDialog* linkApp= new linkDialog(&csmaVector[i]->bandwidth,&csmaVector[i]->delay);
			s.str("");
			s<<"Configure CSMA Link Parameters : "<<switchName;
			linkApp->setWindowTitle(s.str().c_str());
			linkApp->exec();

			break;
		}

	}

}

bool TopologyMap::terminateNS3()
{
	QMessageBox msgBox;
	msgBox.setText("This operation will terminate the application and exit");
	msgBox.setInformativeText("Are You Sure You Want To Exit?");
	msgBox.setWindowTitle("Quit Mpls Simulator");
	msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	
	int ret = msgBox.exec();

	switch (ret)
	{
		case QMessageBox::Yes :
			if ( this->connStatus ==1 )
			{
				client_socket->transmit("11241ExitAndQuit");
			}
			return 1;
			break;

		default :
			return 0;
			break;
	}
}

void TopologyMap::showLSP()
{
	this->lspMode=1;
	unsigned int lspIndex=0;
	for ( unsigned int i=0; i < ftnVector.size(); i++ )
	{
		NodeIcon *lsrNode=getNodeIconFromName(ftnVector[i]->nodeName);
		NodeIcon *neighNode=getNodeIconFromName(ftnVector[i]->nextHop);

		int startX=(lsrNode->pos().x()+lsrNode->width()/2);
		int startY =(lsrNode->pos().y()+lsrNode->height()/2);

		int stopX=(neighNode->pos().x()+neighNode->height()/2);
		int stopY=(neighNode->pos().y()+neighNode->height()/2);

		unsigned int j=0;
		for ( j=0; j < lspVector.size(); j++ )
		{
			if ( lspVector[i]->label == ftnVector[i]->label )
			{
				break;
			}
		}

		if ( j == ( lspVector.size() ) )
		{
			//Get Nodes' current position and next hop current position
			//
			labelColour* newLabCol=new labelColour;
			newLabCol->colour=&colourVector[labelColourIndex++];
			newLabCol->label=ftnVector[i]->label;
			labelColourVector.push_back(newLabCol);

			lsp* newLsp= new lsp; 
			newLsp->label=(ftnVector[i]->label);	
			newLsp->index=lspIndex++;

			line* newLine=new line;
			newLine->startX=startX;
			newLine->startY=startY;
			newLine->stopX=stopX;
			newLine->stopY=stopY;
			newLine->label=ftnVector[i]->label;
			
			newLsp->linesVector.push_back(newLine);
			
			std::string neighName=ftnVector[i]->nextHop;	
			unsigned int currentLabel = ftnVector[i]->label;
			for ( unsigned int k=0; k<ilmVector.size(); k++ )
			{
				if ( ( ilmVector[k]->nodeName == neighName ) &&  (ilmVector[k]->incomingLabel == currentLabel ) )
				{
					if ( ilmVector[k]->operation != "POP" )
					{
						NodeIcon *currNode=getNodeIconFromName(ilmVector[k]->nodeName);
						NodeIcon *nextHop=getNodeIconFromName(ilmVector[k]->nextHop);

						int startX2=(currNode->pos().x()+currNode->width()/2);
						int startY2 =(currNode->pos().y()+currNode->height()/2);

						int stopX2=(nextHop->pos().x()+nextHop->height()/2);
						int stopY2=(nextHop->pos().y()+nextHop->height()/2);

						currentLabel=ilmVector[k]->outgoingLabel;
						neighName=ilmVector[k]->nextHop;
						if ( ilmVector[k]->incomingLabel != ilmVector[k]->outgoingLabel )
						{
							labelColour* newLabCol2=new labelColour;
							newLabCol2->colour=&colourVector[labelColourIndex++];
							newLabCol2->label=ilmVector[k]->outgoingLabel;
							labelColourVector.push_back(newLabCol2);
						}

						line* newLine2=new line;
						newLine2->startX=startX2;
						newLine2->startY=startY2;
						newLine2->stopX=stopX2;
						newLine2->stopY=stopY2;
						newLine2->label=currentLabel;

						newLsp->linesVector.push_back(newLine2);
						k=0;						
						
					}
					else
					{
						break;
					}

				}
			}
			lspVector.push_back(newLsp);
		}
	}

	if ( lspVector.size() != 0 )
	{
		LspButtons *lspPopup=new LspButtons(lspVector.size());
		lspPopup->exec();
		
		if ( lspPopup->cancelled==1 )
		{
			delete lspPopup;
			return;
		}

		for ( unsigned int m=0; m<lspVector.size(); m++ )
		{
			PacketRectangle *newRect=new PacketRectangle(this, &(lspVector[m]->linesVector), 20);
			packetRectangleVector.push_back(newRect);

			PacketRectangle *newRect2=new PacketRectangle(this, &(lspVector[m]->linesVector), 0);
			packetRectangleVector.push_back(newRect2);

			//newRect->Run();
			/*if ( lspVector[m]->index == lspPopup->checkedButton )
			{
				this->packetLinesVector=lspVector[m]->linesVector;
				std::cout<<"packet vec size "<<packetLinesVector.size()<<std::endl;
				if ( lspVector[m]->linesVector.size() != 0 )
				{
					this->currentX=lspVector[m]->linesVector[0]->startX;
					this->lineIndex=0;
				}	
				else
				{
					this->currentX=0;
				}
		
				timer=new QTimer(this);
				connect(timer,SIGNAL(timeout()),this,SLOT(mover()));
				timer->start(10);
				//usleep(100000000);
				break;
			}
			this->lspMode=0;
			this->currentX=0;
			//this->packetLinesVector=NULL;*/

		}
		this->setAcceptDrops(0);
		delete lspPopup;
		lspPopup=NULL;
	}
}

void TopologyMap::drawPacketRectangle()
{
	for ( unsigned int i=0; i<packetRectangleVector.size(); i++ )
	{
		packetRectangleVector[i]->Run();
	}
}

void TopologyMap::stopLSP()
{
	for ( unsigned int i=0; i<packetRectangleVector.size(); i++ )
	{
		packetRectangleVector[i]->stop();
		delete packetRectangleVector[i];
		packetRectangleVector[i]=NULL;
		packetRectangleVector.erase(packetRectangleVector.begin()+i);
		i--;
	}

	this->lspMode=0;
	this->setAcceptDrops(1);

	for ( unsigned int k=0; k<lspVector.size() ; k++ )
	{
		
		for ( unsigned int n=0; n<lspVector[k]->linesVector.size() ; n++ )
		{
			delete lspVector[k]->linesVector[n];
			lspVector[k]->linesVector.erase( lspVector[k]->linesVector.begin() + n);
			n--;
		}

		delete lspVector[k];
		lspVector.erase(lspVector.begin()+k);
		k--;	
	}
	update();
}

QColor TopologyMap::getLabelColour(unsigned int label)
{
	for ( unsigned int i=0; i<labelColourVector.size(); i++ )
	{
		if ( labelColourVector[i]->label == label )
		{
			return (*labelColourVector[i]->colour);
		}
	}
	return Qt::white;
}

bool TopologyMap::getLspMode()
{
	return this->lspMode;
}

void TopologyMap::createLinkFailures()
{
	std::cout<<"link failures"<<std::endl;
	for ( uint32_t i=0; i<linkFailVector.size(); i++ )
	{
		s.str("");
		s<<"SCHEDULE P2PLINKFAILURE " << linkFailVector[i]->lineIndex<<" "<<linkFailVector[i]->failTime<<" "<<linkFailVector[i]->resume<<" "<<linkFailVector[i]->resumeTime;// 15.6 1 30
		this->sendCommand(s.str());	
		//this->scheduleFailure(lineIndex,failTime,resume,resumeTime);
		//SCHEDULE P2PLINKFAILURE 12 15.6 1 30
	}
}

void TopologyMap::parseMPLS()
{

	//delete newP;
	//startMplsSimulation();

	//showMeTheStateOfThings();
	startMplsSimulation();
}

QPoint TopologyMap::getNodeIconPos(std::string nodeName)
{
	NodeIcon *suspect=getNodeIconFromName(nodeName);
	return suspect->pos();
}

void TopologyMap::mplsSimulation()
{
	//if ( this->animationOn != 1 )
	//{
	//	return;
	//}
	
	size_t maxTime=0;
	for ( size_t t=0; t<this->mplsPacketVector.size(); t++ )
	{
		if ( mplsPacketVector[t]->startTime > maxTime )
		{
			maxTime=mplsPacketVector[t]->startTime;
		}

		if ( this->simTime > mplsPacketVector[t]->startTime && ( ( mplsPacketVector[t]->startTime%100 ) < 10 ) && mplsPacketVector[t]->stopped !=1 )
		{
			if ( this->animationStarted == -1 ) { animationStarted=0 ; animationFinished =0; }

			mplsPacketVector[t]->Run();
		}

	}	

	if ( (animationFinished >= animationStarted)  && ( simTime > maxTime )  )
	{
		stopMplsSimulation();
	}

}

void TopologyMap::startMplsSimulation()
{
	if ( mplsPacketVector.size() == 0 )
	{
		this->statusBar->showMessage("No MPLS Packets");
		return;
	
	}

	simTimer->start(10);
	this->lspMode=1;
	this->statusBar->clearMessage();
	this->statusBar->showMessage("Running Animation");
	setMouseTracking(false);
}

void TopologyMap::stopMplsSimulation()
{
	simTimer->stop();
	this->simTime=0;
	this->lspMode=0;
	for ( size_t t=0; t<mplsPacketVector.size(); t++ )
	{
		mplsPacketVector[t]->reset();
	}
	update();
	animationFinished =-4;
	animationStarted =-1;
	this->statusBar->clearMessage();
	this->statusBar->showMessage("Animation Completed");
	setMouseTracking(true);
}

void TopologyMap::updateSimTime()
{
	simTime+=10;
	update();
	
}

void TopologyMap::showMeTheStateOfThings()
{
}

uint16_t TopologyMap::getNodeIconHeight(std::string nodeName)
{

	NodeIcon *suspect=getNodeIconFromName(nodeName);
	return suspect->width();
}

uint16_t TopologyMap::getNodeIconWidth(std::string nodeName)
{

	NodeIcon *suspect=getNodeIconFromName(nodeName);
	return suspect->width();
}
