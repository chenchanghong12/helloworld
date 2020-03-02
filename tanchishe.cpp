#include <QDebug>

#include "tanchishe.h"

tanchishe::tanchishe(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QFont font;
	font.setFamily("Î¢ÈíÑÅºÚ");
	font.setPointSize(28);
	ui.rightLevelLabel->setFont(font);
	ui.leftLevelLabel->setFont(font);
	qDebug() << connect(ui.view, &Viewer::updateLeftLevelLabel, ui.leftLevelLabel, static_cast<void (QLabel::*)(int)>(&QLabel::setNum));
	qDebug() << connect(ui.view, &Viewer::updateRightLevelLabel, ui.rightLevelLabel, static_cast<void (QLabel::*)(int)>(&QLabel::setNum));
	qDebug() << connect(ui.startButton, &QPushButton::clicked, ui.view, &Viewer::gameStart);
	qDebug() << connect(ui.pauseButton, &QPushButton::clicked, ui.view, &Viewer::gamePause);
}
