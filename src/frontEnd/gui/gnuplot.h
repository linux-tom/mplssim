#ifndef GNUPLOT2_H
#define GNUPLOT2_H

#include <stdio.h>
#include <sstream>
#include <iostream>

using namespace std;

class GNUplot
{
public:
	GNUplot() throw(string);
	~GNUplot();
	void operator ()(const string& command);
	// send any command to gnuplot
protected:
	FILE *gnuplotpipe;
};
 
GNUplot::GNUplot() throw(string)
{
	gnuplotpipe=popen("/usr/bin/gnuplot","w");
	if (!gnuplotpipe)
	{
		throw("Gnuplot not found !");
	}
}
 
GNUplot::~GNUplot()
{
	fprintf(gnuplotpipe,"exit\n");
	pclose(gnuplotpipe);
}
 
void GNUplot::operator() (const string& command)
{
	fprintf(gnuplotpipe,"%s\n",command.c_str());
	fflush(gnuplotpipe);
};

#endif
