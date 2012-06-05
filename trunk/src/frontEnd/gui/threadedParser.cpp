#include "threadedParser.h"
#include <unistd.h>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "gnuplot.h" 

ThreadedParser::ThreadedParser(std::string nodeName,TopologyMap* topologymap)
{
	//pthread_mutex_init( &mutex1,NULL);
	this->simulationID=topologymap->getSimID();
	this->topologymap=topologymap;
	this->nodeName=nodeName;
}

ThreadedParser::~ThreadedParser()
{
	this->WaitForInternalThreadToExit();
}

void ThreadedParser::InternalThreadEntry()
{
	std::cout<<"hello "<<this->nodeName<<std::endl;
	this->parseStuff();
	pthread_cancel(this->_thread);
	return;
}

void ThreadedParser::parseStuff()
{
	this->updateStatus();
	//topologymap->incrementParsed();
	s.str("");
	s<<"/tmp/"<<this->simulationID<<"/"<<nodeName;
	std::string fileName=s.str();
	QFile file(fileName.c_str());
	
	if ( ! file.exists() ) 
	{
		return;
	}

	file.open(QIODevice::ReadOnly | QIODevice::Text);
	
	QTextStream* in=new QTextStream(&file);

	QString line ;
	char result;
	int received=0, dropped=0, queued=0, dequeued=0;
	float time;
	QString line3;
	QString senderIP;

	while (!in->atEnd())
	{
		line = in->readLine();
		QTextStream hello(line.toStdString().c_str() );
		hello >> result;
		hello >> time ;
		if ( result == 'd' )
		{
			dropped++;
			insertJunk(&hello,1); 
			hello >> line3 ;
			std::string sIP;
			if ( line3=="ns3::EthernetHeader" )
			{
				this->insertJunk(&hello,4);
				hello >> line3 ;
				if ( line3 != "ns3::ArpHeader" )
				{
					this->insertJunk(&hello,21); // for csma
					hello >> senderIP ;
					sIP=senderIP.toStdString().substr(0, senderIP.toStdString().size()-1);
				}
				else
				{
					this->insertJunk(&hello,8); // for csma
					hello >> line3;
					if ( line3.toStdString() == "mac:" )
					{
						this->insertJunk(&hello,3); 
						hello >> senderIP ;
					}
					else
					{
						hello >> senderIP ;
					}
					sIP=senderIP.toStdString().substr(0, senderIP.toStdString().size()-1);
				}
			}
			else
			{
				this->insertJunk(&hello,26); // for P2P
				hello >> senderIP ;
				sIP=senderIP.toStdString().substr(0, senderIP.toStdString().size()-1);

			}
			
			std::string senderName=topologymap->getNodeFromIP(sIP);	
			if ( senderName == "NULL" )
			{
				std::cout<<"DROPPED "<<senderIP.toStdString()<<"  "<<line3.toStdString()<<std::endl;
			}
			unsigned int k=0;
			for ( k=0; k<senderVector.size();k++)
			{
				if ( senderVector[k]->nodeName == senderName )
				{
					senderVector[k]->droppedPackets++;
					break;
				}
			}
			if ( k == (senderVector.size()) ) // new node
			{
				senderNode* newNode=new senderNode;
				senderVector.push_back(newNode);
				senderVector[k]->droppedPackets=1;
				senderVector[k]->sentPackets=0;
				senderVector[k]->receivedPackets=0;
				senderVector[k]->nodeName=senderName;
			}
			
			/*unsigned int milliSeconds=int((1000)*time+0.5);
			
			for ( j=0; j<packetsVector.size();j++)
			{
				if ( packetsVector[j]->nodeName == senderName && packetsVector[j]->milliseconds == milliSeconds  )
				{
					packetsVector[j]->droppedPackets++;
					break;
				}
			}
			
			if ( j == (packetsVector.size()) ) // new packet instance
			{

				packetStruct* newPacket=new packetStruct;
				packetsVector[j]->milliseconds=milliSeconds;
				packetsVector[j]->droppedPackets=1;
				packetsVector[j]->receivedPackets=0;
				packetsVector[j]->sentPackets=0;
				packetsVector[j]->nodeName=senderName;
			}*/
			unsigned int q=0;
			unsigned int seconds=int(time+0.5);
			q=0;
			for (q=0; q<packetsVector2.size();q++)
			{
				if ( packetsVector2[q]->nodeName == senderName && packetsVector2[q]->seconds == seconds  )
				{
					packetsVector2[q]->droppedPackets++;
					break;
				}
			}
			
			if ( q == (packetsVector2.size()) ) // new packet instance
			{

				packetStruct2* newPacket=new packetStruct2;
				packetsVector2.push_back(newPacket);
				packetsVector2[q]->seconds=seconds;
				packetsVector2[q]->droppedPackets=1;
				packetsVector2[q]->receivedPackets=0;
				packetsVector2[q]->sentPackets=0;
				packetsVector2[q]->nodeName=senderName;
				//std::cout<<"New packetVector created at "<<seconds<<std::endl;
				//std::cout<<"NEW "<<nodeName<<" Sender "<<senderName<<std::endl;
			}
			
		}
		else if ( result == 'r' )
		{
			received++;
			this->insertJunk(&hello,1); // for csma
			hello >> line3 ;
			if ( line3=="ns3::EthernetHeader" )
			{
				this->insertJunk(&hello,4);
				hello >> line3 ;
				if ( line3 != "ns3::ArpHeader" )
				{
					this->insertJunk(&hello,19); // for csma
					hello >> senderIP ;
				}
				else
				{
					this->insertJunk(&hello,6); // for csma
					hello >> senderIP ;
				}
			}
			else
			{
				this->insertJunk(&hello,19); // for P2P
				hello >> senderIP ;
			}

			std::string senderName=topologymap->getNodeFromIP(senderIP.toStdString());	
			if ( senderName == "NULL" )
			{
				std::cout<<"RECEIVED: "<<senderIP.toStdString()<<"  "<<line3.toStdString()<<std::endl;
			}
				
			unsigned int k=0;
			for ( k=0; k<senderVector.size();k++)
			{
				if ( senderVector[k]->nodeName == senderName )
				{
					senderVector[k]->receivedPackets++;
					break;
				}
			}
	
			if ( k == (senderVector.size()) ) // new node
			{
				senderNode* newNode=new senderNode;
				senderVector.push_back(newNode);
				senderVector[k]->receivedPackets=1;
				senderVector[k]->droppedPackets=0;
				senderVector[k]->sentPackets=0;
				senderVector[k]->nodeName=senderName;
			}
			unsigned int j=0;
			/*	unsigned int milliSeconds=int((1000*time)+0.5);
			
			for ( j=0; j<packetsVector.size();j++)
			{
				if ( packetsVector[j]->nodeName == senderName && packetsVector[j]->milliseconds == milliSeconds  )
				{
					packetsVector[j]->receivedPackets++;
					break;
				}
			}
			
			if ( j == (packetsVector.size()) ) // new packet instance
			{

				packetStruct* newPacket=new packetStruct;
				packetsVector.push_back(newPacket);
				packetsVector[j]->milliseconds=milliSeconds;
				packetsVector[j]->droppedPackets=0;
				packetsVector[j]->receivedPackets=1;
				packetsVector[j]->sentPackets=0;
				packetsVector[j]->nodeName=senderName;
			}*/
			unsigned int seconds=int(time+0.5);
			
			for ( j=0; j<packetsVector2.size();j++)
			{
				if ( packetsVector2[j]->nodeName == senderName && packetsVector2[j]->seconds == seconds  )
				{
					packetsVector2[j]->receivedPackets++;
					break;
				}
			}
			
			if ( j == (packetsVector2.size()) ) // new packet instance
			{

				packetStruct2* newPacket=new packetStruct2;
				packetsVector2.push_back(newPacket);
				packetsVector2[j]->seconds=seconds;
				packetsVector2[j]->droppedPackets=0;
				packetsVector2[j]->receivedPackets=1;
				packetsVector2[j]->sentPackets=0;
				packetsVector2[j]->nodeName=senderName;
			}
			
		}
		else if ( result == '+' )
		{
			queued++;
		}
		else if ( result == '-' )
		{
			dequeued++;
			insertJunk(&hello,1); 
			hello >> line3 ;
			//insertJunk(&hello,26);
			std::string sIP;
			if ( line3=="ns3::EthernetHeader" )
			{
				this->insertJunk(&hello,4);
				hello >> line3 ;
				if ( line3 != "ns3::ArpHeader" )
				{
					this->insertJunk(&hello,21); // for csma
					hello >> senderIP ;
					sIP=senderIP.toStdString().substr(0, senderIP.toStdString().size()-1);
				}
				else
				{
					this->insertJunk(&hello,8); // for csma
					hello >> line3;
					if ( line3.toStdString() == "mac:" )
					{
						this->insertJunk(&hello,3); 
						hello >> senderIP ;
					}
					else
					{
						hello >> senderIP ;
					}
					sIP=senderIP.toStdString().substr(0, senderIP.toStdString().size()-1);
				}
			}
			else
			{
				this->insertJunk(&hello,26); // for P2P
				hello >> senderIP ;
				sIP=senderIP.toStdString().substr(0, senderIP.toStdString().size()-1);

			}
			
			std::string senderName=topologymap->getNodeFromIP(sIP);	
			if ( senderName == "NULL" )
			{
				std::cout<<"SENT "<<senderIP.toStdString()<<"  "<<line3.toStdString()<<std::endl;
			}
			unsigned int k=0;
			for ( k=0; k<senderVector.size();k++)
			{
				if ( senderVector[k]->nodeName == senderName )
				{
					senderVector[k]->sentPackets++;
					break;
				}
			}
	
			if ( k == (senderVector.size()) ) // new node
			{
				senderNode* newNode=new senderNode;
				senderVector.push_back(newNode);
				senderVector[k]->sentPackets=1;
				senderVector[k]->droppedPackets=0;
				senderVector[k]->receivedPackets=0;
				senderVector[k]->nodeName=senderName;
			}

		/*	unsigned int milliSeconds=int((1000)*time+0.5);
			
			for ( j=0; j<packetsVector.size();j++)
			{
				if ( packetsVector[j]->nodeName == senderName && packetsVector[j]->milliseconds == milliSeconds  )
				{
					packetsVector[j]->sentPackets++;
					break;
				}
			}
			
			if ( j == (packetsVector.size()) ) // new packet instance
			{

				packetStruct* newPacket=new packetStruct;
				packetsVector.push_back(newPacket);
				packetsVector[j]->milliseconds=milliSeconds;
				packetsVector[j]->droppedPackets=0;
				packetsVector[j]->receivedPackets=0;
				packetsVector[j]->sentPackets=1;
				packetsVector[j]->nodeName=senderName;
			}*/
			unsigned int j=0;
			unsigned int seconds=int(time+0.5);
			
			for ( j=0; j<packetsVector2.size();j++)
			{
				if ( packetsVector2[j]->nodeName == senderName && packetsVector2[j]->seconds == seconds  )
				{
					packetsVector2[j]->sentPackets++;
					break;
				}
			}
				
			if ( j == (packetsVector2.size()) ) // new packet instance
			{

				packetStruct2* newPacket=new packetStruct2;
				packetsVector2.push_back(newPacket);
				packetsVector2[j]->seconds=seconds;
				packetsVector2[j]->droppedPackets=0;
				packetsVector2[j]->receivedPackets=0;
				packetsVector2[j]->sentPackets=1;
				packetsVector2[j]->nodeName=senderName;
			}
			
		}
		else
		{
		}
	}
	file.close();
	
	//generateDataFiles
	s.str("");
	s<<"/tmp/"<<this->simulationID<<"/"<<nodeName<<".dat";
	QFile datFile( s.str().c_str() );
	datFile.open( QIODevice::WriteOnly | QIODevice::Text );
	QTextStream dataOut(&datFile);
	dataOut << "Nodes\t"<<"Dropped\t"<<"Sent\t"<<"Received\n";
	for ( unsigned int j=0; j<senderVector.size(); j++)
	{
		dataOut << senderVector[j]->nodeName.c_str() <<"\t"<<senderVector[j]->droppedPackets<<"\t"<<senderVector[j]->sentPackets<<"\t"<<senderVector[j]->receivedPackets<<"\n";
	}
	//senderVector.clear();
	datFile.close();

	s.str("");
	s<<this->nodeName<<" Histogram";

	TopologyMap::nodeGraph *newGraph2=new TopologyMap::nodeGraph;
	newGraph2->nodeName=this->nodeName;
	newGraph2->graphName=s.str();
	
	s.str("");
	s<<"/tmp/"<<this->simulationID<<"/";
	std::string baseDir=s.str();
	s<<this->nodeName<<"-histogram.png";
	newGraph2->fileName=s.str();
	newGraph2->index=0;
	this->pushToVector(newGraph2);

	int y_axis=0;
	if ( received > dropped )
	{
		if ( dequeued > received )
		{
			y_axis=dequeued;
		}
		else
		{
			y_axis=received;
		}
	}
	else
	{
		if ( dequeued > dropped )
		{
			y_axis=dequeued;
		}
		else
		{
			y_axis=dropped;
		}
	}
	generateHistogram(nodeName, y_axis);

	QFile *datFile2;
	QTextStream *dataOut2;

	for ( unsigned int j=0; j<senderVector.size(); j++)
	{
		unsigned int max_y=0, max_t=0, min_t =100000000;
		std::string senderName=senderVector[j]->nodeName;

		s.str("");
		s<<nodeName<<"-"<<senderName;
		std::string pairName=s.str();
		std::cout<<pairName<<std::endl;
		s.str("");
		s<<"/tmp/"<<this->simulationID<<"/"<<pairName<<".dat";

		datFile2 = new QFile(s.str().c_str());
		datFile2->open( QIODevice::WriteOnly | QIODevice::Text );
		dataOut2 = new QTextStream(datFile2);
		(*dataOut2) << "Time\t"<<"Dropped\t"<<"Sent\t"<<"Received\n";
		
		for ( unsigned int k=0; k<packetsVector2.size(); k++)
		{
			if ( packetsVector2[k]->nodeName == senderName )
			{
				if ( packetsVector2[k]->droppedPackets >= max_y )
				{
					if ( packetsVector2[k]->droppedPackets >= packetsVector2[k]->receivedPackets )
					{
						max_y=packetsVector2[k]->droppedPackets;
						if (packetsVector2[k]->sentPackets >= packetsVector2[k]->droppedPackets)
						{
							max_y=packetsVector2[k]->sentPackets;
						}
					}
					else
					{
						max_y=packetsVector2[k]->receivedPackets;
						if (packetsVector2[k]->sentPackets >= packetsVector2[k]->receivedPackets)
						{
							max_y=packetsVector2[k]->sentPackets;
						}
					}
				}

				if ( packetsVector2[k]->seconds >= max_t)
				{
					max_t = packetsVector2[k]->seconds;	
				}

				if ( packetsVector2[k]->seconds <= min_t)
				{
					min_t = packetsVector2[k]->seconds;	
				}

				(*dataOut2) << packetsVector2[k]->seconds<<"\t"<<packetsVector2[k]->droppedPackets<<"\t"<<packetsVector2[k]->sentPackets<<"\t"<<packetsVector2[k]->receivedPackets<<"\n";

			}
		}
		std::cout<<max_t<<std::endl;
		datFile2->close();
		this->generateTrafficProfile(pairName,max_y,max_t, min_t);

		s.str("");
		s<<pairName<<" Traffic-Profile";

		TopologyMap::nodeGraph *newGraph=new TopologyMap::nodeGraph;
		newGraph->nodeName=this->nodeName;
		newGraph->graphName=s.str();
		
		s.str("");
		s<<"/tmp/"<<this->simulationID<<"/";
		std::string baseDir=s.str();
		s<<pairName<<"-profile.png";
		newGraph->fileName=s.str();
		this->pushToVector(newGraph);
		
	}

	for ( unsigned int k=0; k<packetsVector.size(); k++ )
	{
		delete packetsVector2[k];
		packetsVector2.erase(packetsVector2.begin()+k);
		k--;
	}

	for ( unsigned int j=0; j<senderVector.size(); j++ )
	{
		delete senderVector[j];
		senderVector.erase(senderVector.begin()+j);
		j--;
	}
	topologymap->setSimRunning(0);
	topologymap->setSimStatus(1);
	//topologymap->incrementParsed();
	this->updateStatus();
	//pthread_cancel(this->_thread);
}


