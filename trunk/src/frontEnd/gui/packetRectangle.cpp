#include "packetRectangle.h"

PacketRectangle::PacketRectangle(TopologyMap* topologymap,std::vector<TopologyMap::line*>* packetLinesVector, int delay)
{
	this->topologymap=topologymap;
	this->packetLinesVector=packetLinesVector;
	timer=new QTimer(topologymap);
	connect(timer,SIGNAL(timeout()),this,SLOT(mover()));
	timer->start(10);
	this->counter=0;
	this->lineIndex=0;
	this->delay=delay;
	if ((*packetLinesVector).size() != 0 )
	{
		this->currentX=(*packetLinesVector)[0]->startX;
	}
	else
	{
		this->currentX=0;
	}
}

PacketRectangle::~PacketRectangle()
{

}

void PacketRectangle::Run()
{
	if ( (*packetLinesVector).size() == 0 ) 
	{
		return;
	}

	QRect rect(10,10,10,10);
	double grad=((*packetLinesVector)[lineIndex]->stopY-(*packetLinesVector)[lineIndex]->startY)/((*packetLinesVector)[lineIndex]->stopX-(*packetLinesVector)[lineIndex]->startX);
	double c=(*packetLinesVector)[lineIndex]->startY-(grad*(*packetLinesVector)[lineIndex]->startX);

	int x=int(currentX-delay);
	int y=int((grad*x)+c);
	if (( ( (*packetLinesVector)[lineIndex]->stopX >= (*packetLinesVector)[lineIndex]->startX )&& ( grad >= 0 ) && (x < (*packetLinesVector)[lineIndex]->startX ) ) || ( ( (*packetLinesVector)[lineIndex]->stopX <= (*packetLinesVector)[lineIndex]->startX )&& ( grad <= 0 ) && (x > (*packetLinesVector)[lineIndex]->startX ) )  || ( ( (*packetLinesVector)[lineIndex]->stopX <= (*packetLinesVector)[lineIndex]->startX )&& ( grad >= 0 ) && (x > (*packetLinesVector)[lineIndex]->startX ) ) || ( ( (*packetLinesVector)[lineIndex]->stopX >= (*packetLinesVector)[lineIndex]->startX )&& ( grad <= 0 ) && (x < (*packetLinesVector)[lineIndex]->startX ) )  )
	{
		if ((( (*packetLinesVector)[lineIndex]->stopY <= (*packetLinesVector)[lineIndex]->startY) && ( grad <= 0 )) || (( (*packetLinesVector)[lineIndex]->stopY >= (*packetLinesVector)[lineIndex]->startY )&& ( grad >= 0 )))
		{
			currentX=currentX+2;
		}
		else
		{

			currentX=currentX-2;
		}
		return;
	}


	if ((( (*packetLinesVector)[lineIndex]->stopY <= (*packetLinesVector)[lineIndex]->startY) && ( grad <= 0 )) || (( (*packetLinesVector)[lineIndex]->stopY >= (*packetLinesVector)[lineIndex]->startY )&& ( grad >= 0 )))
	{
		currentX=currentX+2;
	}
	else
	{

		currentX=currentX-2;
	}

	if (  (( (*packetLinesVector)[lineIndex]->stopY <= (*packetLinesVector)[lineIndex]->startY) && ( grad <= 0 ) && ( ( (grad*currentX) + c ) <= (*packetLinesVector)[lineIndex]->stopY ) ) || (((*packetLinesVector)[lineIndex]->stopY >= (*packetLinesVector)[lineIndex]->startY)&& (grad < 0 ) && (((grad*currentX)+c ) >= (*packetLinesVector)[lineIndex]->stopY  ) ) || ( ((*packetLinesVector)[lineIndex]->stopY < (*packetLinesVector)[lineIndex]->startY ) && (grad >= 0 ) && (((grad*currentX)+c)<=(*packetLinesVector)[lineIndex]->stopY)) || (( (*packetLinesVector)[lineIndex]->stopY >= (*packetLinesVector)[lineIndex]->startY )&& ( grad >= 0 ) &&( ((grad * currentX) +c ) >=  (*packetLinesVector)[lineIndex]->stopY) ))
	{
		if ( (this->lineIndex +1) >=(*packetLinesVector).size())
		{
			this->lineIndex=0;
		}
		else
		{
			this->lineIndex++;
		}
		currentX=(*packetLinesVector)[lineIndex]->startX;
	}

	rect.moveTopLeft(QPoint(x,y));
	QPainter painter(topologymap);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::green);
	painter.setBrush(topologymap->getLabelColour((*packetLinesVector)[lineIndex]->label) );
	painter.drawRect(rect);
	painter.setBrush(Qt::red);

}

void PacketRectangle::stop()
{
	timer->stop();
	(*packetLinesVector).clear();
}

void PacketRectangle::mover()
{
	topologymap->update();
	/*if ( this->counter > 10000 )
	{
		for ( unsigned int k=0; k<lspVector.size() ; k++ )
		{
			lspVector[k]->linesVector.clear();
			delete lspVector[k];
			lspVector.erase(lspVector.begin()+k);
			k--;	
		}
	}
	else
	{
		this->counter++;
	}*/
}

