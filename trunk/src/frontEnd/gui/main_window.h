#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "topology.h"
#include "nodeIcon.h"
#include "../client/ClientSocket.h"
#include "../client/SocketException.h"
#include <QtGui>
#include <QLabel>
#include <QHBoxLayout>
#include <QStatusBar>
#include <sstream>

class MainWindow : public QMainWindow
{
	Q_OBJECT  

	public:
		TopologyMap *topologyMap;
		MainWindow(QWidget *parent = 0);
		~MainWindow();
	private:
		unsigned int lsrIndex;
		unsigned int ipIndex;
		unsigned int clientIndex;
		unsigned int serverIndex;
		std::stringstream s;
		QAction *newSim ;
		QAction *addclient ;
		QAction *addserver ;
		QAction *addip ;
		QAction *addlsr ;
		QAction *addSwitch; 
		QAction *addline ;
		QAction *startsim; 
		QAction *build; 
		QAction *lspAct; 
		QAction *cont; 
		QAction *remove ;
		QAction *quit2 ;
		QStatusBar* statusBar2;
	signals:
		//void connectionCreated(int newValue);

	private slots:

		//void changeStatusBar();    
		void AddLsrNode();

		void AddIPSwitch();
		void AddIPNode();
		void AddClientNode();
		void AddServerNode();
		void AddConnectionLine();
		void StartSimulation();
		void continueSimulation();
		void buildSimulation();
		void newSimulation();
		void About();
		void deleteNode();
		void showLsp();
		void terminateApp();
};

#endif
