#include "Snake.h"
using State=Snake::State;

Snake::Snake()
	:body(blockLength, blockLength), direction(Right), headPos(nullptr), endPos(),
#ifdef _DEBUG
	data{ {14,11},{13,11},{13,10},{12,10},{11,10},{11,9},{11,8},{11,7},{10,7},{9,7},{8,7},{7,7} }
#else //RELEASE
	data{ {2,0},{1,0},{0,0} }
#endif
{
	loadimage(&body, _T(".\\res\\basketball.png"), blockLength, blockLength);
	this->headPos = &data.at(0);
	this->endPos = data.back();
}

void Snake::draw()
{
	if (!data.empty())
	{
		drawHead();
		for (size_t i = 1; i < data.size(); i++)
			drawImg(data[i].x, data[i].y, body);
	}
}

void Snake::setDirection(Direction dir)
{
	if (dir != direction && dir != direction - 2 && dir != direction + 2)
	{
		direction = dir;
#ifdef _DEBUG
		consolePrintSnake();
#endif // _DEBUG
	}
}

State Snake::play(Food& egg, Food& resin)
{
	//自动移动
	move();
	//边界检测
	State hitBdr = isHitBorder();
	//撞身体检测
	State hitBdy = isHitBody();
	if (hitBdy == hitBody)
		return hitBody;
	//吃东西检测
	State ateEgg = isAteFood(egg);
	State ateResin = isAteFood(resin);
	if (ateEgg == ateFood || ateResin == ateFood)
		return ateFood;
	return normal;
}

#ifdef _DEBUG
void Snake::consolePrintSnake()
{
	for (const auto& it : this->data)
		std::cout << '(' << it.x << ',' << it.y << ')';
	std::cout << std::endl;
}
#endif // _DEBUG

void Snake::drawHead()const
{
	IMAGE head(blockLength, blockLength);
	std::string headStr = ".\\res\\ikun" + std::to_string(this->direction) + ".png";
	TCHAR headTStr[20];
	MultiByteToWideChar(CP_ACP, 0, headStr.c_str(), -1, headTStr, headStr.size() + 1);
	loadimage(&head, headTStr, blockLength, blockLength);
	drawImg(headPos->x, headPos->y, head);
}

void Snake::moveHead()
{
	switch (direction)
	{
	case Up:
		headPos->y--;
		break;
	case Left:
		headPos->x--;
		break;
	case Down:
		headPos->y++;
		break;
	case Right:
		headPos->x++;
		break;
	default:
		break;
	}
}

//自动移动
void Snake::move()
{
	endPos = data.back();
	for (size_t i = data.size() - 1; i > 0; i--)
		data[i] = data[i - 1];
	moveHead();
}

//边界检测
State Snake::isHitBorder()
{
	bool hitUp = headPos->y == -1 && direction == Up;
	bool hitLeft = headPos->x == -1 && direction == Left;
	bool hitDown = headPos->y == yNum && direction == Down;
	bool hitRight = headPos->x == xNum && direction == Right;

	if (hitUp)
		headPos->y = yNum - 1;
	else if (hitLeft)
		headPos->x = xNum - 1;
	else if (hitDown)
		headPos->y = 0;
	else if (hitRight)
		headPos->x = 0;

	bool rlt = hitUp || hitDown || hitLeft || hitRight;
	return rlt ? hitBorder : normal;
}

//撞身体检测
State Snake::isHitBody()
{
	for (size_t i = 1; i < data.size(); i++)
		if (*this->headPos == this->data[i])
			return State::hitBody;
	return State::normal;
}

//吃东西检测
State Snake::isAteFood(Food& food)
{
	int rlt = food.isAte(headPos);
	if (rlt > 0)
	{
		data.push_back(endPos);
		headPos = &data.at(0);
		food.setPos(data);
		return State::ateFood;
	}
	else if (rlt < 0 && !data.empty())
	{
		data.pop_back();
		headPos = &data.at(0);
		food.setPos(data);
		return State::ateFood;
	}
	return State::normal;
}