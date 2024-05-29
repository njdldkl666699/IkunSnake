#include"UISystem.h"

using namespace std::chrono_literals;
using std::this_thread::sleep_for;

auto playSpeed = 300ms;
const auto flushSpeed = 10ms;
const auto gameOverWait = 1s;

const size_t winScore = 100;

UISystem::UISystem()
	:isLost(false), isMsTrdEnd(false),
	mouseThread(&UISystem::getMouseClick, this)
{
#ifdef _DEBUG
	initgraph(widgetWidth, widgetHeight, EX_SHOWCONSOLE);
#else // RELEASE
	initgraph(widgetLength, widgetHeight);
#endif
	mouseThread.detach();
	BeginBatchDraw();
	while(true)
	{
		bool isExit = gameStart();
		if (isExit)
			break;
		timer.start();
		gamePlay();
		timer.stop();
		gameOver();
	}
	isMsTrdEnd = true;
}

UISystem::~UISystem()
{
	EndBatchDraw();
	closegraph();
}

void UISystem::drawSystem()
{
	setlinecolor(RGB(85, 85, 85));
	for (int i = 0; i <= xNum; i++)
		line(xBegPos + i * blockLength, yBegPos, xBegPos + i * blockLength, yEndPos);
	for (int j = 0; j <= yNum; j++)
		line(xBegPos, yBegPos + j * blockLength, xEndPos, yBegPos + j * blockLength);

	RECT tagRct = { widgetWidth * 3 / 4,widgetHeight / 4 + 28, widgetWidth, widgetHeight / 4 + 56 };
	drawSetText("�÷�", &tagRct, 28, _T("��Բ"), BLACK, FW_BOLD);
	drawTips();
}

void UISystem::refresh()
{
	cleardevice();
	loadimage(NULL, _T(".\\res\\playbk.jpg"), widgetWidth, widgetHeight);
	drawSystem();
	gameboard.draw();
	FlushBatchDraw();
}

void UISystem::getMouseClick()
{
	static ExMessage mouseEvent;
	while (!isMsTrdEnd)
	{
		mouseEvent = getmessage(EX_MOUSE);
		if (mouseEvent.message == WM_LBUTTONDOWN)
			mouse = mouseEvent;
	}
}

bool UISystem::gameStart()
{
	while (true)
	{
		loadimage(NULL, _T(".\\res\\gamemenu.png"), widgetWidth, widgetHeight);
		FlushBatchDraw();

		if (mouse.x > 235 && mouse.x < 475)
		{
			//GameStart Button 235,175;475,260
			if (mouse.y > 175 && mouse.y < 260 && mouse.message == WM_LBUTTONDOWN)
			{
				mouse.message = WM_MOUSEMOVE;
				break;
			}
			//GameRule Button 235,300;475,385
			if (mouse.y > 300 && mouse.y < 385 && mouse.message == WM_LBUTTONDOWN)
			{
				mouse.message = WM_MOUSEMOVE;
				loadimage(NULL, _T(".\\res\\gamerule.png"), widgetWidth, widgetHeight);
				FlushBatchDraw();
				while (true)
				{
					//Back Button 408,473;618,547
					if (mouse.x > 408 && mouse.x < 618 && mouse.y > 473 && mouse.y < 547 
						&& mouse.message == WM_LBUTTONDOWN)
					{
						mouse.message = WM_MOUSEMOVE;
						break;
					}
					sleep_for(flushSpeed);
				}
			}
			//GameExit Button 235,425;475,510
			if (mouse.y > 425 && mouse.y < 510 && mouse.message == WM_LBUTTONDOWN)
			{
				mouse.message = WM_MOUSEMOVE;
				return true;
			}
		}
		sleep_for(flushSpeed);
	}

	//��������
	mciSendString(_T("open .\\res\\bgm.mp3 alias bgm"), 0, 0, 0);
	mciSendString(_T("play bgm repeat"), 0, 0, 0);
	return false;
}

void UISystem::gamePlay()
{
	while (!isLost)
	{
		refresh();
		std::this_thread::sleep_for(playSpeed);
		if (_kbhit())
		{
			bool isPause = gameboard.rotate(_getch());
			if (isPause)
			{
				timer.pause();
				(void)_getch();
				timer.restore();
			}
		}
		if (GetAsyncKeyState('J') & 0x8000)
			playSpeed = 150ms;
		else
			playSpeed = 300ms;

		isLost = gameboard.autoPlay();
	}
}

void UISystem::gameOver()
{
	drawTips();
	FlushBatchDraw();
	mciSendString(_T("stop bgm"), 0, 0, 0);
	PlaySound(_T(".\\res\\ngm.wav"), NULL, SND_FILENAME | SND_ASYNC);
	sleep_for(gameOverWait);

	cleardevice();

	static const int maxScore = xNum * yNum - 2;
	IMAGE img(widgetWidth, widgetHeight);
	if (gameboard.getScore() < winScore)
	{
		//fail
		loadimage(&img, _T(".\\res\\gamefail.png"), widgetWidth, widgetHeight);
	}
	else
	{
		//win
		loadimage(&img, _T(".\\res\\gamewin.png"), widgetWidth, widgetHeight);

		//�ʵ�
		if (gameboard.getScore() >= maxScore)
			system(".\\res\\����̫��.mp4");
	}
	putimage(0, 0, &img);

	RECT textRct = { 0,widgetHeight / 8,widgetWidth,widgetHeight / 2 };
	std::string text = "��Ϸ������\n���ĵ÷�Ϊ��" + std::to_string(gameboard.getScore())
		+ " ��\n����ʱ�䣺" + std::to_string((int)this->timer.duration()) + " ��";
	drawSetText(text, &textRct, 60, _T("��Բ"), BLACK, FW_DONTCARE);

	FlushBatchDraw();
	while (true)
	{
		//Back Button 408,473;620,550
		if (mouse.x > 408 && mouse.x < 620 && mouse.y > 473 && mouse.y < 550
			&& mouse.message == WM_LBUTTONDOWN)
		{
			mouse.message = WM_MOUSEMOVE;
			break;
		}
	}

	//reset
	gameboard = GameBoard();
	mciSendString(_T("close bgm"), 0, 0, 0);
	isLost = false;
}

void UISystem::drawTips() const
{
	RECT tipRct = { widgetWidth * 2 / 3 - 80,widgetHeight / 2 + 80, widgetWidth - 80, widgetHeight * 3 / 4 };
	std::string tip;
	switch (gameboard.getScore())
	{
	case 0:
		tip = "�����~����Ϸ��������";
		break;
	case 1:
		tip = "Σ";
		break;
	case 50:
		tip = "ֻ����̫�� baby";
		break;
	case winScore:
		tip = "ֻ����ʵ����̫�� baby\n����Ӯ�ˣ�";
		break;
	case 150:
		tip = "ӭ��������������\n��˴�������";
		break;
	case 200:
		tip = "���ָо��Ҵ�δ��\nCause I got a crush on you who you";
		break;
	case 250:
		tip = "�����ҵ��������˭";
		break;
	default:
		break;
	}
	if (isLost)
	{
		tip = "�����~����Ϸ��������";
	}
	drawSetText(tip, &tipRct, 28, _T("��Բ"), BLACK, FW_BOLD);
}