#ifndef SERVERAPP_H
#define SERVERAPP_H

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

//class serverAppDialog : public QWidget
class serverAppDialog : public QDialog
{
Q_OBJECT
public:
	serverAppDialog(QDialog *parent = 0);
	bool cancelled;
	std::string appType;
	unsigned int port;
	float startTime;
	float stopTime;
private:
	QPushButton *add;
	QPushButton *cancel;
	QLabel *app;
	QLabel *appPort;
	QLabel *appStart;
	QLabel *appStop;
	QComboBox *appCombo;
	QLineEdit *le1;
	QLineEdit *le2;
	QLineEdit *le3;
	QGridLayout *grid;

private slots:
	void add2();
	void cancelClicked();
};

#endif
