#pragma once
#include "framework.h"
#include <string>
#include <sstream>
#include <codecvt>
#include <iomanip>

class GraphMaker
{
private:
	int startx = 350, starty = 350, graphWidth = 240, graphHeight = 240;
	int nPoint = 0;
	std::string arrayText = "";
	int** pointsArray = 0;
public:
	GraphMaker(void);
	void OnCreate(HWND);
	int GetTextFromClipboard(HWND, long);
	int** GetArray(std::string);
	void SortArray();
	void OnPaint(HWND, HDC);
};