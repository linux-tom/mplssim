#ifndef SHOWGRAPHS_H
#define SHOWGRAPHS_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPrinter>
#include <vector>
#include "topology.h"
#include <sstream>

class TopologyMap;
class vector;
class ShowGraphs : public QDialog
{
	Q_OBJECT

public:
	std::vector<TopologyMap::ilmStruct*>* ilmVector;
	//ShowGraphs(QWidget *parent = 0 );
	ShowGraphs(std::string nodeName,std::vector<TopologyMap::nodeGraph*>* graphsVector, QDialog *parent=0); 

private:
	void updateActions();
	void scaleImage(double factor);
	QLabel *imageLabel;
	double scaleFactor;
	std::vector<TopologyMap::nodeGraph*> graphsVector; 
	std::vector<TopologyMap::nodeGraph*> localGraphsVector; 
	std::string nodeName;
	unsigned short graphNo;
	QImage *currentImage;
	TopologyMap::nodeGraph* currentGraph;
	std::stringstream s;
	/*QPushButton *add ;
	QPushButton *rename ;
	QPushButton *remove ;
	QPushButton *removeall ;*/
	QPushButton *next; 
	QPushButton *previous ;
	QGridLayout *grid ;

private slots:
	void goNext();
	void goPrev();
	
};

#endif
