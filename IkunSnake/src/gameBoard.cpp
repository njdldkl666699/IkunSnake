#include"GameBoard.h"

GameBoard::GameBoard() :snake(),
	egg(1, _T(".\\res\\egg.png")), resin(-1, _T(".\\res\\resin.png"))
{
	egg.setPos(snake.getData());
	resin.setPos(snake.getData(), &egg);
}

bool GameBoard::rotate(char input)
{
	using Dir = Snake::Direction;
	Dir dir;
	switch (input)
	{
	case 'w':
		dir = Dir::Up;
		break;
	case 'a':
		dir = Dir::Left;
		break;
	case 's':
		dir = Dir::Down;
		break;
	case 'd':
		dir = Dir::Right;
		break;
	case 'p':
		return true;
	default:
		break;
	}
	snake.setDirection(dir);
	return false;
}

bool GameBoard::autoPlay()
{
	Snake::State state = snake.play(egg, resin);
	if (state == Snake::State::hitBody)
		return true;
	return false;
}

void GameBoard::draw()
{
	snake.draw();
	egg.draw();
	resin.draw();
	score = snake.getData().size();
	RECT scoreRct = { widgetLength * 3 / 4, widgetLength / 4 - 48, widgetLength, widgetLength / 2 };
	drawSetText(std::to_string(score), &scoreRct, 48, _T("свт╡"), BLACK, FW_DONTCARE);
}