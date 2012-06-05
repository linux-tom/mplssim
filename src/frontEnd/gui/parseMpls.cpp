#include "parseMpls.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

MplsParser::MplsParser(std::string fileName, TopologyMap* topologymap)
{
	this->topologymap=topologymap;
	this->simID=topologymap->getSimID();
	this->resultFile=new QFile(fileName.c_str());
	this->resultFile->open(QIODevice::ReadOnly | QIODevice::Text);
	s.str("");
	s<<"/tmp/"<<this->simID<<".results";
	this->simFile=new QFile( s.str().c_str() );
	simFile->open( QIODevice::WriteOnly | QIODevice::Text );
	fileLines = new QStringList;
	this->nextTime=1000;
}

MplsParser::~MplsParser()
{
}

void MplsParser::InternalThreadEntry()
{
	parseStuff();
	delete fileLines;
	pthread_cancel(this->_thread);
	return;
}

void MplsParser::parseStuff()
{
	QString line ;
	QTextStream* in=new QTextStream(this->resultFile);

	QString firstWord;
	QString secondWord;
	while (!in->atEnd())
	{
		line = in->readLine();
		QTextStream hello(line.toStdString().c_str() );
		hello >> firstWord;
		hello >> secondWord ;
		
		if ( firstWord.toStdString() == topologymap->getSimID() && secondWord == "STARTED" )
		//if ( firstWord == "simID0" && secondWord == "STARTED" )
		{
			QTextStream dataOut(this->simFile);

			while ( !in->atEnd() )
			{
				line = in->readLine();

				this->fileLines->append(line);

				QTextStream hello2(line.toStdString().c_str() );
				hello2 >> firstWord;
				hello2 >> secondWord ;
				if ( firstWord.toStdString() == topologymap->getSimID() && secondWord == "STOPPED" )
				//if ( firstWord == "simID0" && secondWord == "STOPPED" )
				{
					break;
				}
				else
				{
					dataOut << line<<"\n";
				}
			}
			simFile->close();
			break;
		}
		
	}
	resultFile->close();
	this->createObjects();
	this->theRealStuff();

	std::cout << topologymap->mplsPacketVector.size() << std::endl;
	//topologymap->mplsPacketVector.push_back(packet);
}

