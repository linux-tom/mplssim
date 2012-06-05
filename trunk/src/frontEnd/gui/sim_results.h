#ifndef SIMRESULTS_H
#define SIMRESULTS_H

#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QFile>
#include <QLabel>
#include <QString>
#include <QTextStream>
#include <QTextEdit>

class SimResultWindow : public QWidget
{
  public:
    SimResultWindow(std::string& fileName,QWidget *parent = 0);

  private:
	QTextEdit *textArea; 
	QTextStream *in;

};

#endif
