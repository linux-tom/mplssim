#include "editServerApp.h"
#include <sstream> 

EditServerApp::EditServerApp(TopologyMap::serverAppStruct* serverApp,QDialog* parent) : QDialog (parent)
{
	std::stringstream s;
	this->serverApp=serverApp;	

	QValidator *inputMask=new QIntValidator(1,65535,this);
	QValidator *inputMask2=new QDoubleValidator(0.00001,100000000,7,this);

	appCombo = new QComboBox;
	appCombo->addItem(tr("UDP Echo Server"));
	appCombo->addItem(tr("TCP Sink"));
	//appCombo->addItem(tr("ICMP Echo Server"));

	portL=new QLabel("Port",this);
	startL=new QLabel("Start Time",this);
	stopL=new QLabel("Stop Time",this);

	QLabel *sec1=new QLabel("Seconds",this);
	QLabel *sec2=new QLabel("Seconds",this);
	QLabel *appL=new QLabel("Application Type",this);

	s.str("");
	s<<serverApp->port;
	portE = new QLineEdit(this);
	portE->setValidator(inputMask);
	portE->setText(s.str().c_str());

	s.str("");
	s<<serverApp->startTime;
	startE = new QLineEdit(this);
	startE->setValidator(inputMask2);
	startE->setText(s.str().c_str());

	s.str("");
	s<<serverApp->stopTime;
	stopE = new QLineEdit(this);
	stopE->setValidator(inputMask2);
	stopE->setText(s.str().c_str());

	add = new QPushButton("Save", this);
	cancel = new QPushButton("Cancel", this);

	grid = new QGridLayout(); 
	grid->addWidget(appL, 0, 0);
	grid->addWidget(appCombo, 0, 1);
	grid->addWidget(portL, 1, 0);
	grid->addWidget(portE, 1, 1);
	grid->addWidget(startL, 2, 0);
	grid->addWidget(sec1, 2, 2);
	grid->addWidget(startE, 2, 1);
	grid->addWidget(stopL, 3, 0);
	grid->addWidget(stopE, 3, 1);
	grid->addWidget(sec2, 3, 2);
	grid->addWidget(add, 4, 0);
	grid->addWidget(cancel, 4, 1);
	
	this->setLayout(grid);
	s.str("");
	s<<"Edit Application on "<<serverApp->nodeName;
	this->setWindowTitle(s.str().c_str());
	
	connect(add, SIGNAL(clicked()), this, SLOT(save()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelAct()));

}
EditServerApp::~EditServerApp()
{
}

void EditServerApp::save()
{
	this->serverApp->appType=appCombo->currentText().toStdString();
	std::istringstream buffer( portE->text().toStdString() );
	buffer >> (this->serverApp)->port;
	buffer.clear();
	buffer.str(startE->text().toStdString());
	buffer >> serverApp->startTime;
	buffer.clear();
	buffer.str(stopE->text().toStdString());
	buffer >> serverApp->stopTime;
	buffer.clear();

	this->hide();	
}

void EditServerApp::cancelAct()
{
	this->hide();
}
