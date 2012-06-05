#include "superThreader.h"
#include <unistd.h>
#include <iostream>

SuperThreader::SuperThreader() : Threader()
{
}

SuperThreader::~SuperThreader()
{
}

void SuperThreader::InternalThreadEntry()
{
	int i=0;
	while ( i<60 )
	{
		std::cout << "hello" <<std::endl;
		usleep(1000000);
		i++;
	}

}
