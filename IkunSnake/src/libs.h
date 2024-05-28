#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<conio.h>
#include<string>
#include<vector>
#include<fstream>
#include<ctime>
#include<chrono>
#include<thread>

//“Ù¿÷
#include<Windows.h>
#include<mmsystem.h>
#pragma comment (lib,"winmm.lib")

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

const int widgetLength = 1024, widgetHeight = 576;
const int blockLength = 30;
const int xBegPos = 20, yBegPos = 20;
const int xNum = (widgetLength - 2 * xBegPos) / blockLength - 13;
const int yNum = (widgetHeight - 2 * yBegPos) / blockLength;
const int xEndPos = xBegPos + xNum * blockLength;
const int yEndPos = yBegPos + yNum * blockLength;
const double PI = 3.14159265359;

void putimagePNG(int picture_x, int picture_y, IMAGE* picture);

void drawImg(const int x, const int y, IMAGE &img);

// using std::string rather than LPCTSTR !!
void drawSetText(const std::string& str, RECT* rect, const int height,
    LPCTSTR font, COLORREF color, LONG weight);