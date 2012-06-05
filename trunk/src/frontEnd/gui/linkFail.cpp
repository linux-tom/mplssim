#include "linkFail.h"
#include <sstream>

LinkFail::LinkFail(QDialog *parent) : QDialog(parent)
{
	this->cancelled=1;
	this->resume=0;

	failL = new QLabel("Fail At :", this);
	resumeL = new QLabel("Restore At :", this);
	QLabel* sec1 = new QLabel("Seconds", this);
	QLabel* sec2 = new QLabel("Seconds", this);

	QValidator *inputMask=new QDoubleValidator(0,1000000,5,this);

	add = new QPushButton("Schedule", this);
	cancel = new QPushButton("Cancel", this);

	le1 = new QLineEdit(this);
	le1->setValidator(inputMask);
	le1->setText("0");

	le2 = new QLineEdit(this);
	le2->setValidator(inputMask);
	le2->setText("0");
	le2->setReadOnly(true);

	cb = new QCheckBox("Restore Link?", this);
	connect(cb, SIGNAL(stateChanged(int)), this, SLOT(restore(int)));

	grid = new QGridLayout(); 

	grid->addWidget(failL, 0, 0);
	grid->addWidget(le1, 0, 1);
	grid->addWidget(sec1, 0, 2);
	grid->addWidget(cb, 1, 0);
	grid->addWidget(resumeL, 2, 0);
	grid->addWidget(le2, 2, 1);
	grid->addWidget(sec2, 0, 2);
	grid->addWidget(add, 3, 0);
	grid->addWidget(cancel, 3, 1);

	connect(add, SIGNAL(clicked()), this, SLOT(add2()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

	setLayout(grid);
	this->setWindowTitle("Schedule Link Failure");
}

void LinkFail::add2()
{
	this->cancelled=0;
	std::istringstream buffer( le1->text().toStdString() );
	buffer >> failTime;
	buffer.clear();
	buffer.str(le2->text().toStdString());
	if (cb->checkState() == Qt::Checked)
	{
		this->resume=1;
		buffer >> resumeTime;
	}
	else
	{
		this->resume=0;
		resumeTime=0;
	}
	buffer.clear();

	this->hide();	
}

void LinkFail::cancelClicked()
{
	this->cancelled=1;
	this->hide();
}

void LinkFail::restore(int state)
{
	if (state == Qt::Checked)
	{
		le2->setReadOnly(false);
	}
	else
	{
		le2->setReadOnly(true);
	}
}
