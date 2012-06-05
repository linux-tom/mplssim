#include <QtGui>
//#include <iostream>
#include "nodeIcon.h"
#include <QMenu>

NodeIcon::NodeIcon(int index,std::string name, std::string type, TopologyMap *parent ) : QLabel(parent)
{
	this->index=index;
	this->name=name;
	this->type=type;
	this->topologyMap=parent;	
	this->addressSet=0;	
	this->switchIndex=0;	
	this->baseIP="NULL";	

	if ( this->type=="MPLS")
	{
		this->icon=":/icons/lsr.png";
	}	
	else if ( this->type == "CLIENT" )
	{
		this->icon=":/icons/client.png";
	}
	else if ( this->type == "SWITCH" )
	{
		this->icon=":/icons/switch.png";
	}
	else if ( this->type == "SERVER" )
	{
		this->icon=":/icons/server.png";
	}
	else if ( this->type == "IP_ROUTER" )
	{
		this->icon=":/icons/ip.png";
	}
	else
	{
	}

	this->setToolTip( tr(this->name.c_str()) );
	this->setPixmap(QPixmap(this->icon));
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->move(20, 20);
	this->show();
	this->AddMenus();

}

NodeIcon::~NodeIcon()
{
	for ( unsigned int i=0; i<actionVector.size();i++ )
	{
		delete actionVector[i];
	}
	actionVector.clear();
}

void NodeIcon::setName(std::string name)
{
	this->name=name;
}

QString NodeIcon::getIcon()
{
	return this->icon;
}
std::string NodeIcon::getType()
{
	return this->type;
}

std::string NodeIcon::getName()
{
	return this->name;
}

void NodeIcon::remove()
{
	this->name="deleted";	
}
void NodeIcon::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu *menu=new QMenu(this);
	for ( unsigned int i=0; i<actionVector.size();i++ )
	{
		actionVector[i]->action->setDisabled(!(actionVector[i]->enabled));
		menu->addAction(actionVector[i]->action);
	}
	menu->exec(event->globalPos());
}

std::string NodeIcon::getMainAddress()
{
	if ( this->addressVector.size() == 0 )
	{
		return "NO IP SET";
	}
	else
	{
		return this->addressVector[0];
	}
}


