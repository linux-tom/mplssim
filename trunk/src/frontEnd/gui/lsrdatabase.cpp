#include <lsrdatabase.h>
#include <QCheckBox>
#include <QLineEdit>
#include <sstream>

LsrDB::LsrDB(std::string nodeName, std::vector<TopologyMap::ilmStruct*>* ilmVector, std::vector<TopologyMap::ftnStruct*>* ftnVector  ,QDialog *parent ) : QDialog(parent)
{
	this->ilmVector=ilmVector;
	this->ftnVector=ftnVector;
	this->cancelled=1;
	grid = new QGridLayout(); 
	int a=0, b=0;

	std::stringstream s;

	for ( unsigned int i=0; i<(*ilmVector).size(); i++)
	{
		QCheckBox *cb;
		if ( (*ilmVector)[i]->nodeName == nodeName )
		{
			if ( (*ilmVector)[i]->operation == "POP" )
			{
				//QLabel* operation = new QLabel("POP", this);
				//grid->addWidget(operation, a, 0);

				s.str("");
				s<<"POP\nIncoming Label = "<<(*ilmVector)[i]->incomingLabel;
				cb = new QCheckBox(s.str().c_str(), this);
				grid->addWidget(cb, a, 0);
				a++;
				checkBx* cbEntry=new checkBx;
				cbEntry->cb=cb;
				cbEntry->index=(*ilmVector)[i]->index;
				cbVector.push_back(cbEntry);
			}
			else if ( (*ilmVector)[i]->operation == "SWAP" )
			{
				//QLabel* operation = new QLabel("SWAP", this);
				//grid->addWidget(operation, a, 0);
				b++;
				s.str("");
				s<<"SWAP\nIncoming Label = "<<(*ilmVector)[i]->incomingLabel<<"\nOutgoing Label = " <<(*ilmVector)[i]->outgoingLabel<<"\nNext Hop = "<<(*ilmVector)[i]->nextHop;
		
				cb = new QCheckBox(s.str().c_str(), this);

				grid->addWidget(cb, a, 0);
				a++;
				checkBx* cbEntry=new checkBx;
				cbEntry->cb=cb;
				cbEntry->index=(*ilmVector)[i]->index;
				cbVector.push_back(cbEntry);
			}
			else
			{}
		}
	}	

	for ( unsigned int i=0; i<(*ftnVector).size(); i++)
	{
		QCheckBox *cb;
		std::stringstream s;
		if ( (*ftnVector)[i]->nodeName == nodeName )
		{
			/*QLabel* operation = new QLabel("FTN", this);
			grid->addWidget(operation, a, 0);*/

			s.str("");
			s<<"FEC To NHLFE\nIncoming IP = "<<(*ftnVector)[i]->sourceIP<<"\nDestination IP = "<<(*ftnVector)[i]->destinationIP<<"\nPush Label = "<<(*ftnVector)[i]->label <<"\nNext Hop = "<<(*ftnVector)[i]->nextHop;

			cb = new QCheckBox(s.str().c_str(), this);

			grid->addWidget(cb, a,0);
			a++;
			checkBx* cbEntry=new checkBx;
			cbEntry->cb=cb;
			cbEntry->index=(*ftnVector)[i]->index;
			cbVector.push_back(cbEntry);
		}
	}	

	if(a==0)
	{
		QLabel* Error = new QLabel("NO ENTRIES IN LSR DATABASE", this);
		grid->addWidget(Error, 0, 0);
		a++;	
	}
	else
	{
		/*	QLabel* titles = new QLabel("OPERATION    : ", this);
		grid->addWidget(titles, 0, 0);
		b++;

		QLabel* title2 = new QLabel(" :    DETAILS ", this);
		grid->addWidget(title2, 0, 1);*/
	}

	save2 = new QPushButton("Delete Checked", this);
	cancel2 = new QPushButton("Cancel", this);
	grid->addWidget(save2, a, b);
	grid->addWidget(cancel2, a, b+1);
	
	connect(save2, SIGNAL(clicked()), this, SLOT(deleteEntry()));
	connect(cancel2, SIGNAL(clicked()), this, SLOT(cancel()));
	
	this->setLayout(grid);
	s.str("");
	s<<"LSR DATABASE FOR "<<nodeName;
	this->setWindowTitle(s.str().c_str());
}
LsrDB::~LsrDB()
{


}

void LsrDB::deleteEntry()
{
	for ( unsigned int i=0; i<cbVector.size(); i++ )
	{
		if ( cbVector[i]->cb->checkState()== Qt::Checked ) 
		{
			for ( unsigned int j=0; j<(*ilmVector).size(); j++ )
			{
				if ( (*ilmVector)[j]->index==cbVector[i]->index )
				{
					delete (*ilmVector)[j];
					(*ilmVector).erase((*ilmVector).begin()+j);	
					break;
				}
			}
			for ( unsigned int j=0; j<(*ftnVector).size(); j++ )
			{
				if ( (*ftnVector)[j]->index==cbVector[i]->index )
				{
					delete (*ftnVector)[j];
					(*ftnVector).erase((*ftnVector).begin()+j);	
					break;
				}
			}
					
		}
	}
	this->hide();

}
void LsrDB::cancel()
{
	this->hide();
}

void LsrDB::save()
{
	this->cancelled=0;
}
