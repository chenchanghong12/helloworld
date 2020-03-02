#include <random>
#include <ctime>
#include <algorithm>
#include <qDebug>

#include "sheItem.h"


void SheItem::move(Viewer::Direction direction)
{
	if ((direction ^ -1) == _direction) {
		return;
	}
	QPoint point;
	if (direction != Viewer::Direction::DEFUALT) {
		_direction = direction;
	}
	switch (_direction)
	{
	case Viewer::Direction::UP:
		point = QPoint(_path.front().x(), _path.front().y() - 1);
		break;
	case Viewer::Direction::DOWN:
		point = QPoint(_path.front().x(), _path.front().y() + 1);
		break;
	case Viewer::Direction::LEFT:
		point = QPoint(_path.front().x() - 1, _path.front().y());
		break;
	case Viewer::Direction::RIGHT:
		point = QPoint(_path.front().x() + 1, _path.front().y());
		break;
	default:
		return;
	}
	if (point.x() < 0 || point.x() >= _width || point.y() < 0 || point.y() >= _width) {
		emit againstWall();
		return;
	}
	if (point == _food) {
		_path.push_front(point);
		this->scored();
		emit foodWasAte();
		update();
		return;
	}
	if (_parent->isHited(point)) {
		emit againstShe();
		return;
	}
	_path.push_front(point);
	_path.pop_back();
	this->start();
	update();
	return ;
}
SheItem::SheItem(int diameter, int width, Viewer* parent) :
	QObject(parent),
	_parent(parent),
	_color(Qt::black),
	_direction(Viewer::Direction::RIGHT),
	_diameter(diameter),
	_width(width),
	_score(0),
	_timeout(400)
{
	this->setZValue(5);
	srand(std::clock());
	_path.push_front(QPoint(_width / 2 - 1, _width / 2));
	_path.push_front(QPoint(_width / 2, _width / 2));
	_path.push_front(QPoint(_width / 2 + 1, _width / 2));

	this->setPos(QPoint());
	_timer = new QTimer(this);
	qDebug() << connect(_timer, &QTimer::timeout, this, static_cast<void (SheItem::*)(void)>(&SheItem::move));
	qDebug() << connect(parent, &Viewer::updateFood, this, &SheItem::setFood);

}
SheItem::~SheItem()
{

}

void SheItem::initialize()
{
	_timeout = 400;
	_score = 0;
	_level = 1;
	emit levelChanged(_level);
	_direction = Viewer::Direction::RIGHT;
	_path.clear();
	_path.push_front(_startPoint);
	_path.push_front(QPoint(_startPoint.x() + 1, _startPoint.y()));
	_path.push_front(QPoint(_startPoint.x() + 2, _startPoint.y()));
}

void SheItem::setColor(Qt::GlobalColor color)
{
	this->_color = color;
}

void SheItem::setDirection(Viewer::Direction direction)
{
	_direction = direction;
}

bool SheItem::isCovered(QPoint point)
{
	return std::find(_path.begin(), _path.end(), point) != _path.end();
}

QRectF SheItem::boundingRect() const
{
	return QRectF(0, 0, (double)_width * _diameter, (double)_width * _diameter);
}

void SheItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen pen(_color, (double)_diameter - 1);
	painter->setPen(pen);
	for (const auto& point : _path) {
		painter->drawPoint(point * _diameter + QPoint(_diameter >> 1, _diameter >> 1));
	}
}



void SheItem::stop()
{
	_timer->stop();
}

void SheItem::start()
{
	_timer->start(_timeout);
}

void SheItem::setStartPoint(QPoint startPoint)
{
	this->_startPoint = startPoint;
	_path.clear();
	_path.push_front(QPoint(startPoint.x(), startPoint.y()));
	_path.push_front(QPoint(startPoint.x() + 1, startPoint.y()));
	_path.push_front(QPoint(startPoint.x() + 2, startPoint.y()));
}

void SheItem::scored(int x)
{
	_score += x;
	while (_score >= _level * 10) {
		_timeout *= 0.8;
		_level++;
		emit levelChanged(_level);
	}
	this->start();
}

void SheItem::move()
{
	this->move(Viewer::Direction::DEFUALT);
}
