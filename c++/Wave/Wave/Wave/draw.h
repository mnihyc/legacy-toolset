#pragma once

int drawevery = 600;

#include "def.h"
#include "debug.h"
/*#include "Chart.h"  
#ifdef _DEBUG  
#pragma comment(lib, "PlotDll_d.lib")  
#else  
#pragma comment(lib, "PlotDll.lib")  
#endif*/

LRESULT CALLBACK WindowProc(HWND,UINT,WPARAM,LPARAM);
char FileName[MAX_PATH] = { 0 };
void DrawPicture(LPCSTR FN)
{
	strcpy(FileName, FN);
	HWND hwnd;
	MSG msg;
	ZeroMemory(&msg,sizeof(msg));
	HINSTANCE hInstance = (HINSTANCE)GetConsoleWindow();
	TCHAR ClassName[] = {TEXT("ClassWindowWAVEProjectCreated")};
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = ClassName;
	wndclass.lpszMenuName = NULL;
	RegisterClass(&wndclass);
	hwnd = CreateWindow(ClassName, TEXT("WAVE Show Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 600, NULL, NULL, hInstance, NULL);
	if(hwnd)
		ShowWindow(hwnd, SW_SHOW);
	else
	{
		UnregisterClass(ClassName, hInstance);
		DEBUG::err("CreateWindow() error !");
	}
	UpdateWindow(hwnd);
	SetWindowLong(hwnd,GWL_STYLE,GetWindowLong(hwnd,GWL_STYLE)&~WS_MINIMIZEBOX);
	SetWindowLong(hwnd,GWL_STYLE,GetWindowLong(hwnd,GWL_STYLE)&~WS_MAXIMIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE)&~WS_THICKFRAME);
	DEBUG::info("The window was be created !");
	while (GetMessage(&msg,hwnd,0,0)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(ClassName, hInstance);
	DEBUG::info("The window was be destroyed !");
}
inline int RectHeight(RECT rect)
{
	return (rect.bottom - rect.top);
}
inline int RectWidth(RECT rect)
{
	return (rect.right - rect.left);
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	RECT rect;
	HDC hdc;
	HPEN gpen, pen;
	int x, y, y1, y2;
	int min, max, Lmin, Lmax, Rmin, Rmax;
	float ABP = pow(2, Sound->BitsPerSample - 1);
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		if (!GetClientRect(hwnd, &rect))DEBUG::err("GetClientRect() failed !");
		DEBUG::info("GetClientRect() OK !");
		hdc = BeginPaint(hwnd, &ps);
		if (hdc == NULL)DEBUG::err("BeginPaint() failed !");
		DEBUG::info("BeginPaint() OK !");
		gpen = CreatePen(PS_SOLID, 1, RGB(0, 250, 0));
		if (gpen == NULL)DEBUG::err("CreatePen()*gpen failed !");
		DEBUG::info("CreatePen()*gpen OK !");
		if (Sound->channel == 2)
		{
			x = 0; y = RectHeight(rect) / 2;
			pen = CreatePen(PS_SOLID, 1, RGB(200, 0, 0));
			if (pen == NULL)DEBUG::err("CreatePen()*pen failed !");
			DEBUG::info("CreatePen()*pen OK !");
			SelectObject(hdc, pen);
			MoveToEx(hdc, 0, y, NULL);
			LineTo(hdc, RectWidth(rect), y);
			if (!DeleteObject(pen))DEBUG::err("DeleteObject()*pen failed !");
			DEBUG::info("DeletePen()*pen OK !");
			SelectObject(hdc, gpen);
			DWORD TimeNow;
			sndPlaySoundA(FileName, SND_ASYNC);
			for (int lp = 1; lp <= floor(Sound->TimeLast); lp++)
			{
				TimeNow = GetTickCount();
				x = 0;
				while (x <= RectWidth(rect))
				{
					min = INT_MAX, max = INT_MIN;
					for (int j = (x + (lp - 1) * RectWidth(rect)) *(Fmt->wavFormat.dwSamplesPerSec / RectWidth(rect)); j < (x + 1 + (lp - 1) * RectWidth(rect)) *(Fmt->wavFormat.dwSamplesPerSec / RectWidth(rect)); j++)
					{
						if (Sound->ldata[j] < min)min = Sound->ldata[j];
						if (Sound->ldata[j] >max)max = Sound->ldata[j];
					}
					MoveToEx(hdc, x, y + ((double)max / ABP*(double)RectHeight(rect) / 2.0), NULL);
					LineTo(hdc, x, y + ((double)min / ABP*(double)RectHeight(rect) / 2.0));
					x++;
					while (GetTickCount() - TimeNow < (Fmt->wavFormat.dwSamplesPerSec / RectWidth(rect)))Sleep(10);
				}
				while (GetTickCount() - TimeNow < 1000)Sleep(10);
				HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
				FillRect(hdc, &rect, hBrush);
				DeleteObject(hBrush);
				pen = CreatePen(PS_SOLID, 1, RGB(200, 0, 0));
				SelectObject(hdc, pen);
				MoveToEx(hdc, 0, y, NULL);
				LineTo(hdc, RectWidth(rect), y);
				DeleteObject(pen);
				SelectObject(hdc, gpen);
			}
		}
		else
		{
			x = 0, y = RectHeight(rect) / 2;
			pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			if (pen == NULL)DEBUG::err("CreatePen()*pen failed !");
			DEBUG::info("CreatePen()*pen OK !");
			SelectObject(hdc, pen);
			MoveToEx(hdc, 0, y, NULL);
			LineTo(hdc, RectWidth(rect), y);
			if (!DeleteObject(pen))DEBUG::err("DeleteObject()*pen failed !");
			DEBUG::info("DeletePen()*pen OK !");
			y1 = RectHeight(rect) / 4; y2 = y1 * 3;
			pen = CreatePen(PS_SOLID, 1, RGB(200, 0, 0));
			if (pen == NULL)DEBUG::err("CreatePen()*pen*2 failed !");
			DEBUG::info("CreatePen()*pen*2 OK !");
			SelectObject(hdc, pen);
			MoveToEx(hdc, 0, y1, NULL);
			LineTo(hdc, RectWidth(rect), y1);
			MoveToEx(hdc, 0, y2, NULL);
			LineTo(hdc, RectWidth(rect), y2);
			if (!DeleteObject(pen))DEBUG::err("DeleteObject()*pen*2 failed !");
			DEBUG::info("DeletePen()*pen*2 OK !");
			SelectObject(hdc, gpen);
			while (x < RectWidth(rect))
			{
				Lmin = INT_MAX; Lmax = INT_MIN; Rmin = INT_MAX; Rmax = INT_MIN;
				for (int j = x*Sound->num / RectWidth(rect); j < (x + 1)*Sound->num / RectWidth(rect); j++)
				{
					if (Sound->ldata[j] < Lmin)Lmin = Sound->ldata[j];
					if (Sound->ldata[j] >Lmax)Lmax = Sound->ldata[j];
					if (Sound->rdata[j] < Rmin)Rmin = Sound->rdata[j];
					if (Sound->rdata[j] >Rmax)Rmax = Sound->rdata[j];
				}
				MoveToEx(hdc, x, y1 + (Lmax*RectWidth(rect) / 4.0 / ABP), NULL);
				LineTo(hdc, x, y1 + (Lmin*RectWidth(rect) / 4.0 / ABP));
				MoveToEx(hdc, x, y2 + (Rmax*RectWidth(rect) / 4.0 / ABP), NULL);
				LineTo(hdc, x, y2 + (Rmin*RectWidth(rect) / 4.0 / ABP));
				x++;
			}
		}
		if (!DeleteObject(gpen))DEBUG::err("DeleteObject()*gpen failed !");
		DEBUG::info("DeletePen()*gpen OK !");
		if (!EndPaint(hwnd, &ps))DEBUG::err("EndPaint() failed !");
		DEBUG::info("EndPaint() OK !");
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
