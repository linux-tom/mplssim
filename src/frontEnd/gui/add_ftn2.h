#ifndef ADDFTN_H
#define ADDFTN_H

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

//class AddFtnDialog2 : public QWidget
class AddFtnDialog2 : public QDialog
{
Q_OBJECT
public:
	AddFtnDialog2(std::vector<std::string>& neighList,QDialog *parent = 0);
	bool cancelled;
	std::string sourceAddr;
	std::string destAddr;
	std::string outLabel;
	std::string nextHopNode;
private:
	QPushButton *add;
	QPushButton *cancel;
	QPushButton *sourceB;
	QPushButton *destinationB;
	QLabel *source;
	QLabel *destination;
	QLabel *next;
	QLabel *label;
	QLineEdit *le3;
	QComboBox *nextHop;
	QGridLayout *grid;

private slots:
	void add2();
	void cancelClicked();
};

#endif
