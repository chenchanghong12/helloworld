#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_tanchishe.h"

class tanchishe : public QMainWindow
{
	Q_OBJECT

public:
	tanchishe(QWidget *parent = Q_NULLPTR);

private:
	Ui::tanchisheClass ui;

};