void ThreadedParser::insertJunk(QTextStream* stream, unsigned int count )
{
	QString junk;
	for ( unsigned int i=0; i<count; i++ )
	{
		*stream >> junk;
	}
} 
	
void ThreadedParser::generateHistogram(std::string nodeName,int y_axis)
{
	s.str("");
	s<<"/tmp/"<<this->simulationID<<"/";
	//s<<"/tmp/simID0/";
	std::string baseDir=s.str();
	GNUplot plotter;
	y_axis=int(1.25 * y_axis);
	s.str("");
	s<<"set terminal png size 1000, 500\n";
	//s<<"set terminal jpeg medium\n";
	s<<"set output \""<<baseDir<<nodeName<<"-histogram.png\"\n";	
	s<<"set title \"PACKETS RECEIVED : "<<nodeName<<" \"\n ";
	s<<"set auto x\n";
	s<<"set yrange [0:"<<y_axis<<"]\n";
	s<<"set style data histogram\n";
	s<<"set style histogram cluster gap 1\n"; 
	s<<"set style fill solid border -1\n";
	s<<"set boxwidth 0.9 relative\n";
	s<<"set ylabel \"Total Packets\"\n";
	s<<"set xtic rotate by 0 scale 0 font \",14\"\n";
	s<<"set bmargin 10\n";
	s<<"plot \'"<<baseDir<<nodeName<<".dat"<<"\' using 2:xtic(1) ti col, \'\' u 3 ti col, \'\' u 4 ti col\n";
	//	s<<"plot \'/tmp/simID0/SERVER_0.dat\' using 2:xtic(1) ti col, \'\' u 3 ti col, \'\' u 4 ti col\n";
	plotter(s.str());
}


