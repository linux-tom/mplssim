#ifndef SUPERTHREADER
#define SUPERTHREADER

#include "threader.h"

class SuperThreader : public Threader
{
	public:
		SuperThreader();
		~SuperThreader();
		void InternalThreadEntry();

}; 
#endif
