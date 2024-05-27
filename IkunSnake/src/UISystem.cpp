#include"UISystem.h"

UISystem::UISystem()
{
#ifdef _DEBUG
	initgraph(widgetLength, widgetHeight, EX_SHOWCONSOLE);
#else // RELEASE
	initgraph(widgetLength, widgetHeight);
#endif
	BeginBatchDraw();
	gameStart();
	timer.start();
	gamePlay();
	timer.stop();
	gameOver();
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

	RECT tagRct = { widgetLength * 3 / 4,widgetHeight / 4 + 28, widgetLength, widgetHeight / 4 + 56 };
	drawSetText("得分", &tagRct, 28, _T("幼圆"), BLACK, FW_BOLD);
	drawTips();
}

void UISystem::refresh()
{
	cleardevice();
	loadimage(NULL, _T(".\\res\\playbk.jpg"));
	drawSystem();
	gameboard.draw();
	FlushBatchDraw();
	Sleep(playSpeed);
}

void UISystem::gameStart()
{
	loadimage(NULL, _T(".\\res\\bk.jpg"));
	IMAGE img;
	loadimage(&img, _T(".\\res\\gamestart.jpg"), 256, 256);
	putimage(widgetLength / 2 - 128, widgetHeight / 2 - 160, &img);

	RECT chRct = { 0, widgetHeight / 2 + 128, widgetLength, widgetHeight / 2 + 128 + 28 };
	drawSetText("按任意键开始", &chRct, 28, _T("幼圆"), DARKGRAY, FW_BOLD);
	RECT enRct = { 0,widgetHeight / 2 + 128 + 40,widgetLength,widgetLength / 2 + 128 + 64 };
	drawSetText("Press Any Key To Start", &enRct, 26, _T("等线"), LIGHTGRAY, FW_DONTCARE);

	FlushBatchDraw();
	std::fstream fs(".\\firstrun", std::ios::in | std::ios::binary);
	if (!fs.is_open())
	{
		fs.open(".\\firstrun", std::ios::out | std::ios::binary);
		fs.write("你好，小黑子！", 22);
		fs.close();
		system(".\\res\\README.txt");
	}
	(void)_getch();
	this->isLost = false;

	//播放音乐
	mciSendString(_T("open .\\res\\bgm.mp3 alias bgm"), 0, 0, 0);
	mciSendString(_T("play bgm repeat"), 0, 0, 0);
	//mciSendString(_T("repeat bgm"), 0, 0, 0);
}

void UISystem::gamePlay()
{
	while (!isLost)
	{
		refresh();
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
		isLost = gameboard.autoPlay();
	}
	return;
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

	RECT textRct = { 0,widgetHeight / 8,widgetLength,widgetHeight / 2 };
	std::string text = "游戏结束！\n您的得分为：" + std::to_string(gameboard.getScore())
		+ "分\n游玩时间：" + std::to_string((int)this->timer.duration()) + "秒";
	drawSetText(text, &textRct, 60, _T("幼圆"), BLACK, FW_DONTCARE);

	RECT rankRct = { 0,widgetHeight / 2 + 24 ,widgetLength,widgetHeight * 3 / 4 + 24 };
	const int maxScore = xNum * yNum - 2;
	IMAGE img;
	if (gameboard.getScore() < winScore)
	{
		//fail
		drawSetText("蔡", &rankRct, 96, _T("幼圆"), RED, FW_BOLD);
		loadimage(&img, _T(".\\res\\gamefail.jpg"), 240, 320);
	}
	else
	{
		//win
		drawSetText("你干嘛 ~ 啊哈 ~ 哎呦 ~ ", &rankRct, 36, _T("幼圆"), BLACK, FW_BOLD);
		loadimage(&img, _T(".\\res\\gamewin.jpg"), 280, 320);
		//彩蛋
		if (gameboard.getScore() == maxScore)
			system(".\\res\\鸡你太美.mp4");
	}
	putimage(widgetLength - 280, widgetHeight - 360, &img);
	FlushBatchDraw();
}

void UISystem::drawTips()
{
	RECT tipRct = { widgetLength * 2 / 3 - 80,widgetHeight / 2 + 80, widgetLength - 80, widgetHeight * 3 / 4 };
	std::string tip;
	switch (gameboard.getScore())
	{
	case 0:
		tip = "你干嘛~（游戏结束！）";
		break;
	case 1:
		tip = "危";
		break;
	case 50:
		tip = "只因你太美 baby";
		break;
	case winScore:
		tip = "只因你实在是太美 baby\n（你赢了）";
		break;
	case 150:
		tip = "迎面走来的你让我\n如此蠢蠢欲动";
		break;
	case 200:
		tip = "这种感觉我从未有\nCause I got a crush on you who you";
		break;
	case 250:
		tip = "你是我的我是你的谁";
		break;
	default:
		break;
	}
	if (isLost)
	{
		tip = "你干嘛~（游戏结束！）";
	}
	drawSetText(tip, &tipRct, 28, _T("幼圆"), BLACK, FW_BOLD);
}