void NodeIcon::AddMenus()
{

	if ( this->type=="MPLS")
	{

		QAction *createConn=new QAction(tr("Create &Connection"), this);
		nodeAction* connAction=new nodeAction;
		connAction->name="Connection";
		connAction->action=createConn;
		connAction->enabled=1;
		actionVector.push_back(connAction);
		connect(createConn, SIGNAL(triggered()), this, SLOT(createConn()));
		
		QAction *addFTN=new QAction(tr("Add &FTN"), this);
		nodeAction* ftnAction=new nodeAction;
		ftnAction->name="Add FTN";
		ftnAction->action=addFTN;
		ftnAction->enabled=1;
		actionVector.push_back(ftnAction);
		connect(addFTN, SIGNAL(triggered()), this, SLOT(addFecToNhlfe()));

		QAction *addILM=new QAction(tr("Add &Swap ILM"), this);
		nodeAction* ilmAction=new nodeAction;
		ilmAction->name="Add ILM";
		ilmAction->action=addILM;
		ilmAction->enabled=1;
		actionVector.push_back(ilmAction);
		connect(addILM, SIGNAL(triggered()), this, SLOT(addIncLabelMap()));

		QAction *pop=new QAction(tr("Add &Pop ILM"), this);
		nodeAction *popAction=new nodeAction;
		popAction->name="Add POP";
		popAction->action=pop;
		popAction->enabled=1;
		actionVector.push_back(popAction);
		connect(pop, SIGNAL(triggered()), this, SLOT(popLabel()));

		QAction *db=new QAction(tr("Show &LSR DB"), this);
		nodeAction *dbAction=new nodeAction;
		dbAction->name="Add POP";
		dbAction->action=db;
		dbAction->enabled=1;
		actionVector.push_back(dbAction);
		connect(db, SIGNAL(triggered()), this, SLOT(showLSRDB()));

		QAction *del=new QAction(tr("&Delete Node"), this);
		nodeAction* delAction=new nodeAction;
		delAction->name="Delete";
		delAction->action=del;
		delAction->enabled=1;
		actionVector.push_back(delAction);
		connect(del, SIGNAL(triggered()), this, SLOT(del()));
	}	
	else if ( this->type == "CLIENT" )
	{
		QAction *createConn=new QAction(tr("Create &Connection"), this);
		nodeAction* connAction=new nodeAction;
		connAction->name="Connection";
		connAction->action=createConn;
		connAction->enabled=1;
		actionVector.push_back(connAction);
		connect(createConn, SIGNAL(triggered()), this, SLOT(createConn()));

		QAction *confApp=new QAction(tr("&Add Client Application"), this);
		nodeAction* confAction=new nodeAction;
		confAction->name="Config_Client_App";
		confAction->action=confApp;
		confAction->enabled=1;
		actionVector.push_back(confAction);
		connect(confApp, SIGNAL(triggered()), this, SLOT(confClientApp()));

		QAction *apps=new QAction(tr("Show &Installed Applications"), this);
		nodeAction* appsAction=new nodeAction;
		appsAction->name="apps";
		appsAction->action=apps;
		appsAction->enabled=1;
		actionVector.push_back(appsAction);
		connect(apps, SIGNAL(triggered()), this, SLOT(showApps()));

		QAction *hist=new QAction(tr("Show &Graphs"), this);
		nodeAction* histAction=new nodeAction;
		histAction->name="histogram";
		histAction->action=hist;
		histAction->enabled=0;
		actionVector.push_back(histAction);
		connect(hist, SIGNAL(triggered()), this, SLOT(showGraphs()));

		QAction *del=new QAction(tr("&Delete Node"), this);
		nodeAction* delAction=new nodeAction;
		delAction->name="Delete";
		delAction->action=del;
		delAction->enabled=1;
		actionVector.push_back(delAction);
		connect(del, SIGNAL(triggered()), this, SLOT(del()));

	}
	else if ( this->type == "SERVER" )
	{
		QAction *createConn=new QAction(tr("Create &Connection"), this);
		nodeAction* connAction=new nodeAction;
		connAction->name="Connection";
		connAction->action=createConn;
		connAction->enabled=1;
		actionVector.push_back(connAction);
		connect(createConn, SIGNAL(triggered()), this, SLOT(createConn()));


		QAction *setAppType=new QAction(tr("&Add Server Application "), this);
		nodeAction* confAction=new nodeAction;
		confAction->name="Config_Server_App";
		confAction->action=setAppType;
		confAction->enabled=1;
		actionVector.push_back(confAction);
		connect(setAppType, SIGNAL(triggered()), this, SLOT(confServerApp()));

		QAction *apps=new QAction(tr("Show &Installed Applications"), this);
		nodeAction* appsAction=new nodeAction;
		appsAction->name="apps";
		appsAction->action=apps;
		appsAction->enabled=1;
		actionVector.push_back(appsAction);
		connect(apps, SIGNAL(triggered()), this, SLOT(showApps()));

		QAction *hist=new QAction(tr("Show &Graphs"), this);
		nodeAction *histAction=new nodeAction;
		histAction->name="histogram";
		histAction->action=hist;
		histAction->enabled=0;
		actionVector.push_back(histAction);
		connect(hist, SIGNAL(triggered()), this, SLOT(showGraphs()));

		QAction *del=new QAction(tr("&Delete Node"), this);
		nodeAction *delAction=new nodeAction;
		delAction->name="Delete";
		delAction->action=del;
		delAction->enabled=1;
		actionVector.push_back(delAction);
		connect(del, SIGNAL(triggered()), this, SLOT(del()));

	}
	else if ( this->type == "IP_ROUTER" )
	{

		QAction *createConn=new QAction(tr("Create &Connection"), this);
		nodeAction *connAction=new nodeAction;
		connAction->name="Connection";
		connAction->action=createConn;
		connAction->enabled=1;
		actionVector.push_back(connAction);
		connect(createConn, SIGNAL(triggered()), this, SLOT(createConn()));

		QAction *del=new QAction(tr("&Delete Node"), this);
		nodeAction *delAction=new nodeAction;
		delAction->name="Delete";
		delAction->action=del;
		delAction->enabled=1;
		actionVector.push_back(delAction);
		connect(del, SIGNAL(triggered()), this, SLOT(del()));
	}
	else if ( this->type == "SWITCH" )
	{

		QAction *conAct=new QAction(tr("Create &Connection"), this);
		nodeAction *crtAction=new nodeAction;
		crtAction->name="Connection";
		crtAction->action=conAct;
		crtAction->enabled=1;
		actionVector.push_back(crtAction);
		connect(conAct, SIGNAL(triggered()), this, SLOT(createConn()));

		QAction *showAct=new QAction(tr("Change CSMA Link Parameters"), this);
		nodeAction *showAction=new nodeAction;
		showAction->name="csma";
		showAction->action=showAct;
		showAction->enabled=1;
		actionVector.push_back(showAction);
		connect(showAct, SIGNAL(triggered()), this, SLOT(changeLinkParams()));

		QAction *del=new QAction(tr("&Delete Node"), this);
		nodeAction *delAction=new nodeAction;
		delAction->name="Delete";
		delAction->action=del;
		delAction->enabled=1;
		actionVector.push_back(delAction);
		connect(del, SIGNAL(triggered()), this, SLOT(del()));
	}
	else
	{
	}


}

