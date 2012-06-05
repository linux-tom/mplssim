#include <QToolBar>
#include <QApplication>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QtGui>
#include <QLabel>

#include "main_window.h"
#include "topology.h"
#include "nodeIcon.h"
#include <iostream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)  : QMainWindow(parent)
{
	Q_INIT_RESOURCE(icons);

	statusBar2=this->statusBar();

	QPixmap LsrNode("gui/icons/lsr.png");
	QPixmap ServerNode("gui/icons/server.png");
	QPixmap ClientNode("gui/icons/client.png");
	QPixmap IProuter("gui/icons/ip.png");
	QPixmap Play("gui/icons/play.png");
	QPixmap quitpix("gui/icons/exit.png");
	QPixmap line("gui/icons/line.png");
	QPixmap del("gui/icons/del.png");
	QPixmap IpSwitch("gui/icons/switch.png");
	QPixmap Build("gui/icons/build.png");
	QPixmap Continue("gui/icons/continue2.png");
	QPixmap New("gui/icons/new6.png");
	QPixmap animate("gui/icons/animation.png");
	  
	QAction *quit = new QAction("&Quit", this);
	QAction *menuAbout = new QAction("&About", this); 
	  

	QToolBar *toolbar = addToolBar("main toolbar");
	newSim = toolbar->addAction(QIcon(New), "Start New Simulation");
	cont = toolbar->addAction(QIcon(Continue), "Continue Current Simulation");
	cont->setDisabled(1);

	toolbar->setIconSize (QSize(56,56));
	toolbar->addSeparator();

	addclient = toolbar->addAction(QIcon(ClientNode), "Add Client Node");
	addserver = toolbar->addAction(QIcon(ServerNode), "Add Server Node");
	addip = toolbar->addAction(QIcon(IProuter), "Add Ip Router");
	addlsr = toolbar->addAction(QIcon(LsrNode), "Add Lsr Node");

	toolbar->addSeparator();
	  
	addline = toolbar->addAction(QIcon(line), "Create PtoP Connection");
	addSwitch = toolbar->addAction(QIcon(IpSwitch), "Create CSMA Switch");
	toolbar->addSeparator();
	remove = toolbar->addAction(QIcon(del), "Delete Node");
	 
	toolbar->addSeparator();
	build = toolbar->addAction(QIcon(Build), "Build Topology");
	lspAct = toolbar->addAction(QIcon(animate), "Show Packet Animation");
	lspAct->setDisabled(1);
	startsim = toolbar->addAction(QIcon(Play), "Start Simulation");
	startsim->setDisabled(1);
	quit2 = toolbar->addAction(QIcon(quitpix), "Quit Application");

	QAction* newSim2 = new QAction("&New", this);
	QMenu *file;
	file = menuBar()->addMenu("&File");
	file->addAction(newSim2);
	file->addAction(quit);

	QMenu *preferences;
	preferences = menuBar()->addMenu("&Preferences");
	preferences->addAction(quit);

	QMenu *about;
	about = menuBar()->addMenu("&About");
	about->addAction(menuAbout);

	QHBoxLayout *dragLayout = new QHBoxLayout;
	topologyMap=new TopologyMap(this->statusBar2,startsim);
	//topologyMap->initializeButtons(addlsr, addclient, addserver,addip,addSwitch,build, addline, remove, cont, startsim);
	dragLayout->addWidget(topologyMap);
	QWidget *zoneCentral = new QWidget;
	zoneCentral->setLayout(dragLayout);

	this->setCentralWidget(zoneCentral);

	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit())); 
	connect(addlsr, SIGNAL(triggered()), this, SLOT(AddLsrNode()));
	connect(addSwitch, SIGNAL(triggered()), this, SLOT(AddIPSwitch()));
	connect(addip, SIGNAL(triggered()), this, SLOT(AddIPNode()));
	connect(addclient, SIGNAL(triggered()), this, SLOT(AddClientNode()));
	connect(addserver, SIGNAL(triggered()), this, SLOT(AddServerNode()));
	connect(addline, SIGNAL(triggered()), this, SLOT(AddConnectionLine()));
	connect(menuAbout, SIGNAL(triggered()), this, SLOT(About()));
	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
	//connect(quit2, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(quit2, SIGNAL(triggered()), this, SLOT(terminateApp()));
	connect(remove, SIGNAL(triggered()), this, SLOT(deleteNode()));
	connect(build, SIGNAL(triggered()), this, SLOT(buildSimulation()));
	connect(lspAct, SIGNAL(triggered()), this, SLOT(showLsp()));
	connect(startsim, SIGNAL(triggered()), this, SLOT(StartSimulation()));
	connect(newSim, SIGNAL(triggered()), this, SLOT(newSimulation()));
	connect(cont, SIGNAL(triggered()), this, SLOT(continueSimulation()));
}

