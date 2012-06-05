#include <string>
#include <iostream>
#include <sstream>
#include "ServerSocket.h"
#include "SocketException.h"
#include "sim_handler.h"
#include "vector"


//using namespace ns3;
//using namespace mpls;

int
main (int argc, char *argv[])
{
	std::vector <simHandler*> simVector;
	std::cout << "MPLS SIMULATOR SERVER STARTED \n";
	unsigned int simID=0;

	try
	{
		ServerSocket server ( 8888 );

		while ( true )
		{

			ServerSocket new_sock;
			server.accept ( new_sock );
			try
			{
				while ( true )
				{
					std::string command;
					std::stringstream s;
					s.str("");

					new_sock >> command;

					std::cout<<command<<std::endl;
					
					if ( command == "getNewID" )
					{
						s<<"simID"<< simID;	
						new_sock << s.str();
						std::cout<<s.str()<<std::endl;
						simVector.push_back(new simHandler(s.str()));
						simID++;
					}
					else if ( command == "11241ExitAndQuit")
					{
						return 0;
					}
					else
					{
						std::vector <std::string> splitCommand;
						
						std::istringstream iss(command);

						std::stringstream s("");

						std::string sub;

						while (iss >> sub)
						{
							splitCommand.push_back(sub);
						}
						
						for ( unsigned short i=0; i<simVector.size(); i++ )
						{
							if ( simVector[i]->getID() == splitCommand[0] )
							{
								s.str("");
								if ( splitCommand[1] == "DesTroY" && simVector[i]->getSimStatus() == 1 )
								{
									std::cout<<"Destroying Simulation"<<std::endl;
									delete simVector[i];
									simVector.erase(simVector.begin()+i);
									new_sock << "OK";
									break;
								}
								for ( unsigned short j=1; j<splitCommand.size(); j++ )
								{
									s<<splitCommand[j]<<" ";
								}
								simVector[i]->handleCommand(s.str());
								new_sock << "OK";
								break;
							}
						}
					}
				}
			}	
			catch ( SocketException& ) {}

		}
	}
	catch ( SocketException& e )
	{
		std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
	}

return 0;
}