void MplsParser::theRealStuff()
{
	QString line ;
	QString firstWord;
	QString secondWord;
	QString loose;
	std::istringstream ss2;

	for ( int32_t x=0; x<fileLines->size(); x++ )	
	{
		line = fileLines->at(x);
		QTextStream hello(line.toStdString().c_str() );
		hello >> firstWord;
		ss2.clear();
		ss2.str(firstWord.toStdString().c_str());
		float time; 
		std::stringstream ss;

		if ( ss2 >> time ) // Check if we have a time Value, cool
		{
			hello >> loose; // skip one value
			hello >> loose;

			ss2.clear();
			uint16_t nodeID;
			ss2.str(loose.toStdString().substr(0,loose.toStdString().size()-1)); 
			if ( 	ss2 >> nodeID )
			{
				std::string nodeName=getNodeNameFromID(nodeID);
				if ( nodeName != "NULL" ) //just confirming
				{
					hello >> loose;
					if ( (loose == "Classification") ) // New Packet Received
					{
						MplsPacket::activity* mplspac=new MplsPacket::activity;

						mplspac->currentTime=int (time*1000);
						mplspac->currentNode=nodeName;
						mplspac->mpls=1;
						
						this->insertJunk(&hello,25);
						hello>>loose;
						std::string pSource=loose.toStdString();
						hello>>loose;
						hello>>loose;

						std::string pDest=loose.toStdString().substr(0,loose.toStdString().size()-1);
						QTextStream hello3(fileLines->at(x+1).toStdString().c_str() );
						this->insertJunk(&hello3,3);
						hello3 >> loose;

						//std::cout<<loose.toStdString()<<std::endl;

						MplsPacket* packet= new MplsPacket(pSource, pDest,(time*1000),topologymap);
						
						if ( loose == "Found" )
						{

							x=x+3;

							QTextStream hello4(getSelectedPolicy(x).c_str());

							this->insertJunk(&hello4,5);
							hello4 >> loose;
							//std::cout<<loose.toStdString()<<std::endl;
							//return;
							ss.str(loose.toStdString().substr(5, loose.toStdString().size()-1));
							uint32_t label;
	
							ss >> label;
							hello4 >> loose;
							hello4 >> loose;

							//std::cout << " packet hop "<<loose.toStdString()<<std::endl;

							std::string nextHop=getNodeNameFromIP(loose.toStdString());
							//hello4 >> loose;
							//hello4 >> loose;

							mplspac->nextHop=nextHop;
							mplspac->label=label;
							mplspac->done=0;

							uint16_t b=0;
							for ( b=0; b<topologymap->labelColourVector.size(); b++ )
							{
								if ( label == topologymap->labelColourVector[b]->label )
								{
									break;
								}

							}

							if ( b == topologymap->labelColourVector.size() )
							{

								TopologyMap::labelColour* newLabCol2=new TopologyMap::labelColour;
								newLabCol2->colour=&(topologymap->colourVector)[topologymap->labelColourIndex++];
								newLabCol2->label=label;
								topologymap->labelColourVector.push_back(newLabCol2);
							}

							//std::cout << label << " " << loose.toStdString()<<std::endl;

							packet->activityVector.push_back(mplspac);	

							//std::cout <<time<<nodeName<< " following packet 1 "<<nextHop <<" "<<label << std::endl;
							if ( nextHop == "NULL" )
							{
								//std::cout << "NULLNESS " <<loose.toStdString()<< std::endl;
								mplspac->nextHop="NULL";
								mplspac->label=0;
								mplspac->mpls=0;
								mplspac->done=1;

							}
							else
							{
								followToEnd(packet,x);
							}
						}

						else if ( loose == "Dropping" )
						{
							mplspac->nextHop="NULL";
							mplspac->label=0;
							mplspac->mpls=0;
							mplspac->done=1;
							//std::cout << "NULLNESS " <<loose.toStdString()<< std::endl;

							packet->activityVector.push_back(mplspac);	
						}
						else {}
						
						topologymap->mplsPacketVector.push_back(packet);
						nextTime=nextTime+100;
					}
				}
			}
			

		}
		else
		{
			//std::cout << firstWord.toStdString() << std::endl;
		}
	}
}

