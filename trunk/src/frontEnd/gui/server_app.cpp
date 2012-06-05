#include "server_app.h"
#include <sstream>

serverAppDialog::serverAppDialog(QDialog *parent) : QDialog(parent)
{
	this->cancelled=0;
	QLabel *sec1=new QLabel("Seconds", this);
	QLabel *sec2=new QLabel("Seconds", this);
	app = new QLabel("Application Type:", this);
	appPort = new QLabel("Listening Port:", this);
	appStart = new QLabel("Start Time:", this);
	appStop = new QLabel("Stop Time :", this);

	QValidator *inputMask=new QIntValidator(1,1000000,this);
	QValidator *inputMask2=new QDoubleValidator(0.00001,100000000,7,this);

	add = new QPushButton("Add", this);
	cancel = new QPushButton("Cancel", this);


	appCombo = new QComboBox;
	appCombo->addItem(tr("UDP Echo Server"));
	appCombo->addItem(tr("TCP Sink"));
//	appCombo->addItem(tr("ICMP Echo Server"));

	le1 = new QLineEdit(this);
	le1->setValidator(inputMask);
	le1->setText("80");

	le2 = new QLineEdit(this);
	le2->setValidator(inputMask2);
	le2->setText("0.1");

	le3 = new QLineEdit(this);
	le3->setValidator(inputMask2);
	le3->setText("100");

	grid = new QGridLayout(); 

	grid->addWidget(app, 0, 0);
	grid->addWidget(appCombo, 0, 1);
	grid->addWidget(appPort, 1, 0);
	grid->addWidget(le1, 1, 1);
	grid->addWidget(appStart, 2, 0);
	grid->addWidget(sec1, 2, 2);
	grid->addWidget(le2, 2, 1);
	grid->addWidget(appStop, 3, 0);
	grid->addWidget(le3, 3, 1);
	grid->addWidget(sec2, 3, 2);
	grid->addWidget(add, 4, 0);
	grid->addWidget(cancel, 4, 1);

	connect(add, SIGNAL(clicked()), this, SLOT(add2()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

	setLayout(grid);
	this->setWindowTitle("Configure Client Application");
}

void serverAppDialog::add2()
{
	this->appType=appCombo->currentText().toStdString();
	std::istringstream buffer( le1->text().toStdString() );
	buffer >> this->port;
	buffer.clear();
	buffer.str(le2->text().toStdString());
	buffer >> this->startTime;
	buffer.clear();
	buffer.str(le3->text().toStdString());
	buffer >> this->stopTime;
	buffer.clear();

	this->hide();	
}

void serverAppDialog::cancelClicked()
{
	this->cancelled=1;
	this->hide();

}
