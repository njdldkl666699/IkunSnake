#include"UISystem.h"

using namespace std::chrono_literals;
//const int playSpeed = 300;
auto playSpeed = 300ms;
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
	(void)_getch();
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
	//Sleep(playSpeed);
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

		if (mouse.x > 235 && mouse.x < 475&&mouse.message==WM_LBUTTONDOWN)
		{
			mouse.message = WM_MOUSEMOVE;
			//GameStart Button 235,175;475,260
			if (mouse.y > 175 && mouse.y < 260)
			{
				break;
			}
			//GameRule Button 235,300;475,385
			if (mouse.y > 300 && mouse.y < 385)
			{
				loadimage(NULL, _T(".\\res\\gamerule.png"), widgetWidth, widgetHeight);
				FlushBatchDraw();
				while (true)
				{
					//Back Button 
					
				}
			}
			//GameExit Button 235,425;475,510
			if (mouse.y > 425 && mouse.y < 510)
			{
				return true;
			}
		}
	}

	//��������
	mciSendString(_T("open .\\res\\bgm.mp3 alias bgm"), 0, 0, 0);
	mciSendString(_T("play bgm repeat"), 0, 0, 0);
	return false;
}

void UISystem::gamePlay()
{
	//gameboard = GameBoard();
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
	(void)_getch();
	cleardevice();
	loadimage(NULL, _T(".\\res\\bk.jpg"));

	RECT textRct = { 0,widgetHeight / 8,widgetWidth,widgetHeight / 2 };
	std::string text = "��Ϸ������\n���ĵ÷�Ϊ��" + std::to_string(gameboard.getScore())
		+ "��\n����ʱ�䣺" + std::to_string((int)this->timer.duration()) + "��";
	drawSetText(text, &textRct, 60, _T("��Բ"), BLACK, FW_DONTCARE);

	RECT rankRct = { 0,widgetHeight / 2 + 24 ,widgetWidth,widgetHeight * 3 / 4 + 24 };
	const int maxScore = xNum * yNum - 2;
	IMAGE img;
	if (gameboard.getScore() < winScore)
	{
		//fail
		drawSetText("��", &rankRct, 96, _T("��Բ"), RED, FW_BOLD);
		loadimage(&img, _T(".\\res\\gamefail.jpg"), 240, 320);
	}
	else
	{
		//win
		drawSetText("����� ~ ���� ~ ���� ~ ", &rankRct, 36, _T("��Բ"), BLACK, FW_BOLD);
		loadimage(&img, _T(".\\res\\gamewin.jpg"), 280, 320);
		//�ʵ�
		if (gameboard.getScore() == maxScore)
			system(".\\res\\����̫��.mp4");
	}
	putimage(widgetWidth - 280, widgetHeight - 360, &img);
	FlushBatchDraw();
}

void UISystem::drawTips()
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