#pragma once
#include"libs.h"
#include"Point.h"

class Food :public Point
{
public:
	Food(int eff, LPCTSTR filepath);

	void setPos(const std::vector<Point>& data, Food* other = nullptr);

	int isAte(const Point* headPos)const;

	void draw();

private:
	int effect;
	IMAGE img;
};

