#ifndef LINKPARAM_H
#define LINKPARAM_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <QComboBox>
#include <QCheckBox>
#include <iostream>
#include <string>

//class linkDialog : public QWidget
class linkDialog : public QDialog
{
Q_OBJECT
public:
	linkDialog(std::string* bw, std::string* delay, QDialog *parent = 0);
	bool cancelled;
	std::string appType;
	unsigned int port;
	std::string* linkBW;
	std::string* linkDelay;
private:
	QPushButton *add;
	QPushButton *cancel;
	QLabel *bandwidth;
	QLabel *latency;
	QComboBox *appCombo;
	QLineEdit *le1;
	QLineEdit *le2;
	QGridLayout *grid;

private slots:
	void add2();
	void cancelClicked();
};

#endif
