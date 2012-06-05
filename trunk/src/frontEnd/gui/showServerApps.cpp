#include <showServerApps.h>
#include <QCheckBox>
#include <QLineEdit>
#include <sstream>
#include "editServerApp.h"

ServerAppStore::ServerAppStore(std::string nodeName, std::vector<TopologyMap::serverAppStruct*>* serverAppVector ,QDialog *parent ) : QDialog(parent)
{
	this->serverAppVector=serverAppVector;
	this->cancelled=1;
	grid = new QGridLayout(); 
	int a=0, b=0;

	std::stringstream s;
	for ( unsigned int i=0; i<(*serverAppVector).size(); i++)
	{
		QCheckBox *cb;
		if ( (*serverAppVector)[i]->nodeName == nodeName )
		{
			s.str("");
			if ( (*serverAppVector)[i]->appType == "UDP Echo Server" )
			{
				//QLabel* operation = new QLabel("UDP Echo Server", this);
				//grid->addWidget(operation, a, 0);

				s.str("");
				s<<"UDP Echo Server\n\tListening Port : "<<(*serverAppVector)[i]->port<<" \n\tApplication Start Time : "<<(*serverAppVector)[i]->startTime<<" Seconds \n\tApplication Stop Time : "<<(*serverAppVector)[i]->stopTime<<" Seconds";

				cb = new QCheckBox(s.str().c_str(), this);
				grid->addWidget(cb, a, 0);
				checkBx* cbEntry=new checkBx;
				cbEntry->cb=cb;
				cbEntry->index=(*serverAppVector)[i]->index;
				cbVector.push_back(cbEntry);

				QPushButton *editb=new QPushButton("Edit",this);
				editb->setCheckable(1);
				editb->setChecked(0);
				connect(editb, SIGNAL(clicked()), this, SLOT(edit()));
				appButton *btn=new appButton;

				btn->button=editb;
				btn->app=(*serverAppVector)[i];
				
				buttonVector.push_back(btn);
				grid->addWidget(editb, a, 1);
				a++;
			}
			if ( (*serverAppVector)[i]->appType == "TCP Sink" )
			{
				//QLabel* operation = new QLabel("UDP Echo Server", this);
				//grid->addWidget(operation, a, 0);

				s.str("");
				s<<"TCP Sink\n\tListening Port : "<<(*serverAppVector)[i]->port<<" \n\tApplication Start Time : "<<(*serverAppVector)[i]->startTime<<" Seconds \n\tApplication Stop Time : "<<(*serverAppVector)[i]->stopTime<<" Seconds";

				cb = new QCheckBox(s.str().c_str(), this);
				grid->addWidget(cb, a, 0);
				checkBx* cbEntry=new checkBx;
				cbEntry->cb=cb;
				cbEntry->index=(*serverAppVector)[i]->index;
				cbVector.push_back(cbEntry);

				QPushButton *editb=new QPushButton("Edit",this);
				editb->setCheckable(1);
				editb->setChecked(0);
				connect(editb, SIGNAL(clicked()), this, SLOT(edit()));
				appButton *btn=new appButton;

				btn->button=editb;
				btn->app=(*serverAppVector)[i];
				
				buttonVector.push_back(btn);
				grid->addWidget(editb, a, 1);
				a++;
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
	
	save2 = new QPushButton("Delete Checked", this);
	cancel2 = new QPushButton("Cancel", this);
	okb = new QPushButton("Ok", this);
	okb->setDefault(1);
	grid->addWidget(save2, a, b);
	grid->addWidget(cancel2, a, b+1);
	grid->addWidget(okb, a, b+2);
	
	connect(save2, SIGNAL(clicked()), this, SLOT(deleteEntry()));
	connect(cancel2, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(okb, SIGNAL(clicked()), this, SLOT(cancel()));

	this->setLayout(grid);
	s.str("");
	s<<"APPLICATIONS INSTALLED ON "<<nodeName;
	this->setWindowTitle(s.str().c_str());
}
ServerAppStore::~ServerAppStore()
{


}

void ServerAppStore::deleteEntry()
{
	for ( unsigned int i=0; i<cbVector.size(); i++ )
	{
		if ( cbVector[i]->cb->checkState()== Qt::Checked ) 
		{
			for ( unsigned int j=0; j<(*serverAppVector).size(); j++ )
			{
				if ( (*serverAppVector)[j]->index==cbVector[i]->index )
				{
					delete (*serverAppVector)[j];
					(*serverAppVector).erase((*serverAppVector).begin()+j);	
					break;
				}
			}
		}
	}
	this->hide();

}
void ServerAppStore::cancel()
{
	this->hide();
}

void ServerAppStore::save()
{
	this->cancelled=0;
}

void ServerAppStore::edit()
{
	TopologyMap::serverAppStruct* activeApp;
	bool chk=0;

	for ( unsigned int i=0; i<buttonVector.size(); i++)
	{
		if ( buttonVector[i]->button->isChecked() )
		{
			activeApp=buttonVector[i]->app;
			buttonVector[i]->button->setChecked(0);
			chk=1;
			break;
		}
		else
		{
			activeApp=NULL;
		}
			 
	}
	
	if ( chk==1 )
	{
		EditServerApp *appW=new EditServerApp(activeApp);
		appW->exec();
		delete appW;
		appW=NULL;

		for ( unsigned int i=0; i<cbVector.size(); i++ )
		{
			if ( cbVector[i]->index == activeApp->index )
			{
				std::stringstream s;
				s<<"UDP Echo Server\n\tListening Port : "<<activeApp->port<<" \n\tApplication Start Time : "<<activeApp->startTime<<"\n\tApplication Stop Time : "<<activeApp->stopTime;
				cbVector[i]->cb->setText(s.str().c_str());
				
				break;
			}
		}
	
	}

	okb->setFocus();
}
