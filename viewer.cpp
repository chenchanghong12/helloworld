#include <qDebug>
#include <windows.h>

#include "viewer.h"
#include "sheItem.h"


Viewer::Viewer(QWidget* parent) :
	QGraphicsView(parent),
	_width(30),
	_sheDiameter(10),
	_state(State::GAMEOVER)
{
	this->grabKeyboard();
	QGraphicsScene* scene = new QGraphicsScene(this);
	scene->setBackgroundBrush(Qt::gray);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setSceneRect(0, 0, (double)_width * _sheDiameter, (double)_width * _sheDiameter);
	this->setScene(scene);

	QFont font;
	font.setFamily("Î¢ÈíÑÅºÚ");
	font.setPointSize(30);
	font.setCapitalization(QFont::Capitalize);
	_text = new QGraphicsSimpleTextItem();
	_text->setFont(font);
	_text->setZValue(10);
	QGraphicsRectItem* outlinBorder = new QGraphicsRectItem();
	outlinBorder->setZValue(7);
	outlinBorder->setRect(0, 0, _width * _sheDiameter, _width * _sheDiameter);
	QPen pen(Qt::black, 2);
	outlinBorder->setPen(pen);
	this->scene()->addItem(outlinBorder);
	SheItem* sheItem = new SheItem(_sheDiameter, _width, this);
	sheItem->setColor(Qt::red);
	sheItem->setStartPoint(QPoint(0, 0));
	_she.push_back(sheItem);
	qDebug() << connect(sheItem, &SheItem::foodWasAte, this, &Viewer::generateFood);
	qDebug() << connect(sheItem, &SheItem::againstShe, this, &Viewer::leftAgainstShe);
	qDebug() << connect(sheItem, &SheItem::againstWall, this, &Viewer::leftAgainstWall);
	qDebug() << connect(sheItem, &SheItem::levelChanged, this, &Viewer::updateLeftLevel);
	this->scene()->addItem(sheItem);
	sheItem = new SheItem(_sheDiameter, _width, this);
	sheItem->setColor(Qt::green);
	sheItem->setStartPoint(QPoint(0, _width-1));
	qDebug() << connect(sheItem, &SheItem::foodWasAte, this, &Viewer::generateFood);
	qDebug() << connect(sheItem, &SheItem::againstShe, this, &Viewer::rightAgainstShe);
	qDebug() << connect(sheItem, &SheItem::againstWall, this, &Viewer::rightAgainstWall);
	qDebug() << connect(sheItem, &SheItem::levelChanged, this, &Viewer::updateRightLevel);
	_she.push_back(sheItem);
	this->scene()->addItem(sheItem);
	this->scene()->addItem(_text);
	_foodItem = new QGraphicsEllipseItem();
	_foodItem->setBrush(QBrush(Qt::black));
	_foodItem->setZValue(7);
	_foodItem->setSpanAngle(16 * 360);  // ÆðÊ¼½Ç¶È
	this->scene()->addItem(_foodItem);
}

Viewer::~Viewer()
{
	this->scene()->clear();
}

void Viewer::keyPressEvent(QKeyEvent* event)
{
	if (_state != State::START) {
		event->ignore();
		return;
	}
	Direction direction = Direction::DEFUALT;
	switch (event->key())
	{
	case Qt::Key_Left:
		direction = Direction::LEFT;
		break;

	case Qt::Key_Right:
		direction = Direction::RIGHT;
		break;

	case Qt::Key_Up:
		direction = Direction::UP;
		break;

	case Qt::Key_Down:
		direction = Direction::DOWN;
		break;
	default:
		event->ignore();
	}
	if (direction != Direction::DEFUALT) {
		this->update(_she[1], direction);
		if (_state != State::START) {
			event->accept();
			return;
		}
		event->accept();
	}

	direction = Direction::DEFUALT;
	switch (event->key())
	{
	case Qt::Key_A:
		direction = Direction::LEFT;
		break;

	case Qt::Key_D:
		direction = Direction::RIGHT;
		break;

	case Qt::Key_W:
		direction = Direction::UP;
		break;

	case Qt::Key_S:
		direction = Direction::DOWN;
		break;
	default:
		event->ignore();
	}
	if (direction != Direction::DEFUALT) {
		this->update(_she[0], direction);
		if (_state != State::START) {
			event->accept();
			return;
		}
		event->accept();
	}
}


void Viewer::showMessage(const QString& message, int timeout)
{
	emit this->mainWindowMessage(message, timeout);
}



void Viewer::gameStart()
{

	_text->setText("");
	if (_state == State::GAMEOVER) {
		for (auto she : _she) {
			she->initialize();
		}
		this->generateFood();
	}
	_state = State::START;
	this->start();
}

void Viewer::generateFood()
{
	QPoint point(rand() % _width, rand() % _width);
	while (true) {
		point = QPoint(rand() % _width, rand() % _width);
		bool k = true;
		for (auto she : this->_she) {
			if (she->isCovered(point)) {
				k = false;
				break;
			}
		}
		if (k) {
			break;
		}
	}
	_food = point;
	_foodItem->setRect(QRectF(point.x() * _sheDiameter, point.y()*_sheDiameter, _sheDiameter, _sheDiameter));
	emit updateFood(point);
}

bool Viewer::isHited(QPoint point)
{
	for (auto she : _she) {
		if (she->isCovered(point)) {
			return true;
		}
	}
	return false;
}

void Viewer::gamePause()
{
	_text->setText("pause!");
	_state = State::PAUSE;
	this->stop();
}

void Viewer::start()
{
	for (auto she : _she) {
		she->start();
	}
}

void Viewer::gameOver()
{
	_text->setText("game over!");
	_state = State::GAMEOVER;
	this->stop();
}

void Viewer::stop()
{
	for (auto she : _she) {
		she->stop();
	}
}



void Viewer::update(SheItem* sheItem,Direction direction)
{
	sheItem->move(direction);
}

void Viewer::updateLeftLevel(int x)
{
	emit updateLeftLevelLabel(x);
}

void Viewer::updateRightLevel(int x)
{
	emit updateRightLevelLabel(x);
}

void Viewer::leftAgainstWall()
{
	this->gameOver();
}

void Viewer::leftAgainstShe()
{
	this->gameOver();
}

void Viewer::rightAgainstWall()
{
	this->gameOver();
}

void Viewer::rightAgainstShe()
{
	this->gameOver();
}
