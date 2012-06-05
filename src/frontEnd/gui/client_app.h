#ifndef CLIENTAPP_H
#define CLIENTAPP_H

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

//class clientAppDialog : public QWidget
class clientAppDialog : public QDialog
{
Q_OBJECT
public:
	clientAppDialog(std::vector<std::string>& serverList,  QDialog *parent = 0);
	~clientAppDialog();
	bool cancelled;
	std::string appType;
	std::string server;
	unsigned int port;
	float startTime;
	float stopTime;
	unsigned int packetSize;
	unsigned int numPackets;
	float interval;
private:
	QPushButton *add;
	QPushButton *cancel;
	QLabel *app;
	QLabel *serverLabel;
	QLabel *appPort;
	QLabel *appStart;
	QLabel *appStop;
	QLabel *packSize;
	QLabel *packNum;
	QLabel *appInterval;
	QComboBox *appCombo;
	QComboBox *serverCombo;
	QLineEdit *le1;
	QLineEdit *le2;
	QLineEdit *le3;
	QLineEdit *le4;
	QLineEdit *le5;
	QLineEdit *le6;
	QGridLayout *grid;

private slots:
	void add2();
	void cancelClicked();
};

#endif
