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
#include <stdint.h>

//class AddFtnDialog : public QWidget
class AddFtnDialog : public QDialog
{
Q_OBJECT
public:
	AddFtnDialog(std::vector<std::string>& neighList,QDialog *parent = 0);
	bool cancelled;
	bool fastreroute;
	std::string sourceAddr;
	std::string destAddr;
	std::string fec;
	unsigned int outLabel;
	unsigned int outLabel2;
	uint16_t destPort;
	uint16_t srcPort;
	std::string nextHopNode;
	std::string nextHopNode2;

	std::string policy;
	std::string policyAttribute;
	unsigned int policyAttributeValue;
	float weight1;
	float weight2;

private:
	QPushButton *add;
	QPushButton *cancel;
	QLabel *source;
	QLabel *destination;
	QLabel *next;
	QLabel *label;
	QLabel *next2;
	QLabel *label2;
	QLabel *labelTCP;
	QLabel *labelUDP;
	QCheckBox *cb;
	QCheckBox *sIPcb;
	QCheckBox *dIPcb;
	QCheckBox *uPcb;
	QCheckBox *tPcb;
	QCheckBox *suPcb;
	QCheckBox *stPcb;

	QLineEdit *le1;
	QLineEdit *le2;
	QLineEdit *le3;
	QLineEdit *le4;
	QLineEdit *le5;
	QLineEdit *le6;
	QLineEdit *le7;
	QLineEdit *le8;
	QLineEdit *le9;
	QLineEdit *le10;
	QLineEdit *le11;
	QLabel *main;
	QLabel *bcup;
	QLabel *polic;
	QLabel *attrValue;
	QLabel *weights;
	QComboBox *nextHop;
	QComboBox *nextHop2;
	QComboBox *policyType;
	QComboBox *policyAttr;
	QGridLayout *grid;

private slots:
	void add2();
	void cancelClicked();
	void fastReroute(int state);
	void weightsChanged();
	void showWeights(QString item);
	void enableSIP(int state);
	void enableDIP(int state);
	void enableTP(int state);
	void enableUP(int state);
	void enableSTP(int state);
	void enableSUP(int state);
};

#endif
