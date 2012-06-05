#include <showClientApps.h>
#include <QCheckBox>
#include <QLineEdit>
#include <sstream>

ClientAppStore::ClientAppStore(std::string nodeName, std::vector<TopologyMap::clientAppStruct*>* clientAppVector ,QDialog *parent ) : QDialog(parent)
{
	this->clientAppVector=clientAppVector;
	this->cancelled=1;
	grid = new QGridLayout(); 
	int a=0, b=0;

	std::stringstream s;
	for ( unsigned int i=0; i<(*clientAppVector).size(); i++)
	{
		QCheckBox *cb;
		if ( (*clientAppVector)[i]->nodeName == nodeName )
		{
			s.str("");
			if ( (*clientAppVector)[i]->appType == "UDP Echo" )
			{
				s.str("");
				s<<"UDP Echo Client\nDestination Server : "<<(*clientAppVector)[i]->destServer<<"\nDestination Port : "<<(*clientAppVector)[i]->destPort<<"\nPacket Size : "<<(*clientAppVector)[i]->packetSize<<"\nNumber of Packets : " <<(*clientAppVector)[i]->packetNum<<"\nPacket Interval : "<<(*clientAppVector)[i]->interval<<" \nApplication Start Time : "<<(*clientAppVector)[i]->startTime<<"\nApplication Stop Time : "<<(*clientAppVector)[i]->stopTime;

				cb = new QCheckBox(s.str().c_str(), this);
				grid->addWidget(cb, a, 0);
				a++;
				checkBx* cbEntry=new checkBx;
				cbEntry->cb=cb;
				cbEntry->index=(*clientAppVector)[i]->index;
				cbVector.push_back(cbEntry);
			}
			else if ( (*clientAppVector)[i]->appType == "Random TCP Bursts" )
			{
				s.str("");
				s<<"Random TCP Bursts\nDestination Sink : "<<(*clientAppVector)[i]->destServer<<"\nDestination Port : "<<(*clientAppVector)[i]->destPort<<"\nPacket Size : "<<(*clientAppVector)[i]->packetSize<<"\nNumber of Packets : " <<(*clientAppVector)[i]->packetNum<<" \nApplication Start Time : "<<(*clientAppVector)[i]->startTime<<"\nApplication Stop Time : "<<(*clientAppVector)[i]->stopTime;

				cb = new QCheckBox(s.str().c_str(), this);
				grid->addWidget(cb, a, 0);
				a++;
				checkBx* cbEntry=new checkBx;
				cbEntry->cb=cb;
				cbEntry->index=(*clientAppVector)[i]->index;
				cbVector.push_back(cbEntry);
			}
			else
			{}
		}
	}	

	if(a==0)
	{
		QLabel* Error = new QLabel("NO APPLICATIONS INSTALLED", this);
		grid->addWidget(Error, 0, 0);
		a++;	
	}
	else
	{
		/*	QLabel* titles = new QLabel("Application Type    : ", this);
		grid->addWidget(titles, 0, 0);
		b++;

		QLabel* title2 = new QLabel(" :    DETAILS ", this);
		grid->addWidget(title2, 0, 1);*/
	}

	save2 = new QPushButton("Delete Checked", this);
	cancel2 = new QPushButton("Cancel", this);
	cancel2->setDefault(1);
	grid->addWidget(save2, a, b);
	grid->addWidget(cancel2, a, b+1);
	
	connect(save2, SIGNAL(clicked()), this, SLOT(deleteEntry()));
	connect(cancel2, SIGNAL(clicked()), this, SLOT(cancel()));
	
	this->setLayout(grid);
	s.str("");
	s<<"APPLICATIONS INSTALLED ON "<<nodeName;
	this->setWindowTitle(s.str().c_str());
}
ClientAppStore::~ClientAppStore()
{


}

void ClientAppStore::deleteEntry()
{
	for ( unsigned int i=0; i<cbVector.size(); i++ )
	{
		if ( cbVector[i]->cb->checkState()== Qt::Checked ) 
		{
			for ( unsigned int j=0; j<(*clientAppVector).size(); j++ )
			{
				if ( (*clientAppVector)[j]->index==cbVector[i]->index )
				{
					delete (*clientAppVector)[j];
					(*clientAppVector).erase((*clientAppVector).begin()+j);	
					break;
				}
			}
		}
	}
	this->hide();

}
void ClientAppStore::cancel()
{
	this->hide();
}

void ClientAppStore::save()
{
	this->cancelled=0;
}
