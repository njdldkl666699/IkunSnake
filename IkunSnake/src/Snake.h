#pragma once
#include"libs.h"
#include"Point.h"
#include"Food.h"

class Snake
{
public:
	enum Direction
	{
		Up, Left, Down, Right, None
	};

	enum State
	{
		hitBorder, hitBody, ateFood, normal
	};

	Snake();

	void draw();

	void setDirection(Direction dir);

	State play(Food& egg, Food& resin);

	const auto& getData()const { return this->data; }

	Point getHeadPos()const { return *headPos; }

private:
	void drawHead()const;

	void moveHead();

	//自动移动
	void move();

	//边界检测
	State isHitBorder();

	//撞身体检测
	State isHitBody();

	//吃东西检测
	State isAteFood(Food& food);

#ifdef _DEBUG
	void consolePrintSnake();
#endif // _DEBUG

private:
	IMAGE body;
	std::vector<Point> data;
	Direction direction;
	Point* headPos;
	Point endPos;
};