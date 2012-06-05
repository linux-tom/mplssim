#ifndef CONNLINE_H
#define CONNLINE_H
#include <iostream>

class ConnLine
{
	public:
		ConnLine(std::string start, std::string end,std::string subnet,std::string linkStartIpv4Address,std::string linkEndIpv4Address, std::string linkBandWidth, std::string linkDelay, int index);
		~ConnLine();
		std::string linkStart;
		std::string linkEnd;
		std::string subnet;
		std::string linkStartIpv4Address;
		std::string linkEndIpv4Address;
		std::string linkBandWidth;
		std::string linkDelay;
		unsigned int getIndex();
	private:		
		unsigned int index;
};
#endif
