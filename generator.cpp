#include "generator.h"
#include "generator.rh"

INT_PTR CALLBACK Gener(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            int nPoint = 0;
            int xMin = 0;
            int xMax = 0;
            int yMin = 0;
            int yMax = 0;

            try
            {
                nPoint = (int)round(stod(Generator::GetBoxText(hDlg, IDC_NPOINT)));
                xMin = (int)round(stod(Generator::GetBoxText(hDlg, IDC_XMIN)));
                xMax = (int)round(stod(Generator::GetBoxText(hDlg, IDC_XMAX)));
                yMin = (int)round(stod(Generator::GetBoxText(hDlg, IDC_YMIN)));
                yMax = (int)round(stod(Generator::GetBoxText(hDlg, IDC_YMAX)));
            }
            catch (...)
            {
                MessageBox(hDlg, L"Неправильний формат", L"Помилка", MB_OK | MB_ICONERROR);
                break;
            }

            if (nPoint < 2 || nPoint > 30)
            {
                MessageBox(hDlg, L"кількість точок повинна бути від 2 до 30", L"Спробуйте ще", MB_OK | MB_ICONERROR);
                break;
            }
            if (abs(xMin) > 240 || abs(xMax) > 240 || abs(yMin) > 240 || abs(yMax) > 240)
            {
                MessageBox(hDlg, L"координати повинні бути за модулем не більше 240", L"Спробуйте ще", MB_OK | MB_ICONERROR);
                break;
            }
            if (xMin > xMax || yMin > yMax)
            {
                MessageBox(hDlg, L"Неправильно введені дані(min быльше за max)", L"Спробуйте ще", MB_OK | MB_ICONERROR);
                break;
            }

            HWND hWnd2 = FindWindow(L"OBJECT2", NULL);
            if (!hWnd2)
            {
                WinExec("Object2.exe", SW_SHOW);
                hWnd2 = FindWindow(L"OBJECT2", NULL);
            }
            int data[5] = { nPoint, xMin, xMax, yMin, yMax };
            Generator::SendData(hWnd2, GetParent(hDlg), data, sizeof(data));

            HWND hWnd3 = FindWindow(L"OBJECT3", NULL);
            if (!hWnd3)
            {
                WinExec("Object3.exe", SW_SHOW);
                hWnd3 = FindWindow(L"OBJECT3", NULL);
            }
            PostMessage(hWnd3, WM_CLIPBOARDUPDATE, NULL, NULL);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

Generator::Generator(void) {}

std::wstring Generator::GetBoxText(HWND hWnd, int boxID)
{
    WCHAR buff[8];
    GetWindowText(GetDlgItem(hWnd, boxID), buff, 8);

    return buff;
}

void Generator::SendData(HWND hWndDest, HWND hWndSrc, void* lp, long cb)
{
    COPYDATASTRUCT cds;

    cds.dwData = 1;
    cds.cbData = cb;
    cds.lpData = lp;

    SendMessage(hWndDest, WM_COPYDATA, (WPARAM)hWndSrc, (LPARAM)&cds);
}

void Generator::Start(HINSTANCE hInst, HWND hWnd)
{
    DialogBox(hInst, MAKEINTRESOURCE(IDD_GENERATOR), hWnd, Gener);
}

void Generator::Close()
{
    HWND hWnd2 = FindWindow(L"OBJECT2", NULL);
    HWND hWnd3 = FindWindow(L"OBJECT3", NULL);

    if (hWnd2) PostMessage(hWnd2, WM_DESTROY, NULL, NULL);
    if (hWnd3) PostMessage(hWnd3, WM_DESTROY, NULL, NULL);
}