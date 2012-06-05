#include "client_app.h"
#include <sstream>
//clientAppDialog(std::vector<std::string>& serverList,  QDialog *parent = 0);
clientAppDialog::clientAppDialog(std::vector<std::string>& serverList, QDialog *parent) : QDialog(parent)
{
	this->cancelled=0;
	QLabel *sec1=new QLabel("Seconds", this);
	QLabel *sec2=new QLabel("Seconds", this);
	QLabel *sec3=new QLabel("MilliSeconds", this);
	QLabel *bytes=new QLabel("Bytes", this);
	app = new QLabel("Application Type:", this);
	serverLabel = new QLabel("Server:", this);
	appPort = new QLabel("Destination Port:", this);
	appStart = new QLabel("Start Time:", this);
	appStop = new QLabel("Stop Time :", this);
	packSize = new QLabel("Packet Size :", this);
	packNum = new QLabel("No. of Packets to Send :", this);
	appInterval = new QLabel("Period Between Packets :", this);

	QValidator *inputMask=new QIntValidator(1,1000000000,this);
	QValidator *timeMask=new QDoubleValidator(0.0000001,1000000000,6,this);

	add = new QPushButton("Add", this);
	cancel = new QPushButton("Cancel", this);


	serverCombo = new QComboBox;
	if ( serverList.size() == 0 )
	{
		serverCombo->addItem("");
	}
	else
	{
		for ( unsigned int i=0; i<serverList.size(); i++)
		{
			serverCombo->addItem(tr(serverList[i].c_str()));
		}
	}

	appCombo = new QComboBox;
	appCombo->addItem(tr("UDP Echo"));
	appCombo->addItem(tr("Random TCP Bursts"));
//	appCombo->addItem(tr("ICMP"));

	le1 = new QLineEdit(this);
	le1->setValidator(inputMask);
	le1->setText("80");

	le2 = new QLineEdit(this);
	le2->setValidator(timeMask);
	le2->setText("1");

	le3 = new QLineEdit(this);
	le3->setValidator(timeMask);
	le3->setText("100");

	le4 = new QLineEdit(this);
	le4->setValidator(inputMask);
	le4->setText("1024");

	le5 = new QLineEdit(this);
	le5->setValidator(inputMask);
	le5->setText("1000");

	le6 = new QLineEdit(this);
	le6->setValidator(timeMask);
	le6->setText("1");

	grid = new QGridLayout(); 

	grid->addWidget(app, 0, 0);
	grid->addWidget(appCombo, 0, 1);
	grid->addWidget(serverLabel, 1, 0);
	grid->addWidget(serverCombo, 1, 1);
	grid->addWidget(appPort, 2, 0);
	grid->addWidget(le1, 2, 1);
	grid->addWidget(appStart, 3, 0);
	grid->addWidget(sec1, 3, 2);
	grid->addWidget(le2, 3, 1);
	grid->addWidget(appStop, 4, 0);
	grid->addWidget(le3, 4, 1);
	grid->addWidget(sec2, 4, 2);
	grid->addWidget(packSize, 5, 0);
	grid->addWidget(le4, 5, 1);
	grid->addWidget(bytes, 5, 2);
	grid->addWidget(packNum, 6, 0);
	grid->addWidget(le5, 6, 1);
	grid->addWidget(appInterval, 7, 0);
	grid->addWidget(sec3, 7, 2);
	grid->addWidget(le6, 7, 1);
	grid->addWidget(add, 8, 0);
	grid->addWidget(cancel, 8, 1);


	connect(add, SIGNAL(clicked()), this, SLOT(add2()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

	setLayout(grid);
	this->setWindowTitle("Configure Client Application");
}

clientAppDialog::~clientAppDialog()
{

}

void clientAppDialog::add2()
{
	this->appType=appCombo->currentText().toStdString();
	this->server=serverCombo->currentText().toStdString();
	std::istringstream buffer( le1->text().toStdString() );
	buffer >> this->port;
	buffer.clear();
	buffer.str(le2->text().toStdString());
	buffer >> this->startTime;
	buffer.clear();
	buffer.str(le3->text().toStdString());
	buffer >> this->stopTime;
	buffer.clear();
	buffer.str(le4->text().toStdString());
	buffer >> this->packetSize;
	buffer.clear();
	buffer.str(le5->text().toStdString());
	buffer >> this->numPackets;
	buffer.clear();
	buffer.str(le6->text().toStdString());
	buffer >> this->interval;
	buffer.clear();

	this->hide();	
}

void clientAppDialog::cancelClicked()
{
	this->cancelled=1;
	this->hide();

}
