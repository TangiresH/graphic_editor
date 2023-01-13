#pragma once
#include "framework.h"
#include <string>
#include <random>
#include <sstream>
#include <codecvt>

class PointsDisplay
{
private:

	int nPoint = 0;
	int xMin = 0;
	int xMax = 0;
	int yMin = 0;
	int yMax = 0;

	int** pointsArray = 0;
	std::string arrayText = "";
public:
	PointsDisplay(void);
	void OnCopyData(HWND, WPARAM, LPARAM);
	int** CreateArray(int, int, int, int, int);
	std::string GetArrayStr(int**, int);
	int PutTextToClipboard(HWND, const char*);
	void OnPaint(HWND, HDC);
};