void MplsParser::followToEnd(MplsPacket *packet, int index )
{
	MplsPacket::activity* mplspac=new MplsPacket::activity;

	for ( int32_t x=index; x<fileLines->size(); x++ )
	{
		QString firstWord;
		QString loose;
		QString line = fileLines->at(x);
		QTextStream hello(line.toStdString().c_str() );
		hello >> firstWord;
		std::istringstream ss2;
		ss2.clear();
		ss2.str(firstWord.toStdString().c_str());
		float time; 
		std::stringstream ss;

		if ( ss2 >> time ) // Check if we have a time Value, cool
		{
			hello >> loose; // skip one value
			hello >> loose;

			ss2.clear();
			uint16_t nodeID;
			ss2.str(loose.toStdString().substr(0,loose.toStdString().size()-1)); 
			if ( 	ss2 >> nodeID )
			{
				std::string nodeName=getNodeNameFromID(nodeID);

				if ( nodeName != "NULL" ) //just confirming
				{
					hello >> loose;
					if ( loose == "Packet" ) // New Packet Received
					{
						hello >> loose;

						//find source node

						hello >> loose;
						std::string mac;
						mac = loose.toStdString().substr(6,loose.toStdString().size()-1);
						std::string sourceNode=getNodeNameFromMac(mac);

						int last=packet->activityVector.size();

						if ( (sourceNode == packet->activityVector[last-1]->currentNode) && (packet->activityVector[last-1]->nextHop == nodeName ) )
						{

							int linkDelay = 100;// 100 ms delay - consider the packet dropped
							int timeDiff = int ( time*1000) - packet->activityVector[last-1]->currentTime;

							if ( timeDiff <= linkDelay ) 
							{

								mplspac->currentTime=int (time*1000);
								mplspac->currentNode=nodeName;
								x=x+1;

								QTextStream hello3(fileLines->at(x).toStdString().c_str() );
								this->insertJunk(&hello3,5);
								hello3 >>loose; 	
								ss2.clear();
								ss2.str(loose.toStdString().substr(6,loose.toStdString().size()-1));
								uint32_t topLabel; 
								ss2 >> topLabel;

								//std::cout << "Top label "<<topLabel << std::endl;

								if ( topLabel == packet->activityVector[last-1]->label )
								{

									x=x+2;

									QTextStream hello99(fileLines->at(x).toStdString().c_str());

									hello99 >> loose;
									hello99 >> loose;
									hello99 >> loose;
									hello99 >> loose;

									if ( loose == "Found" )
									{
										x=x+2;

										QTextStream hello4(getSelectedPolicy(x).c_str());//(in->readLine().toStdString().c_str() );
										this->insertJunk(&hello4,5);
										std::string operation;	
										hello4 >> loose;
										operation = loose.toStdString();

										if ( operation == "pop" )
										{
											mplspac->nextHop="NULL";
											mplspac->label=0;
											mplspac->mpls=0;
											mplspac->done=0;
											packet->activityVector.push_back(mplspac);	
											//std::cout <<time<<" " <<nodeName<< " popping packet  "<<topLabel<<" "<<packet->destination<< std::endl;
										}
										else // a swap operation, lets get the swapped label
										{
											uint32_t outgoingLabel;
											ss2.clear();
											ss2.str(operation.substr(5,operation.size()-1));
											ss2 >> outgoingLabel;
											operation = "swap";	
											hello4 >> loose;
											hello4 >> loose;

											if ( outgoingLabel != packet->activityVector[last-1]->label )
											{
												TopologyMap::labelColour* newLabCol2=new TopologyMap::labelColour;
												newLabCol2->colour=&(topologymap->colourVector)[topologymap->labelColourIndex++];
												newLabCol2->label=outgoingLabel;
												topologymap->labelColourVector.push_back(newLabCol2);
											}

											

											std::string nextHop=getNodeNameFromIP(loose.toStdString());
											mplspac->nextHop=nextHop;
											mplspac->label=outgoingLabel;
											mplspac->done=0;
											mplspac->mpls=1;
											packet->activityVector.push_back(mplspac);	

											
											if ( nextHop == "NULL" )
											{
												//std::cout <<x<<"probably dropped "<< std::endl;
												mplspac->currentNode="NULL";
												mplspac->nextHop="NULL";
												//mplspac->nextHop=getNodeNameFromIP(loose.toStdString());
												mplspac->label=0;
												mplspac->done=1;
												mplspac->mpls=0;
												//std::cout << "NULLNESS " <<loose.toStdString()<< std::endl;
												packet->activityVector.push_back(mplspac);	
											}
											else
											{
												
												//std::cout <<time<<nodeName<< " following packet 2 "<<nextHop<<" "<<outgoingLabel << std::endl;
												followToEnd(packet,x);
											}
										}
									}
									else if ( loose == "Dropping" )
									{
										//std::cout <<x<<"probably dropped "<< std::endl;
										mplspac->currentNode="NULL";
										mplspac->nextHop="NULL";
										//mplspac->nextHop=getNodeNameFromIP(loose.toStdString());
										mplspac->label=0;
										mplspac->done=1;
										mplspac->mpls=0;
										//std::cout << "NULLNESS " <<loose.toStdString()<< std::endl;
										packet->activityVector.push_back(mplspac);	

									}
									else
									{

									}
								}
							}

							else // probably dropped
							{
								//std::cout <<x<<" dropped "<< std::endl;
						
								mplspac->currentNode="NULL";
								mplspac->nextHop="NULL";
								//mplspac->nextHop=getNodeNameFromIP(loose.toStdString());
								mplspac->label=0;
								mplspac->done=1;
								mplspac->mpls=0;
								//std::cout << "NULLNESS " <<loose.toStdString()<< std::endl;
								packet->activityVector.push_back(mplspac);	
							}

							break;
						}
					}
				}
			}
		}
	}
}


