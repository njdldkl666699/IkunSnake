#pragma once
#include"Food.h"
#include"Snake.h"

class GameBoard
{
public:
	GameBoard();

	bool rotate(char input);

	bool autoPlay();

	void draw();

	size_t getScore() const 
	{
		return snake.getData().size();
	}

private:
	Snake snake;
	Food egg;
	Food resin;
};

