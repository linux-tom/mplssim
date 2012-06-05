#include "add_ftn.h"
#include <sstream>

AddFtnDialog::AddFtnDialog(std::vector<std::string>& neighList,QDialog *parent) : QDialog(parent)
{
	this->cancelled=0;
	this->destPort=0;
	this->srcPort=0;
	this->sourceAddr="0000";
	this->destAddr="0000";
	this->outLabel=1000000;
	
	//source = new QLabel("Source Address:", this);
	//destination = new QLabel("Destination Address:", this);
	//labelTCP = new QLabel("TCP Destination Port :", this);
	//labelUDP = new QLabel("UDP Destination Port :", this);
	label = new QLabel("Label:", this);
	next = new QLabel("Next Hop :", this);

	sIPcb=new QCheckBox("Source Address", this);
	dIPcb=new QCheckBox("Destination Address", this);
	uPcb=new QCheckBox("UDP Destination Port", this);
	tPcb=new QCheckBox("TCP Destination Port", this);
	suPcb=new QCheckBox("UDP Source Port", this);
	stPcb=new QCheckBox("TCP Source Port", this);

	QValidator *inputMask=new QIntValidator(1,1000000,this);
	QValidator *portMask=new QIntValidator(1,65535,this);
	QDoubleValidator *weightMask=new QDoubleValidator(0.000,1.000,3,this);
	weightMask->setNotation(QDoubleValidator::StandardNotation);
	weightMask->setTop(1.000);

	add = new QPushButton("Add", this);
	cancel = new QPushButton("Cancel", this);


	nextHop = new QComboBox;
	for ( unsigned int i=0; i<neighList.size(); i++)
	{
		nextHop->addItem(tr(neighList[i].c_str()));
	}

	nextHop2 = new QComboBox;
	for ( unsigned int i=0; i<neighList.size(); i++)
	{
		nextHop2->addItem(tr(neighList[i].c_str()));
	}


	le1 = new QLineEdit(this);
	le1->setInputMask("999.999.999.999/99;_");

	le2 = new QLineEdit(this);
	le2->setInputMask("999.999.999.999/99;_");

	le3 = new QLineEdit(this);
	le3->setValidator(inputMask);
	le8 = new QLineEdit(this);
	le8->setValidator(portMask);
	le9 = new QLineEdit(this);
	le9->setValidator(portMask);
	le10 = new QLineEdit(this);
	le10->setValidator(portMask);
	le11 = new QLineEdit(this);
	le11->setValidator(portMask);

	label2 = new QLabel("Label:", this);
	next2 = new QLabel("Next Hop :", this);
	le4 = new QLineEdit(this);
	le4->setValidator(inputMask);
	le5 = new QLineEdit(this);
	le5->setValidator(inputMask);
	le6 = new QLineEdit(this);
	le6->setValidator(weightMask);
	le7 = new QLineEdit(this);
	le7->setValidator(weightMask);

	grid = new QGridLayout(); 
	cb=new QCheckBox("Configure Fast Reroute", this);
	
	main = new QLabel(this);
	main->setTextFormat(Qt::RichText);
	main->setText("<b><u>Primary Path</u></b>");
	bcup = new QLabel(this);
	bcup->setTextFormat(Qt::RichText);
	bcup->setText("<b><u>Backup Path</u></b>");
	polic = new QLabel(this);
	polic->setTextFormat(Qt::RichText);
	polic->setText("<b><u>Add Policy</u></b>");
	attrValue=new QLabel("Value",this);
	weights=new QLabel("Weights : 0<w<1",this);
	policyType=new QComboBox(this);
	policyType->addItem(tr("Default Policy"));
	policyType->addItem(tr("Weighted Policy"));
	policyAttr=new QComboBox;
	policyAttr->addItem(tr("Max Packets in Queue"));
	policyAttr->addItem(tr("Max Bytes in Queue"));
	connect(le6, SIGNAL(textChanged(QString)), this, SLOT(weightsChanged()));

	bcup->hide();
	label2->hide();
	nextHop2->hide();
	next2->hide();
	le4->hide();
	le5->hide();
	le6->hide();
	le7->hide();
	policyAttr->hide();
	policyType->hide();
	attrValue->hide();
	polic->hide();
	attrValue->hide();
	weights->hide();

	//grid->addWidget(source, 0, 0);
	grid->addWidget(sIPcb, 0, 0);
	grid->addWidget(le1, 0, 1);
	//grid->addWidget(destination, 1, 0);
	grid->addWidget(dIPcb, 1, 0);
	grid->addWidget(le2, 1, 1);
	grid->addWidget(suPcb, 2, 0);
	grid->addWidget(le10, 2, 1);
	grid->addWidget(stPcb, 3, 0);
	grid->addWidget(le11, 3, 1);
	grid->addWidget(uPcb, 4, 0);
	grid->addWidget(le8, 4, 1);
	grid->addWidget(tPcb, 5, 0);
	grid->addWidget(le9, 5, 1);
	grid->addWidget(main,6,0);
	grid->addWidget(label, 7, 0);
	grid->addWidget(le3, 7, 1);
	grid->addWidget(next, 8, 0);
	grid->addWidget(nextHop, 8, 1);
	grid->addWidget(cb,9,0);
	grid->addWidget(bcup,10,0);
	grid->addWidget(label2, 11, 0);
	grid->addWidget(le4, 11, 1);
	grid->addWidget(next2, 12, 0);
	grid->addWidget(nextHop2, 12, 1);
	grid->addWidget(polic,13,0);
	grid->addWidget(policyType,14,0);
	grid->addWidget(policyAttr,14,1);
	grid->addWidget(attrValue,15,0);
	grid->addWidget(le5,15,1);
	grid->addWidget(weights,16,0);
	grid->addWidget(le6,17,0);
	grid->addWidget(le7,17,1);
	grid->addWidget(add, 18, 0);
	grid->addWidget(cancel, 18, 1);

	le1->setReadOnly(1);
	le2->setReadOnly(1);
	le8->setReadOnly(1);
	le9->setReadOnly(1);
	le10->setReadOnly(1);
	le11->setReadOnly(1);

	connect(add, SIGNAL(clicked()), this, SLOT(add2()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	connect(cb, SIGNAL(stateChanged(int)), this, SLOT(fastReroute(int)));
	connect(sIPcb, SIGNAL(stateChanged(int)), this, SLOT(enableSIP(int)));
	connect(dIPcb, SIGNAL(stateChanged(int)), this, SLOT(enableDIP(int)));
	connect(uPcb, SIGNAL(stateChanged(int)), this, SLOT(enableUP(int)));
	connect(tPcb, SIGNAL(stateChanged(int)), this, SLOT(enableTP(int)));
	connect(suPcb, SIGNAL(stateChanged(int)), this, SLOT(enableSUP(int)));
	connect(stPcb, SIGNAL(stateChanged(int)), this, SLOT(enableSTP(int)));
	connect(policyType, SIGNAL(activated(QString)), this, SLOT(showWeights(QString)));

	setLayout(grid);
	this->setWindowTitle("Add FEC to NHLFE Map");
}

void AddFtnDialog::fastReroute(int state)
{
	if ( state == Qt::Checked )
	{
		fastreroute=1;
		bcup->show();
		label2->show();
		nextHop2->show();
		next2->show();
		le4->show();
		le5->show();
		policyAttr->show();
		policyType->show();
		attrValue->show();
		polic->show();
		attrValue->show();
	}
	else
	{
		fastreroute=0;
		bcup->hide();
		label2->hide();
		nextHop2->hide();
		next2->hide();
		le4->hide();
		le5->hide();
		le6->hide();
		le7->hide();
		policyAttr->hide();
		policyType->hide();
		attrValue->hide();
		polic->hide();
		attrValue->hide();
		weights->hide();
	}


}

void AddFtnDialog::add2()
{
	std::stringstream FECstate;
	if (sIPcb->isChecked() )
	{
		FECstate<<"SIP";	
	}

	if (dIPcb->isChecked() )
	{
		FECstate<<"DIP";	
	}

	if (suPcb->isChecked() )
	{
		FECstate<<"SUP";	
	}

	if (stPcb->isChecked() )
	{
		FECstate<<"STP";	
	}

	if (uPcb->isChecked() )
	{
		FECstate<<"UP";	
	}

	if (tPcb->isChecked() )
	{
		FECstate<<"TP";	
	}

	fec=FECstate.str();
	std::cout << fec << std::endl;

	this->sourceAddr=le1->text().toStdString();	
	this->destAddr=le2->text().toStdString();	
	this->outLabel=le3->text().toInt();	
	this->nextHopNode=nextHop->currentText().toStdString();

	this->outLabel2=le4->text().toInt();	
	this->nextHopNode2=nextHop2->currentText().toStdString();	
	this->policy=policyType->currentText().toStdString();
	this->policyAttribute=policyAttr->currentText().toStdString();
	this->policyAttributeValue=le5->text().toInt();
	/*std::istringstream ss;
	ss>> this->weight1;
	ss.clear();
	ss >> this->weight2;
	ss.str(le7->text().toStdString());*/
	if (tPcb->isChecked() )
	{
		this->destPort=le9->text().toInt();
	}
	if (uPcb->isChecked() )
	{
		this->destPort=le8->text().toInt();
	}

	if (stPcb->isChecked() )
	{
		this->srcPort=le11->text().toInt();
	}

	if (suPcb->isChecked() )
	{
		this->srcPort=le10->text().toInt();
	}

	weight1=le6->text().toDouble();
	weight2=le7->text().toDouble();

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

void AddFtnDialog::cancelClicked()
{
	this->cancelled=1;
	this->hide();

}

void AddFtnDialog::weightsChanged()
{
	float tmp;
	std::istringstream ss;
	ss.str(le6->text().toStdString());
	ss >> tmp;
	if ( tmp > 1 )
	{
		le6->setText("1");
		le7->setText("0");
	}
	else
	{
		ss.clear();
		tmp = 1-tmp;
		std::stringstream st;
		st<<tmp;
		
		le7->setText(st.str().c_str());
		le7->setReadOnly(true);
	}
}

void AddFtnDialog::showWeights(QString item)
{
	if ( item.toStdString() == "Weighted Policy" )
	{
		weights->show();
		le6->show();
		le7->show();
	}
	else
	{
		weights->hide();
		le7->hide();
		le6->hide();

	}
}

void AddFtnDialog::enableSIP(int state)
{
	if ( state == Qt::Checked )
	{
		le1->setReadOnly(0);
	}
	else
	{
		le1->setReadOnly(1);
	}
}
void AddFtnDialog::enableDIP(int state)
{

	if ( state == Qt::Checked )
	{
		le2->setReadOnly(0);
	}
	else
	{
		le2->setReadOnly(1);
	}
}
void AddFtnDialog::enableUP(int state)
{

	if ( state == Qt::Checked )
	{
		le8->setReadOnly(0);
		tPcb->setCheckState(Qt::Unchecked);
		le8->setText("80");
	}
	else
	{
		le8->setReadOnly(1);
		le8->clear();
	}
}
void AddFtnDialog::enableTP(int state)
{
	if ( state == Qt::Checked )
	{
		le9->setReadOnly(0);
		uPcb->setCheckState(Qt::Unchecked);
		le9->setText("80");
	}
	else
	{
		le9->setReadOnly(1);
		le9->clear();
	}
}

void AddFtnDialog::enableSUP(int state)
{
	if ( state == Qt::Checked )
	{
		le10->setReadOnly(0);
		stPcb->setCheckState(Qt::Unchecked);
		le10->setText("80");
	}
	else
	{
		le10->setReadOnly(1);
		le10->clear();
	}
}

void AddFtnDialog::enableSTP(int state)
{
	if ( state == Qt::Checked )
	{
		le11->setReadOnly(0);
		suPcb->setCheckState(Qt::Unchecked);
		le11->setText("80");
	}
	else
	{
		le11->setReadOnly(1);
		le11->clear();
	}
}