void MplsParser::createObjects()
{
	QString line ;
	QString firstWord;
	QString secondWord;
	QString loose;
	//while (!in->atEnd())
	for ( int32_t x=0; x<fileLines->size(); x++ )	
	{
		line = fileLines->at(x);
		QTextStream hello(line.toStdString().c_str() );
		hello >> firstWord;
		
		if ( firstWord == "NetworkDiscoverer:" )
		{
			this->insertJunk(&hello,6);
			std::string node;
			hello >> loose;
				
			node=loose.toStdString();
			if ( node.at(node.size()-5 )== ':' )
			{
				node=node.substr(4, node.size()-6);
				int nodeIndex;
				std::istringstream ss;
				ss.str(node); 
				ss >> nodeIndex;
				//std::cout<<nodeIndex<<std::endl;
				std::string macAddress;
				hello >> loose;
				macAddress = (loose.toStdString()).substr(0,loose.toStdString().size()-1);	
				this->insertJunk(&hello,1);
				std::string IPaddress;
				hello >> loose;
				IPaddress = loose.toStdString();
				//topologymap
				std::string nodeName=topologymap->getNodeFromIP(IPaddress);//getIPaddress;	
				//std::string nodeName=IPaddress;	
				//std::cout << nodeName << std::endl;	
				nodeIDStruct *newPair=new nodeIDStruct;

				newPair->nodeName=nodeName;
				newPair->macAddressList.push_back(macAddress);
				newPair->IPaddress=IPaddress;
				newPair->ID=nodeIndex;
				uint16_t i;

				for ( i=0; i<nodeIDVector.size(); i++ )
				{
					if ( nodeIDVector[i]->ID==nodeIndex )
					{
						nodeIDVector[i]->macAddressList.push_back(macAddress);	
						break;
					}	
				}
				if ( i == nodeIDVector.size() )
				{
					nodeIDVector.push_back(newPair);
				}
			}
		}
	}
}

std::string MplsParser::getNodeNameFromID(uint16_t ID)
{
	for ( uint16_t i=0; i<nodeIDVector.size(); i++ )
	{
		if ( nodeIDVector[i]->ID == ID )
		{
			return nodeIDVector[i]->nodeName;
		}
	}
	return "NULL";
}

std::string MplsParser::getNodeNameFromIP(std::string IP)
{
	for ( uint16_t i=0; i<nodeIDVector.size(); i++ )
	{
		if ( nodeIDVector[i]->IPaddress == IP )
		{
			return nodeIDVector[i]->nodeName;
		}
	}
	return topologymap->getNodeFromIP(IP);
//	return "NULL";
}

std::string MplsParser::getNodeNameFromMac(std::string mac)
{
	for ( uint16_t i=0; i<nodeIDVector.size(); i++ )
	{

		for ( uint16_t j=0; j<nodeIDVector[i]->macAddressList.size(); j++ )
		{
			if ( nodeIDVector[i]->macAddressList[j] == mac )
			{
				return nodeIDVector[i]->nodeName;
			}
		}
	}
	return "NULL";
}

void MplsParser::skipLine(QTextStream* in, uint16_t count)
{
	for ( unsigned int i=0; i<count; i++ )
	{
		in->readLine();
	}
}

void MplsParser::insertJunk(QTextStream *stream, unsigned int count)
{
	QString junk;
	for ( unsigned int i=0; i<count; i++ )
	{
		*stream >> junk;
	}
}

std::string MplsParser::getSelectedPolicy(int& index)
{

	QString disLine=fileLines->at(index);//stream->readLine().toStdString();
	QTextStream hello4(disLine.toStdString().c_str() );
	QString loose;

	this->insertJunk(&hello4,5);
	hello4 >> loose;

	if ( loose == "pop" )
	{
		
		hello4 >> loose;
		if ( loose == "selected" )
		{
			return disLine.toStdString();
		}
		else
		{
			index++;
			return getSelectedPolicy(index);
		}

	}
	else
	{
		hello4 >> loose;
		hello4 >> loose;

		hello4 >> loose;
		hello4 >> loose;
		if ( loose == "omitted" )
		{
			index++;
			//std::cout<<"omitted,checking again"<<std::endl;
			return getSelectedPolicy(index);
		}
		else
		{
			return disLine.toStdString();
		} 
	}
	return "NULL";
}
