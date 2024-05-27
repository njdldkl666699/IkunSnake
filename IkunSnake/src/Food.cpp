#include"Food.h"

Food::Food(int eff, LPCTSTR filepath)
	: effect(eff)
{
	loadimage(&img, filepath, blockLength, blockLength);
}

void Food::setPos(const std::vector<Point>& data, Food* other)
{
	int xPos, yPos;
	bool isCoincident = true;
	while (isCoincident)
	{
		xPos = rand() % xNum;
		yPos = rand() % yNum;
		if (other != nullptr && other->x == xPos && other->y == yPos)
			continue;

		isCoincident = false;
		for (size_t i = 1; i < data.size(); i++)
		{
			if (*this == data[i])
			{
				isCoincident = true;
				break;
			}
		}
	}
	x = xPos;
	y = yPos;
}

int Food::isAte(const Point* headPos)const
{
	bool rlt = (*this == *headPos);
	if (rlt)
		return this->effect;
	return 0;
}

void Food::draw()
{
	return drawImg(x, y, img);
}