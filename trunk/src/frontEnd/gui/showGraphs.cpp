#include "showGraphs.h"
#include <QVBoxLayout>
#include <QPushButton>

ShowGraphs::ShowGraphs(std::string nodeName,std::vector<TopologyMap::nodeGraph*>* graphsVector,  QDialog *parent) : QDialog(parent)
{
	//this->graphsVector = graphsVector;
	this->graphNo=0;
	this->nodeName = nodeName;

	for ( unsigned int i=0; i<(*graphsVector).size(); i++ )
	{
		if ( (*graphsVector)[i]->nodeName==nodeName )
		{
			(*graphsVector)[i]->index=this->graphNo++;
			this->localGraphsVector.push_back((*graphsVector)[i]);
		}
	}

/*	add = new QPushButton("Add", this);
	rename = new QPushButton("Rename", this);
	remove = new QPushButton("Remove", this);
	removeall = new QPushButton("Remove All", this);*/
	next = new QPushButton("Next", this);
	previous = new QPushButton("Previous", this);

	if ( this->localGraphsVector.size() > 0 )
	{
		currentGraph=localGraphsVector[0];
	
		imageLabel = new QLabel;
		imageLabel->setBackgroundRole(QPalette::Base);
		imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		imageLabel->setScaledContents(true);

		currentImage=new QImage(currentGraph->fileName.c_str());
		imageLabel->setPixmap(QPixmap::fromImage(*currentImage));
		scaleFactor = 1.0;
		previous->setEnabled(0);
		if ( localGraphsVector.size() == 0)
		{
			next->setEnabled(0);
		}
	}
	
	
	s.str("");
	s<<nodeName<<" Histogram";

	grid = new QGridLayout();
	//QLabel *bottomLabel = new QLabel(s.str().c_str());
	
	/*grid->addWidget(add,0,6);
	grid->addWidget(rename,1,6);
	grid->addWidget(remove,2,6);
	grid->addWidget(removeall,3,6);*/

	grid->addWidget(previous,6,2);
	grid->addWidget(next,6,3);
	grid->addWidget(imageLabel, 0, 0, 6, 6);
	this->setLayout(grid);

	s.str("");
	s<<nodeName<<" Simulation Results";	
	this->setWindowTitle(tr(s.str().c_str()));
	resize(700, 700);

	connect(previous, SIGNAL(clicked()), this, SLOT(goPrev()));
	connect(next, SIGNAL(clicked()), this, SLOT(goNext()));
}

void ShowGraphs::goPrev()
{
	//grid->removeWidget(imageLabel);
	if ( int(currentGraph->index-1) >= 0 )
	{
		currentGraph=localGraphsVector[currentGraph->index-1];
		currentImage->load(currentGraph->fileName.c_str());
	}

	if ( currentGraph->index == 0 && localGraphsVector.size() > 1 )
	{
		previous->setEnabled(0);
		next->setEnabled(1);
	}
	
	if ( currentGraph->index <= localGraphsVector.size()-2 )
	{
		next->setEnabled(1);
	}

	imageLabel->setPixmap(QPixmap::fromImage(*currentImage));
	//grid->addWidget(imageLabel, 0, 0, 5, 5);
}

void ShowGraphs::goNext()
{
	if ( (currentGraph->index+1) <= localGraphsVector.size()-1 )
	{
		currentGraph=localGraphsVector[currentGraph->index+1];
		currentImage->load(currentGraph->fileName.c_str());
	}

	if ( currentGraph->index == localGraphsVector.size()-1 )
	{
		next->setEnabled(0);
	}

	if ( currentGraph->index > 0 )
	{
		previous->setEnabled(1);
	}
	imageLabel->setPixmap(QPixmap::fromImage(*currentImage));
}

