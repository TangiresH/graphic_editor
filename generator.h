#pragma once
#include "framework.h"
#include <string>
#include <cmath>

class Generator
{
public:
	Generator(void);
	static std::wstring GetBoxText(HWND, int);
	static void SendData(HWND, HWND, void*, long);
	void Start(HINSTANCE, HWND);
	void Close();
};
