#pragma once
#include"libs.h"
#include"Timer.h"
#include"GameBoard.h"

class UISystem
{
public:
	UISystem();

	~UISystem();

private:
	void drawSystem();

	void refresh();

	void gameStart();

	void gamePlay();

	void gameOver();

	void drawTips();

private:
	GameBoard gameboard;
	Timer timer;
	bool isLost;
};
