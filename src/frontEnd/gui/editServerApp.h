#ifndef EDIT_SERVERAPP
#define EDIT_SERVERAPP

#include <QDialog>
#include <QComboBox>
#include "topology.h"

class TopologyMap;

class EditServerApp : public QDialog
{
	Q_OBJECT
public:
	struct serverAppStruct
	{
		std::string nodeName;
		std::string appType;
		unsigned int port;
		float startTime;
		float stopTime;
		unsigned int index;
	};

	EditServerApp(TopologyMap::serverAppStruct* serverApp, QDialog *parent=0 ) ;
	~EditServerApp();

private:
	TopologyMap::serverAppStruct* serverApp;
	QComboBox* appCombo;
	QLabel *portL;
	QLabel *startL;
	QLabel *stopL;
	QLineEdit* portE;
	QLineEdit* startE;
	QLineEdit* stopE;
	QGridLayout* grid;
	QPushButton *add;
	QPushButton *cancel;

private slots:
	void save();
	void cancelAct();

};


#endif


