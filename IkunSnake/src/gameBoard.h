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

	size_t getScore() const { return score; }

private:
	Snake snake;
	Food egg;
	Food resin;
	size_t score;
};