void NodeIcon::changeLinkParams()
{

	this->topologyMap->changeCSMALinkParams(this->name);

}

void NodeIcon::del()
{
	this->topologyMap->deleteNodeLinks(this->name);
	this->remove();
	this->hide();
}

void NodeIcon::confClientApp()
{
	this->topologyMap->configureClientApp(this->name);
}
void NodeIcon::confServerApp()
{
	this->topologyMap->configureServerApp(this->name);
}
void NodeIcon::createConn()
{
	if ( this->addressSet == 1 )
	{
		return;
	}
	else
	{
		this->topologyMap->createConnection(this->name);
	}
}

void NodeIcon::addFecToNhlfe()
{
	this->topologyMap->addFTN(this->name);
}

void NodeIcon::addIncLabelMap()
{
	this->topologyMap->addILM(this->name);
}

void NodeIcon::popLabel()
{
	this->topologyMap->popLabel(this->name);
}

void NodeIcon::showGraphs()
{
	this->topologyMap->showGraphs(this->name);
}

void NodeIcon::showApps()
{
	if ( this->type =="CLIENT" )
	{
		this->topologyMap->showInstalledClientApps(this->name);
	}
	else if ( this->type == "SERVER" )
	{
		this->topologyMap->showInstalledServerApps(this->name);
	}
	else
	{
	}
}

void NodeIcon::showMainIP()
{
	std::cout<<this->getMainAddress()<<std::endl;
}


void NodeIcon::updateToolTip()
{
	if ( this->type == "CLIENT" || this->type == "SERVER" )
	{
		std::stringstream s;
		s<<this->name<<"\n"<<this->getMainAddress();
		this->setToolTip (tr(s.str().c_str()) );
	}
	if ( this->type == "SWITCH" && this->baseIP !="NULL" )
	{
		std::stringstream s;
		s<<this->name<<"\n"<<"Base IP: "<<this->baseIP<<".0";
		this->setToolTip (tr(s.str().c_str()) );
	}
}

void NodeIcon::postSimulation()
{
	for ( unsigned int i=0; i<actionVector.size(); i++)
	{
		if ( actionVector[i]->name=="histogram")
		{
			actionVector[i]->enabled=1;
		}
		else
		{
			actionVector[i]->enabled=0;
		}
	}
}

void NodeIcon::preSimulation()
{
	for ( unsigned int i=0; i<actionVector.size(); i++)
	{
		if ( actionVector[i]->name=="histogram")
		{
			actionVector[i]->enabled=0;
		}
		else
		{
			actionVector[i]->enabled=1;
		}
	}
}

void NodeIcon::activeSimulation()
{
	for ( unsigned int i=0; i<actionVector.size(); i++)
	{
		actionVector[i]->enabled=0;
	}
}

void NodeIcon::showLSRDB()
{
	this->topologyMap->showLsrDatabase(this->name);
}
