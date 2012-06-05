#include "linkParam.h"
#include <sstream>

linkDialog::linkDialog(std::string* bw, std::string* delay, QDialog *parent) : QDialog(parent)
{
	this->cancelled=0;
	this->linkBW=bw;
	this->linkDelay=delay;
	bandwidth = new QLabel("Link Bandwidth :", this);
	latency = new QLabel("Latency :", this);

	QValidator *inputMask=new QIntValidator(1,1000000,this);

	add = new QPushButton("Set", this);
	cancel = new QPushButton("Cancel", this);

	le1 = new QLineEdit(this);
	//le1->setValidator(inputMask);
	le1->setText((*bw).c_str());

	le2 = new QLineEdit(this);
	//le2->setValidator(inputMask);
	le2->setText((*delay).c_str());

	grid = new QGridLayout(); 

	grid->addWidget(bandwidth, 0, 0);
	grid->addWidget(le1, 0, 1);
	grid->addWidget(latency, 1, 0);
	grid->addWidget(le2, 1, 1);
	grid->addWidget(add, 2, 0);
	grid->addWidget(cancel, 2, 1);

	connect(add, SIGNAL(clicked()), this, SLOT(add2()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

	setLayout(grid);
	this->setWindowTitle("Set Link Parameters");
}

void linkDialog::add2()
{
	std::istringstream buffer( le1->text().toStdString() );
	*this->linkBW=le1->text().toStdString();
	//string.erase(std::remove_if(string.begin(), string.end(), std::isspace), string.end());
	buffer.clear();
	buffer.str(le2->text().toStdString());
	*this->linkDelay=le2->text().toStdString();
	buffer.clear();

	this->hide();	
}

void linkDialog::cancelClicked()
{
	this->cancelled=1;
	this->hide();
}
