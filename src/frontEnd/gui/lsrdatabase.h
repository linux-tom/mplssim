#ifndef LSR_DB
#define LSR_DB

#include <QDialog>
#include <QGridLayout>
#include <sstream>
#include <vector>
#include <QPushButton>
#include "topology.h"

class vector;

class LsrDB : public QDialog
{
Q_OBJECT
private:
	QGridLayout *grid;
	QPushButton *save2;
	QPushButton *cancel2;
	std::vector<TopologyMap::ilmStruct*>* ilmVector;
	std::vector<TopologyMap::ftnStruct*>* ftnVector;

private slots:
	void deleteEntry();
	void cancel();
	void save();

public:
	struct checkBx
	{
		QCheckBox* cb;
		int index;	
	};
	std::vector<checkBx*> cbVector;
	bool cancelled;
	LsrDB(std::string nodeName,std::vector<TopologyMap::ilmStruct*>* ilmVector, std::vector<TopologyMap::ftnStruct*>* ftnVector,  QDialog *parent = 0); 
	~LsrDB();

};
#endif
