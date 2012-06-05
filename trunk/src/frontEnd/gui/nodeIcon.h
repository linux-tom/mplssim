#ifndef NODE_ICON_H
#define NODE_ICON_H


#include <QtGui>
#include "topology.h"
#include <QMenu>
#include <vector>

class TopologyMap;

class NodeIcon : public QLabel 
{
	Q_OBJECT  
	public:

		NodeIcon(int index,std::string name, std::string type, TopologyMap *parent );

		~NodeIcon();

		void setName(std::string name);
		
		void setTooltip(std::string tooltip);
		
		bool addressSet;		

		std::string baseIP;
		int switchIndex;	

		std::string getName();
		std::string getType();
		
		std::vector <std::string> addressVector;
		std::vector <std::string> neighList;
		
		QString getIcon();
		int index;	

		void remove();
		void updateToolTip();
		std::string getAddressList();
		std::string getMainAddress();
		void postSimulation();
		void preSimulation();
		void activeSimulation();

	private:
		TopologyMap* topologyMap;
		QAction *delAct;
		void AddMenus();
		QAction *copyAct;
		QAction *pasteAct;
		std::string type;
		std::string name;
		QString icon;
		struct nodeAction
		{
			QAction *action;
			std::string name;
			bool enabled;
		};
		std::vector <nodeAction*> actionVector;
	private slots:
		void del();
		void createConn();
		void confClientApp();
		void confServerApp();
		void addFecToNhlfe();
		void popLabel();
		void addIncLabelMap();
		void showMainIP();
		void showGraphs();
		void showApps();
		void showLSRDB();
		void changeLinkParams();
		//     void copy();
	     //void paste();

	 protected:
		void contextMenuEvent(QContextMenuEvent *event);


};

#endif
