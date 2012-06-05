#include "connLine.h"

ConnLine::ConnLine(std::string start, std::string end,std::string subnet, std::string linkStartIpv4Address,std::string linkEndIpv4Address, std::string linkBandWidth, std::string linkDelay, int index)
{
	this->linkStart=start;
	this->linkEnd=end;
	this->subnet=subnet;
	this->linkStartIpv4Address=linkStartIpv4Address;
	
	this->linkEndIpv4Address=linkEndIpv4Address;
	this->linkBandWidth=linkBandWidth;
	this->linkDelay=linkDelay;
	this->index=index;
}

ConnLine::~ConnLine()
{
}
unsigned int ConnLine::getIndex()
{
	return this->index;
}
