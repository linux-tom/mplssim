#include <QVBoxLayout>
#include <QFont>
#include <QDesktopWidget>
#include "sim_results.h"


SimResultWindow::SimResultWindow(std::string& fileName, QWidget *parent) : QWidget(parent)
{

	QFile file(fileName.c_str());
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	textArea=new QTextEdit;
	
	in=new QTextStream(&file);

	QString line = in->readLine();

	while (!in->atEnd()) {
		line = in->readLine();
		textArea->append(line);
	}

	textArea->setReadOnly(1);

	QVBoxLayout *vbox = new QVBoxLayout();
	vbox->addWidget(textArea);
	this->setLayout(vbox);
	int WIDTH = 850;
	int HEIGHT= 600;
	QDesktopWidget *desktop = QApplication::desktop();

	int screenWidth= desktop->width();
	int screenHeight= desktop->height();

	int x=(screenWidth -WIDTH)/2;
	int y=(screenHeight - HEIGHT)/2;

	this->setGeometry(x, y, WIDTH, HEIGHT);

	file.close();
//	file.remove(fileName.c_str());	
}
