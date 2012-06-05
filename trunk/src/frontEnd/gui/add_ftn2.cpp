#include "add_ftn2.h"

AddFtnDialog2::AddFtnDialog2(std::vector<std::string>& neighList,QDialog *parent) : QDialog(parent)
{
	this->cancelled=0;
	source = new QLabel("Source Address:", this);
	destination = new QLabel("Destination Address:", this);
	label = new QLabel("Label:", this);
	next = new QLabel("Next Hop :", this);

	QValidator *inputMask=new QIntValidator(1,1000000,this);

	add = new QPushButton("Add", this);
	cancel = new QPushButton("Cancel", this);
	
	destinationB = new QPushButton("Select Destination Node", this);
	sourceB = new QPushButton("Select Source Node", this);
	

	nextHop = new QComboBox;
	for ( unsigned int i=0; i<neighList.size(); i++)
	{
		nextHop->addItem(tr(neighList[i].c_str()));
	}

	le3 = new QLineEdit(this);
	le3->setValidator(inputMask);

	grid = new QGridLayout(); 

	grid->addWidget(source, 0, 0);
	grid->addWidget(sourceB, 0, 1);
	grid->addWidget(destination, 1, 0);
	grid->addWidget(destinationB, 1, 1);
	grid->addWidget(label, 2, 0);
	grid->addWidget(le3, 2, 1);
	grid->addWidget(next, 3, 0);
	grid->addWidget(nextHop, 3, 1);
	grid->addWidget(add, 4, 0);
	grid->addWidget(cancel, 4, 1);


	connect(add, SIGNAL(clicked()), this, SLOT(add2()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

	setLayout(grid);
	this->setWindowTitle("Add FEC to NHLFE Map");
}

void AddFtnDialog2::add2()
{
	//this->sourceAddr=le1->text().toStdString();	
	//this->destAddr=le2->text().toStdString();	
	this->outLabel=le3->text().toStdString();	
	this->nextHopNode=nextHop->currentText().toStdString();
	this->hide();	
}

void AddFtnDialog2::cancelClicked()
{
	this->cancelled=1;
	this->hide();

}
