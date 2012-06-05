#include "mplsPacket.h"
 
MplsPacket::MplsPacket(std::string source, std::string destination,uint32_t startTime, TopologyMap* topology)
{
	this->source=source;
	this->destination=destination;
	this->topologymap=topology;
	this->startTime=startTime;
	this->counter=0;
	this->lineIndex=0;
	this->stopped=0;
	this->started=0;
	this->firstRun=0;
	this->currentX=0;
	this->updated=0;
}


MplsPacket::~MplsPacket()
{

}

void MplsPacket::Run()
{
	if ( this->updated == 0 )
	{
		topologymap->animationStarted++;
		updated=1;
	}

	QRect rect(10,10,10,10);
	if ( started == 0 )
	{
		getNextLine();
	}

	if ( currentX == 0 )
	{
		currentX=currentLine.startX;
	}

	int x=currentX;
	int y=(currentLine.grad*currentX) + currentLine.intercept;

	//The next value of currentX
	if (( (currentLine.stopY <= currentLine.startY) && ( currentLine.grad <= 0 )) || ((currentLine.stopY >= currentLine.startY )&& ( currentLine.grad >= 0 )))
	{
		currentX=currentX+2; // increase x positively
	}
	else
	{

		currentX=currentX-2; //reduce x
	}

	//int x=currentX;
	//int y=currentY;

	rect.moveCenter(QPoint(x,y));
	QPainter painter(topologymap);
	painter.setPen(Qt::NoPen);
	painter.setBrush(topologymap->getLabelColour(currentLine.label));
	painter.drawRect(rect);

//Check if next value of y will be within range. Move to next Line 
		if (  (( currentLine.stopY <= currentLine.startY) && ( currentLine.grad <= 0 ) && ( ( (currentLine.grad*currentX) + currentLine.intercept ) <= currentLine.stopY ) ) || ((currentLine.stopY >= currentLine.startY)&& (currentLine.grad < 0 ) && (((currentLine.grad*currentX)+currentLine.intercept ) >= currentLine.stopY  ) ) || ( (currentLine.stopY < currentLine.startY ) && (currentLine.grad >= 0 ) && (((currentLine.grad*currentX)+currentLine.intercept)<=currentLine.stopY)) || (( currentLine.stopY >= currentLine.startY )&& ( currentLine.grad >= 0 ) &&( ((currentLine.grad * currentX) +currentLine.intercept ) >=  currentLine.stopY) )) 
	{
		if ( firstRun == 1 )
		{
			lineIndex++;
		}
		else
		{
			firstRun =1;
		}

		currentX=0;

		if ( lineIndex <= (activityVector.size()-1) )
		{
			getNextLine();
		}
		else
		{
			this->stopped = 1;
			topologymap->animationFinished++;
		}
	}
}

void MplsPacket::getNextLine()
{
	
	if ( lineIndex >= activityVector.size() )
	{
		stop();
		return;
	}

	if ( ( activityVector[lineIndex]->currentNode == "NULL" )/* || ( (activityVector[lineIndex]->nextHop == "NULL") && activityVector[lineIndex]->done == 1 )*/ )
	{
		stop();
		return;
	}


	if ( started == 0 )
	{

		std::string sourceNode=topologymap->getNodeFromIP(source);

		QPoint sourcePoint	=	topologymap->getNodeIconPos(sourceNode);
		QPoint destPoint	=	topologymap->getNodeIconPos(activityVector[0]->currentNode);

		uint16_t sourceWidth		=	topologymap->getNodeIconWidth(sourceNode);
		uint16_t sourceHeight		=	topologymap->getNodeIconWidth(sourceNode);

		uint16_t destWidth		=	topologymap->getNodeIconWidth(activityVector[0]->currentNode);
		uint16_t destHeight		=	topologymap->getNodeIconWidth(activityVector[0]->currentNode);

		currentLine.startX=sourcePoint.x()+sourceWidth/2;
		currentLine.startY=sourcePoint.y()+sourceHeight/2;
		currentLine.stopX=destPoint.x()+destWidth/2;
		currentLine.stopY=destPoint.y()+destHeight/2;
		currentLine.grad= (float(currentLine.stopY-currentLine.startY)/float(currentLine.stopX-currentLine.startX));
		currentLine.intercept=currentLine.stopY-(currentLine.grad*currentLine.stopX);
		currentLine.label=0;	
		started = 1;
		firstRun = 0;
	}

	else if ( (activityVector[lineIndex]->mpls == 0) && (activityVector[lineIndex]->done !=1) ) // move to dest
	{
		std::string destNode = topologymap->getNodeFromIP(destination);

		QPoint sourcePoint=topologymap->getNodeIconPos(activityVector[lineIndex]->currentNode);
		QPoint destPoint=topologymap->getNodeIconPos(destNode);

		uint16_t sourceWidth		=	topologymap->getNodeIconWidth(activityVector[lineIndex]->currentNode);
		uint16_t sourceHeight		=	topologymap->getNodeIconWidth(activityVector[lineIndex]->currentNode);

		uint16_t destWidth		=	topologymap->getNodeIconWidth(destNode);
		uint16_t destHeight		=	topologymap->getNodeIconWidth(destNode);

		currentLine.startX=sourcePoint.x()+sourceWidth/2;
		currentLine.startY=sourcePoint.y()+sourceHeight/2;
		currentLine.stopX=destPoint.x()+destWidth/2;
		currentLine.stopY=destPoint.y()+destHeight/2;
		currentLine.grad= (float(currentLine.stopY-currentLine.startY)/float(currentLine.stopX-currentLine.startX));
		currentLine.intercept=currentLine.stopY-(currentLine.grad*currentLine.stopX);
		currentLine.label=0;	
	}

	else
	{
		if ( lineIndex >= activityVector.size() )
		{
			stop();
			return;
		}

		if ( activityVector[lineIndex]->currentNode == "NULL" || activityVector[lineIndex]->nextHop == "NULL")
		{
			stop();
			return;
		}

		uint16_t sourceWidth		=	topologymap->getNodeIconWidth(activityVector[lineIndex]->currentNode);
		uint16_t sourceHeight		=	topologymap->getNodeIconWidth(activityVector[lineIndex]->currentNode);

		uint16_t destWidth		=	topologymap->getNodeIconWidth(activityVector[lineIndex]->nextHop);
		uint16_t destHeight		=	topologymap->getNodeIconWidth(activityVector[lineIndex]->nextHop);

		QPoint sourcePoint=topologymap->getNodeIconPos(activityVector[lineIndex]->currentNode);
		QPoint destPoint=topologymap->getNodeIconPos(activityVector[lineIndex]->nextHop);

		currentLine.startX=sourcePoint.x()+sourceWidth/2;
		currentLine.startY=sourcePoint.y()+sourceHeight/2;
		currentLine.stopX=destPoint.x()+destWidth/2;
		currentLine.stopY=destPoint.y()+destHeight/2;
		currentLine.grad= (float(currentLine.stopY-currentLine.startY)/float(currentLine.stopX-currentLine.startX));
		currentLine.intercept=currentLine.stopY-(currentLine.grad*currentLine.stopX);
		currentLine.label=activityVector[lineIndex]->label ;	
	}
}

void MplsPacket::reset()
{
	stopped = 0;
	this->counter=0;
	this->lineIndex=0;
	this->stopped=0;
	this->started=0;
	this->firstRun=0;
	this->currentX=0;
	this->updated=0;
}

void MplsPacket::stop()
{
	this->stopped=1;
	topologymap->animationFinished++;
}
