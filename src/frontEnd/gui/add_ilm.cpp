#include "add_ilm.h"
#include <sstream>

AddIlmWindow::AddIlmWindow( std::vector<std::string>& neighList, QDialog *parent ) : QDialog(parent)
{
	this->cancelled=0;
	this->fastreroute=0;
	incoming = new QLabel("Incoming Label:", this);
	next = new QLabel("Next Hop :", this);
	outgoing = new QLabel("Outgoing Label:", this);

	next2 = new QLabel("Next Hop :", this);
	outgoing2 = new QLabel("Outgoing Label:", this);
	next2->hide();
	outgoing2->hide();

	QIntValidator *inputMask=new QIntValidator(1,1000000,this);
	weightMask=new QDoubleValidator(0.000,1.000,3,this);
	weightMask->setNotation(QDoubleValidator::StandardNotation);
	weightMask->setTop(1.000);

	add = new QPushButton("Add", this);
	cancel = new QPushButton("Cancel", this);

	le1 = new QLineEdit(this);
	le1->setValidator(inputMask);

	nextHop = new QComboBox;
	for ( unsigned int i=0; i<neighList.size(); i++)
	{
		nextHop->addItem(tr(neighList[i].c_str()));
	}

	cb = new QCheckBox("Set Outgoing Label Same as Incoming Label", this);
	cb->setCheckState(Qt::Checked);	

	main = new QLabel(this);
	main->setTextFormat(Qt::RichText);
	main->setText("<b><u>Primary Path</u></b>");
	bcup = new QLabel(this);
	bcup->setTextFormat(Qt::RichText);
	bcup->setText("<b><u>Backup Path</u></b>");
	bcup->hide();
	le2 = new QLineEdit(this);
	le2->setValidator(inputMask);
	le2->setText(le1->text());

	weights = new QLabel("Weights  0<w<1 : ",this);
	weights->hide();
	le5=new QLineEdit(this);
	le5->setValidator(weightMask);
	le5->setInputMask("9.999;_");
	le5->hide();
	le6=new QLineEdit(this);
	le6->setValidator(weightMask);
	le6->setReadOnly(true);
	le6->hide();

	le3 = new QLineEdit(this);
	le3->setValidator(inputMask);
	le3->hide();


	nextHop2 = new QComboBox;
	for ( unsigned int i=0; i<neighList.size(); i++)
	{
		nextHop2->addItem(tr(neighList[i].c_str()));
	}
	nextHop2->hide();

	cb3 = new QCheckBox("Set Outgoing Label Same as Incoming Label", this);
	cb3->setCheckState(Qt::Checked);	
	cb3->hide();

	grid = new QGridLayout(); 

	grid->addWidget(incoming, 0, 0);
	grid->addWidget(le1, 0, 1);
	grid->addWidget(main,1,0);
	grid->addWidget(next, 2, 0);
	grid->addWidget(nextHop, 2, 1);
	grid->addWidget(cb,3,0);
	grid->addWidget(outgoing, 4, 0);
	grid->addWidget(le2, 4, 1);

	cb2 = new QCheckBox("Configure MPLS Fast Reroute", this);
	grid->addWidget(cb2, 5, 0);


	addPolicy=new QLabel(this);
	addPolicy->setTextFormat(Qt::RichText);
	addPolicy->setText("<b><u>Add Policy</u></b>");
	addPolicy->hide();
	attrValue=new QLabel("Value",this);
	attrValue->hide();
	policyType=new QComboBox;
	policyType->addItem(tr("Default Policy"));
	policyType->addItem(tr("Weighted Policy"));
	policyType->hide();
	policyAttr=new QComboBox;
	policyAttr->addItem(tr("Max Packets in Queue"));
	policyAttr->addItem(tr("Max Bytes in Queue"));
	policyAttr->hide();
	le4=new QLineEdit(this);
	le4->setValidator(inputMask);
	le4->hide();


	grid->addWidget(bcup,6,0);
	grid->addWidget(cb3,7,0);
	grid->addWidget(outgoing2,8,0);
	grid->addWidget(le3,8,1);
	grid->addWidget(next2,9,0);
	grid->addWidget(nextHop2,9,1);
	grid->addWidget(addPolicy, 10, 0);
	grid->addWidget(policyType, 11, 0);
	grid->addWidget(policyAttr, 11, 1);
	grid->addWidget(attrValue, 12, 0);
	grid->addWidget(le4, 12, 1);
	grid->addWidget(weights, 13, 0);
	grid->addWidget(le5, 14, 0);
	grid->addWidget(le6, 14, 1);

	grid->addWidget(add, 15, 0);
	grid->addWidget(cancel, 15, 1);

	connect(add, SIGNAL(clicked()), this, SLOT(add2()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	connect(cb, SIGNAL(stateChanged(int)), this, SLOT(setOutgoing(int)));
	connect(policyType, SIGNAL(activated(QString)), this, SLOT(showWeights(QString)));
	connect(cb3, SIGNAL(stateChanged(int)), this, SLOT(setOutgoing2(int)));
	connect(cb2, SIGNAL(stateChanged(int)), this, SLOT(fastReroute(int)));
	connect(le1, SIGNAL(textChanged(QString)), this, SLOT(incomingLabelChanged()));
	connect(le5, SIGNAL(textChanged(QString)), this, SLOT(weightsChanged()));

	setLayout(grid);
	this->setWindowTitle("Add Incoming Label Map");
}

void AddIlmWindow::add2()
{
	this->label1=le1->text().toInt();	
	this->label2=le2->text().toInt();	
	this->label3=le3->text().toInt();	
	this->nextHopNode=nextHop->currentText().toStdString();	
	this->nextHopNode2=nextHop2->currentText().toStdString();	
	this->policy=policyType->currentText().toStdString();
	this->policyAttribute=policyAttr->currentText().toStdString();
	this->policyAttributeValue=le4->text().toInt();
	std::istringstream ss;
	ss.str(le5->text().toStdString());
	ss>> this->weight1;
	ss.clear();
	ss.str(le6->text().toStdString());
	ss >> this->weight2;
	if ( policy == "Default Policy" )
	{
		policy = "DEFAULT";
	}
	else if ( policy == "Weighted Policy")
	{
		policy = "WEIGHTED";
	}
	else{}

	if ( policyAttribute == "Max Packets in Queue" )
	{
		policyAttribute = "PACKETS";
	}
	else if ( policyAttribute == "Max Bytes in Queue" )
	{
		policyAttribute = "BYTES";
	}
	else{}

	this->hide();	
}

void AddIlmWindow::setOutgoing(int state)
{
	if (state == Qt::Checked)
	{
		le2->setReadOnly(true);
	}
	else
	{
		le2->setReadOnly(false);
	}
}

void AddIlmWindow::setOutgoing2(int state)
{
	if (state == Qt::Checked)
	{
		le3->setReadOnly(true);
	}
	else
	{
		le3->setReadOnly(false);
	}
}
void AddIlmWindow::incomingLabelChanged()
{
	if (this->cb->checkState() == Qt::Checked)
	{
		le2->setText(le1->text());
		le2->setReadOnly(true);
	}

}

void AddIlmWindow::weightsChanged()
{
	float tmp;
	std::istringstream ss;
	ss.str(le5->text().toStdString());
	ss >> tmp;
	if ( tmp > 1 )
	{

		le5->setText("1");
		le6->setText("0");
	}
	else
	{
		ss.clear();
		tmp = 1-tmp;
		std::stringstream st;
		st<<tmp;
		
		le6->setText(st.str().c_str());
		le6->setReadOnly(true);
	}
}

void AddIlmWindow::fastReroute(int state)
{
	if (state == Qt::Checked )
	{
		fastreroute=1;
		next2->show();
		outgoing2->show();
		le3->show();
		cb3->show();
		nextHop2->show();
		bcup->show();
		addPolicy->show();
		policyType->show();
		policyAttr->show();
		le4->show();
		attrValue->show();
		repaint();

		grid->update();
	}
	else
	{
		fastreroute=0;
		next2->hide();
		outgoing2->hide();
		le3->hide();
		cb3->hide();
		nextHop2->hide();
		bcup->hide();
		addPolicy->hide();
		policyType->hide();
		policyAttr->hide();
		attrValue->hide();
		le4->hide();
		le5->hide();
		le6->hide();
		weights->hide();

		repaint();
		grid->update();
	}


}

void AddIlmWindow::showWeights(QString item)
{
	if ( item.toStdString() == "Weighted Policy" )
	{
		weights->show();
		le5->show();
		le6->show();

	}
	else
	{
		weights->hide();
		le5->hide();
		le6->hide();

	}
}

void AddIlmWindow::cancelClicked()
{
	this->cancelled=1;
	this->label1=0;
	this->label2=0;
	this->hide();
}

