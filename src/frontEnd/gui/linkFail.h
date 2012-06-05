#ifndef LINKFAIL_H
#define LINKFAIL_H

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

//class LinkFail : public QWidget
class LinkFail : public QDialog
{
Q_OBJECT
public:
	LinkFail( QDialog *parent = 0);
	bool cancelled;
	bool resume;
	float failTime;
	float resumeTime;
	
private:
	QPushButton *add;
	QPushButton *cancel;
	QLabel *failL;
	QLabel *resumeL;
	QCheckBox *cb;
	QLineEdit *le1;
	QLineEdit *le2;
	QGridLayout *grid;

private slots:
	void restore(int);
	void add2();
	void cancelClicked();
};

#endif