void ThreadedParser::generateTrafficProfile(std::string pair, unsigned int max_y, unsigned int max_t, unsigned int min_t)
{
	std::cout<<pair<<std::endl;
	s.str("");
	s<<"/tmp/"<<this->simulationID<<"/";
	std::string baseDir=s.str();
	std::cout<<baseDir<<std::endl;
	GNUplot plotter;
	unsigned int max_y2=int((max_y*1.25)+0.5);
	if ( max_y2 <= max_y )
	{
		max_y=int((max_y*1.5)+0.5);
	}
	else
	{
		max_y=max_y2;
	}
	max_t=int(max_t*1.25);

	s.str("");
	s<<"set terminal png size 1000, 800\n";
	s<<"set output \""<<baseDir<<pair<<"-profile.png\"\n";
	s<<"set title \"TRAFFIC PROFILE : "<<pair<<" \"\n ";
	s<<"set key center top\n";
	s<<"set style fill solid\n";
	s<<"#set xdata time\n";
	s<<"#set timefmt \"%S\"\n";
	s<<"set ytics "<<max_y<<"\n";
	s<<"set y2tics "<<max_y<<" border\n";
	s<<"set xlabel \"Time ( S )\"\n";
	s<<"set ylabel \"Packets\"\n";

	s<<"set style line 1 linetype 1 pointtype 0 linewidth 1 linecolor rgb \"green\"\n";
	s<<"set style line 2 linetype 2 pointtype 0 linewidth 1 linecolor rgb \"blue\"\n";
	s<<"set style line 3 linetype 3 pointtype 0 linewidth 1 linecolor rgb \"red\"\n"; 

	s<<"plot [\"0\":\""<<max_t<<"\"][:] \\\n";
	s<<"\'"<<baseDir<<pair<<".dat\' using 1:2  smooth bezier t  \"DROPPED\" w lines linestyle 3, \\\n";
	s<<"\'"<<baseDir<<pair<<".dat\' using 1:3  smooth bezier  t \"SENT\" w lines linestyle 2, \\\n";
	s<<"\'"<<baseDir<<pair<<".dat\' using 1:4  smooth bezier t \"RECEIVED\" w lines linestyle 1\n";
	plotter(s.str());
	
	/*
	s.str("");	
	s<<"set terminal png size 1000, 500\n";
	s<<"set output \""<<baseDir<<pair<<"-sent-profile.png\"\n";
	s<<"set title \"SENT-TRAFFIC PROFILE : "<<pair<<" \"\n ";
	s<<"set key center top\n";
	s<<"set style fill solid\n";
	s<<"#set xdata time\n";
	s<<"#set timefmt \"%S\"\n";
	s<<"set ytics "<<max_y<<"\n";
	s<<"set y2tics "<<max_y<<" border\n";
	s<<"set xlabel \"Time ( S )\"\n";

	s<<"set style line 1 linetype 1 pointtype 0 linewidth 1 linecolor rgb \"green\"\n";
	s<<"set style line 2 linetype 2 pointtype 0 linewidth 1 linecolor rgb \"blue\"\n";
	s<<"set style line 3 linetype 3 pointtype 0 linewidth 1 linecolor rgb \"red\"\n"; 

	s<<"plot [\""<<min_t<<"\":\""<<max_t<<"\"][:] \\\n";
	s<<"\'"<<baseDir<<pair<<".dat\' using 1:3  smooth bezier  t \"SENT\" w filledcurve \n";
	plotter(s.str());*/
}

void ThreadedParser::updateStatus()
{
	pthread_mutex_lock( &incrementMutex );
	topologymap->incrementParsed();
	pthread_mutex_unlock( &incrementMutex );
}

void ThreadedParser::pushToVector(TopologyMap::nodeGraph* newGraph)
{
	pthread_mutex_lock( &vectorMutex );
	topologymap->graphsVector.push_back(newGraph);
	pthread_mutex_unlock( &vectorMutex );
}

