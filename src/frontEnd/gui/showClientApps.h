#ifndef INSTALLED_CLIENTAPPS
#define INSTALLED_CLIENTAPPS

#include <QDialog>
#include <QGridLayout>
#include <sstream>
#include <vector>
#include <QPushButton>
#include "topology.h"

class vector;

class ClientAppStore : public QDialog
{
Q_OBJECT
private:
	QGridLayout *grid;
	QPushButton *save2;
	QPushButton *cancel2;
	std::vector <TopologyMap::clientAppStruct*>* clientAppVector; 

private slots:
	void deleteEntry();
	void cancel();
	void save();

public:
	struct checkBx
	{
		QCheckBox* cb;
		unsigned int index;	
	};
	std::vector<checkBx*> cbVector;
	bool cancelled;
	ClientAppStore(std::string nodeName,std::vector<TopologyMap::clientAppStruct*>* clientAppVector, QDialog *parent = 0); 
	~ClientAppStore();

};
#endif
