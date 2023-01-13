#include "graph.h"

GraphMaker::GraphMaker(void) {}

void GraphMaker::OnCreate(HWND hWnd)
{
	GetTextFromClipboard(hWnd, 65536);
	pointsArray = GetArray(arrayText);
	SortArray();

	InvalidateRect(hWnd, NULL, TRUE);
}

int GraphMaker::GetTextFromClipboard(HWND hWnd, long maxsize)
{
	HGLOBAL hglb;
	LPTSTR lptstr;
	long size, res;
	res = 0;
	char* dest = new char[maxsize];
	if (!IsClipboardFormatAvailable(CF_TEXT)) return 0;
	if (!OpenClipboard(hWnd)) return 0;
	hglb = GetClipboardData(CF_TEXT);
	if (hglb != NULL)
	{
		lptstr = (LPTSTR)GlobalLock(hglb);
		if (lptstr != NULL)
		{
			size = (long)strlen((char*)lptstr);
			if (size > maxsize)
			{
				lptstr[maxsize] = 0;
				size = (long)strlen((char*)lptstr);
			}
			res = size;
			strcpy_s(dest, maxsize, (char*)lptstr);
			GlobalUnlock(hglb);
		}
	}
	CloseClipboard();
	arrayText = dest;
	return res;
}

int** GraphMaker::GetArray(std::string text)
{
	std::stringstream stream(text);
	int num = (int)std::count(text.cbegin(), text.cend(), '\n');
	nPoint = num;
	int** result = new int* [num];
	for (int i = 0; i < num; i++)
		result[i] = new int[2];

	int element;
	int i = 0;

	for (int i = 0; i < num; i++)
	{
		stream >> element;
		result[i][0] = element;
		stream >> element;
		result[i][1] = element;
	}

	return result;
}


void GraphMaker::OnPaint(HWND hWnd, HDC hdc)
{
	HPEN hPen, hPenOld;
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hPenOld = (HPEN)SelectObject(hdc, hPen);
	MoveToEx(hdc, startx - graphWidth, starty, NULL);
	LineTo(hdc, startx + graphWidth, starty);

	MoveToEx(hdc, startx, starty + graphHeight, NULL);
	LineTo(hdc, startx, starty - graphHeight);

	MoveToEx(hdc, startx - 200, starty - 5 , NULL);
	LineTo(hdc, startx - 200, starty + 5);
	MoveToEx(hdc, startx - 100, starty - 5, NULL);
	LineTo(hdc, startx - 100, starty + 5);
	MoveToEx(hdc, startx + 100, starty - 5, NULL);
	LineTo(hdc, startx + 100, starty + 5);
	MoveToEx(hdc, startx + 200, starty - 5, NULL);
	LineTo(hdc, startx + 200, starty + 5);

	MoveToEx(hdc, startx - 5, starty - 200, NULL);
	LineTo(hdc, startx + 5, starty - 200);
	MoveToEx(hdc, startx - 5, starty - 100, NULL);
	LineTo(hdc, startx + 5, starty - 100);
	MoveToEx(hdc, startx - 5, starty + 100, NULL);
	LineTo(hdc, startx + 5, starty + 100);
	MoveToEx(hdc, startx - 5, starty + 200, NULL);
	LineTo(hdc, startx + 5, starty + 200);



	TextOut(hdc, startx + graphWidth - 8, starty + 3, L"x", 1);
	TextOut(hdc, startx - 12, starty - graphWidth + 3, L"y", 1);
	TextOut(hdc, startx - 10, starty + 4, L"0", 1);

	TextOut(hdc, startx - 210, starty + 4, L"-200", 4);
	TextOut(hdc, startx - 110, starty + 4, L"-100", 4);
	TextOut(hdc, startx + 90, starty + 4, L"100", 4);
	TextOut(hdc, startx + 190, starty + 4, L"200", 4);

	TextOut(hdc, startx - 35, starty - 210, L"200", 4);
	TextOut(hdc, startx - 35, starty - 110, L"100", 4);
	TextOut(hdc, startx - 35, starty + 90, L"-100", 4);
	TextOut(hdc, startx - 35, starty + 190, L"-200", 4);

	int x1, y1, x2, y2;
	x1 = startx + pointsArray[0][0];
	y1 = starty - pointsArray[0][1];
	for (int i = 1; i < nPoint; i++)
	{
		if (i <= nPoint - 1) {
			x2 = startx + pointsArray[i][0];
			y2 = starty - pointsArray[i][1];
			MoveToEx(hdc, x1, y1, NULL);
			LineTo(hdc, x2, y2);
			x1 = x2;
			y1 = y2;
		}
		else break;
	}
	
	SelectObject(hdc, hPenOld);
	DeleteObject(hPen);
}

void GraphMaker::SortArray() 
{
	int i, j;
	for (i = 0; i < nPoint - 1; i++) 
	{
		for (j = 0; j < nPoint - 1; j++)
		{
			if (pointsArray[j][0] > pointsArray[j + 1][0])
				std::swap(pointsArray[j], pointsArray[j + 1]);
		}
	}
}