MainWindow::~MainWindow()
{
}


void MainWindow::AddLsrNode()
{
	topologyMap->AddMplsNode();
}

void MainWindow::AddIPSwitch()
{
	topologyMap->AddSwitchNode();
}


void MainWindow::AddServerNode()
{
	topologyMap->AddServerNode();
}
void MainWindow::AddIPNode()
{
	topologyMap->AddStandardRouterNode();

}

void MainWindow::AddClientNode()
{
	topologyMap->AddClientNode();
}

void MainWindow::buildSimulation()
{
	startsim->setDisabled(0);
}

void MainWindow::showLsp()
{

	if ( topologyMap->getLspMode() == 0 )
	{
		topologyMap->startMplsSimulation();
	}
	else
	{
		topologyMap->stopMplsSimulation();
	}
	//topologyMap->parseMPLS();
}

void MainWindow::newSimulation()
{
	topologyMap->selfDestruct();
}

void MainWindow::StartSimulation()
{
	addlsr->setDisabled(1);
	addclient->setDisabled(1);
	addserver->setDisabled(1);
	addip->setDisabled(1); 
	addSwitch->setDisabled(1);
	build->setDisabled(1);
	addline->setDisabled(1);
	remove->setDisabled(1);
	cont->setDisabled(0);
	startsim->setDisabled(1);
	lspAct->setDisabled(0);
}

void MainWindow::continueSimulation()
{

	addlsr->setDisabled(0);
	addclient->setDisabled(0);
	addserver->setDisabled(0);
	addip->setDisabled(0); 
	addSwitch->setDisabled(0);
	build->setDisabled(0);
	addline->setDisabled(0);
	remove->setDisabled(0);
	cont->setDisabled(1);
	startsim->setDisabled(1);
	lspAct->setDisabled(1);
	topologyMap->setAcceptDrops(1);
	topologyMap->continueSimulation();


}

void MainWindow::AddConnectionLine()
{
	if ( topologyMap->enableLine==1 )
	{
		topologyMap->enableLine=0;
	}
	else
	{
		topologyMap->enableLine=1;
	}
}


void MainWindow::About()
{
	QMessageBox::about(this, "About",
		tr("<p align=\"center\">"
		"<h4>PRJ-076/2012 GRAPHICAL SIMULATION OF MPLS PROTOCOL"
		"</h4>"
		"<br />"
		"Copyright (c) 2012 University of Nairobi<br /><br />"
		"Department of Electrical Engineering<br />"
		"<br />"
		"<strong>Author:</strong><br />"
		"THOMAS GONE &lt;gonethomas@gmail.com&gt;"
		"</p>"
		));
}

void MainWindow::deleteNode()
{
	if ( topologyMap->deleteNode==1 )
	{
		topologyMap->deleteNode=0;
	}
	else
	{
		topologyMap->deleteNode=1;
	}
}

void MainWindow::terminateApp()
{
	bool check=topologyMap->terminateNS3();
	if ( check == 1 )
	{
		qApp->quit();
	}
}
