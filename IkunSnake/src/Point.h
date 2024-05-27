#pragma once

class Point
{
public:
	Point():x(0), y(0){};

	Point(int xPos, int yPos) :x(xPos), y(yPos) {};

	bool operator==(const Point& other)const
	{
		return (this->x == other.x && this->y == other.y);
	}

	bool operator!=(const Point& other)const
	{
		return !(*this == other);
	}

	int x;
	int y;
};

