#include "lsp_buttons.h"
#include <sstream>

LspButtons::LspButtons( unsigned int num, QDialog *parent ) : QDialog(parent)
{
	std::stringstream s;

	this->cancelled=1;

	grid = new QGridLayout(); 
	int a=0;

	for ( unsigned int i=0; i<num; i++ )
	{
		s.str("");
		s<<"LSP "<<(i+1);
		QPushButton* psh=new QPushButton(s.str().c_str(),this);
		grid->addWidget(psh, a, 0);
		psh->setCheckable(1);
		psh->setChecked(0);
		Button* btn=new Button;
		btn->button=psh;
		btn->index=i;
		buttonsVector.push_back(btn);
		connect(psh, SIGNAL(clicked()), this, SLOT(handover()));
		
		a++;
	}

	cancel=new QPushButton("CANCEL",this);
	grid->addWidget(cancel,a, 0);

	connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));

	setLayout(grid);
	this->setWindowTitle("Show Label Switched Paths");
}

void LspButtons::handover()
{
	for (unsigned int j=0; j<this->buttonsVector.size(); j++ )
	{
		if ( buttonsVector[j]->button->isChecked() )
		{
			this->checkedButton=buttonsVector[j]->index;
			buttonsVector[j]->button->setChecked(0);
		}
	}

	this->cancelled=0;
	this->hide();	
}

void LspButtons::cancelClicked()
{
	this->cancelled=1;
	this->hide();
}
