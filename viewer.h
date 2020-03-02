#pragma once
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include <vector>

class SheItem;

class Viewer :public QGraphicsView
{
	Q_OBJECT
public:
	enum class State
	{
		PAUSE,
		START,
		GAMEOVER
	};
	enum Direction
	{
		DEFUALT = 0,
		UP = 1,
		DOWN = -1 ^ 1,
		LEFT = 2,
		RIGHT = -1 ^ 2
	};
	Viewer(QWidget* parent = nullptr);

	~Viewer();
	void keyPressEvent(QKeyEvent* event);
	void showMessage(const QString& message, int timeout = 0);
	void gameStart();
	void gamePause();
	void generateFood();
	bool isHited(QPoint point);
	void gameOver();

	int getSheWidth() { return _sheDiameter; }

signals:
	void mainWindowMessage(const QString&, int);
	void updateLeftLevelLabel(int);
	void updateRightLevelLabel(int);
	void updateFood(QPoint);


private:
	void start();
	void stop();
	void update(SheItem* sheItem, Direction direction = Direction::DEFUALT);
	void updateLeftLevel(int x);
	void updateRightLevel(int x);
	void leftAgainstWall();
	void leftAgainstShe();
	void rightAgainstWall();
	void rightAgainstShe();

	std::vector<SheItem*> _she;
	QPoint _food;
	QGraphicsEllipseItem* _foodItem;
	State _state;
	int _sheDiameter;
	int _width;
	QGraphicsSimpleTextItem* _text;
};