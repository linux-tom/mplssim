#include "gui/main_window.h"
#include <QDesktopWidget>
#include <QApplication>
#include <iostream>
#include "topology.h"
#include "gui/threadedParser.h"

pthread_mutex_t ThreadedParser::incrementMutex=PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t ThreadedParser::vectorMutex=PTHREAD_MUTEX_INITIALIZER; 

void center(QWidget &widget)
{
	
	QDesktopWidget *desktop = QApplication::desktop();

	int screenWidth= desktop->width();
	int screenHeight= desktop->height();
	int WIDTH = screenWidth/1.25;
	int HEIGHT= screenHeight/1.25;

	int x=(screenWidth -WIDTH)/2;
	int y=(screenHeight - HEIGHT)/2;

	widget.setGeometry(x,y,WIDTH,HEIGHT);

}

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);  

  MainWindow mainWindow;

  mainWindow.setWindowTitle("MPLS SIMULATOR");
  mainWindow.show();
  center(mainWindow);

  return app.exec();
}
