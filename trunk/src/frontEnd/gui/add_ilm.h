#ifndef ADDILM_H
#define ADDILM_H

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
#include <vector>

//class AddIlmWindow : public QWidget
class AddIlmWindow : public QDialog
{
Q_OBJECT
public:
	AddIlmWindow( std::vector<std::string>& neighList, QDialog *parent = 0);
	bool cancelled;
	bool fastreroute;
	unsigned int label1;
	unsigned int label2;
	unsigned int label3;
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
	QLabel *incoming;
	QLabel *next;
	QLabel *outgoing;
	QLabel *next2;
	QLabel *outgoing2;
	QLabel *main;
	QLabel *bcup;
	QLabel *weights;
	QLineEdit *le1;
	QLineEdit *le2;
	QLineEdit *le3;
	QLineEdit *le4;
	QLineEdit *le5;
	QLineEdit *le6;
	QComboBox *nextHop;
	QComboBox *nextHop2;
	QCheckBox *cb;
	QCheckBox *cb2;
	QCheckBox *cb3;
	QGridLayout *grid;
	QDoubleValidator *weightMask;

	QLabel *addPolicy;
	QLabel *attrValue;
	QComboBox *policyType;
	QComboBox *policyAttr;

private slots:
	void add2();
	void setOutgoing(int state);
	void setOutgoing2(int state);
	void incomingLabelChanged();
	void cancelClicked();
	void fastReroute(int state);
	void showWeights(QString);
	void weightsChanged();
};

#endif
