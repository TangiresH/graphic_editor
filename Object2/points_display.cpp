#include "points_display.h"

PointsDisplay::PointsDisplay(void) {}

void PointsDisplay::OnCopyData(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* cds;
	cds = (COPYDATASTRUCT*)lParam;
	int* p = (int*)cds->lpData;
	nPoint = p[0];
	xMin = p[1];
	xMax = p[2];
	yMin = p[3];
	yMax = p[4];

	pointsArray = CreateArray(nPoint, xMin, xMax, yMin, yMax);
	arrayText = GetArrayStr(pointsArray, nPoint);
	PutTextToClipboard(hWnd, arrayText.c_str());
	InvalidateRect(hWnd, NULL, TRUE);
}

void PointsDisplay::OnPaint(HWND hWnd, HDC hdc)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	std::string sElem;
	std::wstring wsElem;
	double elem;

	TextOut(hdc, 22, 4, L"x", 1);
	TextOut(hdc, 72, 4, L"y", 1);
	int x = 20, y = 25;
	
	for (int i = 0; i < nPoint; i++)
	{
		
		std::stringstream ss1, ss2;
		elem = pointsArray[i][0];
		ss1 << elem;
		ss1 >> sElem;
		wsElem = converter.from_bytes(sElem);
		TextOut(hdc, x, y, (LPCWSTR)wsElem.c_str(), (int)wcslen((LPCWSTR)wsElem.c_str()));

		x += 50;

		elem = pointsArray[i][1];
		ss2 << elem;
		ss2 >> sElem;
		wsElem = converter.from_bytes(sElem);
		TextOut(hdc, x, y, (LPCWSTR)wsElem.c_str(), (int)wcslen((LPCWSTR)wsElem.c_str()));
		x = 20;
		y += 20;
	}
}

int** PointsDisplay::CreateArray(int num, int xmin, int xmax, int ymin, int ymax)
{
	int** result = new int* [num];
	for (int i = 0; i < num; i++)
		result[i] = new int[2];

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distx(xmin, xmax);
	std::uniform_int_distribution<> disty(ymin, ymax);

	for (int i = 0; i < num; i++) 
	{
		result[i][0] = distx(gen);
		result[i][1] = disty(gen);
	}

	return result;
}

std::string PointsDisplay::GetArrayStr(int** arraySource, int num)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	std::string result;
	std::ostringstream stream;
	int elementx, elementy;

	for (int i = 0; i < num; i++)
	{
		elementx = arraySource[i][0];
		elementy = arraySource[i][1];
		stream << elementx << '\t' << elementy;
		stream << '\n';
	}
	result = stream.str();

	return result;
}

int PointsDisplay::PutTextToClipboard(HWND hWnd, const char* src)
{
	HGLOBAL hglbCopy;
	BYTE* pTmp;
	long len;

	if (src == NULL) return 0;
	if (src[0] == 0) return 0;

	len = (long)strlen(src);
	hglbCopy = GlobalAlloc(GHND, len + 1);

	if (hglbCopy == NULL) return 0;

	pTmp = (BYTE*)GlobalLock(hglbCopy);
	memcpy(pTmp, src, len + 1);
	GlobalUnlock(hglbCopy);

	if (!OpenClipboard(hWnd))
	{
		GlobalFree(hglbCopy);
		return 0;
	}

	EmptyClipboard();
	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();

	return 1;
}