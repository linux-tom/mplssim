#ifndef LSPBUTTONS_H
#define LSPBUTTONS_H

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

//class LspButtons : public QWidget
class LspButtons : public QDialog
{
	Q_OBJECT
public:
	LspButtons( unsigned int num, QDialog *parent = 0);
	bool cancelled;
	unsigned int checkedButton;
private:
	QPushButton *cancel;
	QGridLayout *grid;
	struct Button
	{
		QPushButton *button;
		unsigned int index;

	};
	std::vector<Button*> buttonsVector;

private slots:
	void handover();
	void cancelClicked();
};

#endif
