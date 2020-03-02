#pragma once
#include <QGraphicsItem>
#include <QObject>
#include <list>

#include "viewer.h"


class SheItem :public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:

	SheItem(int diameter, int width, Viewer* parent = nullptr);
	void move(Viewer::Direction direction);

	~SheItem();
	void initialize();
	void setColor(Qt::GlobalColor color);
	void setDirection(Viewer::Direction direction);
	bool isCovered(QPoint point);
	
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	void stop();
	void start();
	void setStartPoint(QPoint startPoint);
	void setFood(QPoint food) { _food = food; }

signals:
	void foodWasAte();
	void againstShe();
	void againstWall();
	void levelChanged(int);

protected:
	void scored(int x = 1);
	void move();

	std::list<QPoint> _path;
	QTimer* _timer;
	QPoint _food;
	QPoint _startPoint;
	Viewer::Direction _direction;
	Viewer* _parent;
	int _width;
	int _diameter;
	int _timeout;
	int _score;
	int _level;
	Qt::GlobalColor _color;
};

