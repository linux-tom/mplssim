#ifndef INSTALLED_SERVERAPPS
#define INSTALLED_SERVERAPPS

#include <QDialog>
#include <QGridLayout>
#include <sstream>
#include <vector>
#include <QPushButton>
#include "topology.h"

class vector;

class ServerAppStore : public QDialog
{
Q_OBJECT
private:
	QGridLayout *grid;
	QPushButton *save2;
	QPushButton *cancel2;
	QPushButton *okb;
	std::vector <TopologyMap::serverAppStruct*>* serverAppVector; 
	struct appButton
	{
		QPushButton* button;
		TopologyMap::serverAppStruct* app;
	};
	std::vector <appButton*> buttonVector;

private slots:
	void deleteEntry();
	void cancel();
	void save();
	void edit();

public:
	struct checkBx
	{
		QCheckBox* cb;
		unsigned int index;	
	};
	std::vector<checkBx*> cbVector;
	bool cancelled;
	ServerAppStore(std::string nodeName,std::vector<TopologyMap::serverAppStruct*>* serverAppVector, QDialog *parent = 0); 
	~ServerAppStore();

};
#endif
