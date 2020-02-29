/*
*	Problem I.F.Me
*
*	by Yonghang Jiang
*
*	source code: IFMeMain.cpp
*
*	2006-8-27
*/
#pragma comment(lib, "winmm")
#pragma comment(lib, "comctl32.lib")
#include <math.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <wtypes.h>
#include <windowsx.h>
#include <winbase.h>
#include <windows.h>
#include <winuser.h>
#include <tchar.h>
#include <psapi.h>
#include <time.h>
#include <dos.h>
#include <conio.h>
#include "Function.h"
#include "resource.h"
//using namespace std;


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
//窗口消息处理程序	（窗口句柄，消息类型，消息数据1，消息数据2）


SNAKE	GreedySnake; 

int WINAPI WinMain (HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					PSTR		szCmdLine,
					int			iCmdShow)
{
	int	cxClient, cyClient;
	RECT	rectMain;
	POINT	pTemp;



	static		TCHAR szAppName[] = TEXT ("GreedySnake") ;
	HWND		hwnd ;
	MSG			msg ;
	WNDCLASS	wndclass ;
	wndclass.style			= CS_HREDRAW | CS_VREDRAW ;

	wndclass.lpfnWndProc	= WndProc ;
	wndclass.cbClsExtra		= 0 ;
	wndclass.cbWndExtra		= 0 ;
	wndclass.hInstance		= hInstance ;
	wndclass.hIcon			= LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor		= LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName	= NULL ;
	wndclass.lpszClassName	= szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL,
			TEXT ("This program requires Windows NT!"),
			szAppName,
			MB_ICONERROR) ;
		return 0 ;
	}

	hwnd	= CreateWindow(	szAppName,					// window class name
		TEXT ("GreedySnake"),				// window caption
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_CAPTION,		// window style
		0,				// initial x position
		0,				// initial y position
		CW_USEDEFAULT,				// initial x size
		CW_USEDEFAULT,				// initial y size
		NULL,						// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL) ;						// creation parameters
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	GetClientRect(hwnd, &rectMain);
	cxClient	= rectMain.right - rectMain.left;
	cyClient	= rectMain.bottom- rectMain.top;
	pTemp.x		= cxClient/3;
	pTemp.y		= cyClient/3;
	GreedySnake	= SNAKE(SNAKE_INILEN, 2, pTemp);
	//	while (GetMessage (&msg, NULL, 0, 0))
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static	FOOD	Food;

	static	HDC		hDC, hMemDC;
	RECT			rectMain;
	static	HBITMAP	hBitmap, hOldBitmap, hFoodBitmap;


	PAINTSTRUCT		ps ;
	POINT			pTemp;

	static	HINSTANCE	hInstance ;
	static	RECT	rect ;


	static	int		cxChar, cyChar;
	static	int		cxClient, cyClient;	//窗口宽度和高度
	static	BOOL	Pause;
	int	i, j;

	RECT	rectText;
	static	RECT	hrect;

	TCHAR	szStr[128];


	switch (message)
	{
	case WM_CREATE:
		SendMessage(hwnd,
			WM_SETICON,
			(WPARAM)TRUE,
			(LPARAM)LoadIcon(GetModuleHandle(NULL),
			(LPCTSTR)IDI_ICON1));
		MessageBox(hwnd, "GreedySnake v0.6.0，冰火梦幻个人作品。", "欢迎试用", MB_OK);
		//initialization
		SetTimer(hwnd, TIMER_SEC, TIME_REFRESH, NULL) ;
		SetTimer(hwnd, TIMER_BGM, TIME_BGM_LEN, NULL) ;
		PlaySound(TEXT("BGM.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
		GetClientRect (hwnd, &rect) ;	//
		cxChar	= LOWORD(GetDialogBaseUnits());
		cyChar	= HIWORD(GetDialogBaseUnits());
		Food.pPosition.x	= rand() % (rect.right-rect.left);
		Food.pPosition.y	= rand() % (rect.bottom-rect.top);
		Food.iMark			= FOOD_MARK;
		Food.Eated			= false;
		Pause				= FALSE;
		hFoodBitmap			= (HBITMAP)LoadImage(NULL, "Food.bmp", IMAGE_BITMAP,  0, 0,	LR_DEFAULTCOLOR | LR_LOADFROMFILE);
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_BGM:
			break;
		case TIMER_SEC:
			if (FALSE == Pause)
			{
				hDC		= GetDC(hwnd);
				hMemDC	= CreateCompatibleDC(hDC);
				GreedySnake.MoveSnake(cxClient, cyClient);
				GetClientRect(hwnd, &rectMain);
				hBitmap	= CreateCompatibleBitmap(hDC, rectMain.right, rectMain.bottom);
			//	hBitmap	= (HBITMAP)LoadImage(NULL, "BackGround.bmp", IMAGE_BITMAP, 0, 0,	LR_DEFAULTCOLOR | LR_LOADFROMFILE);
				hOldBitmap	= (HBITMAP)SelectObject(hMemDC, hBitmap);

				//text
				pTemp.x= 10;
				pTemp.y= 20;
				ShowHeadCoordinate(hMemDC, pTemp, GreedySnake.ssHead);
				pTemp.y+= 20;
				ShowCurrentMark(hMemDC, pTemp, GreedySnake.iMarks);

				//////food and mark
				if (pow((double)(GreedySnake.ssHead.x-Food.pPosition.x),2.0) +
					pow((double)(GreedySnake.ssHead.y-Food.pPosition.y),2.0) < EFFICIENT_EAT*EFFICIENT_EAT)
				{
					//			PlaySound(TEXT("Ate.wav"), NULL, SND_FILENAME | SND_ASYNC);
					GreedySnake.iMarks+=Food.iMark;
				//	GreedySnake.iLen++;
					Food.pPosition.x	= rand()%(rectMain.right-rectMain.left);
					Food.pPosition.y	= rand()%(rectMain.bottom-rectMain.top);
				}
				GreedySnake.iMarks--;
				//////Draw
				Food.DrawFood(hMemDC);
				/*DrawState(hMemDC, NULL, NULL, (LPARAM)hFoodBitmap, 0,
					Food.pPosition.x-EFFICIENT_EAT/2,
					Food.pPosition.y-EFFICIENT_EAT/2,
					Food.pPosition.x+EFFICIENT_EAT/2,
					Food.pPosition.y+EFFICIENT_EAT/2,
					DSS_NORMAL);*/
				GreedySnake.DrawSnake(hMemDC);

				//////Draw over
				BitBlt(hDC,	0,	0,	cxClient,	cyClient,
					hMemDC,	0, 0, SRCCOPY);

				SelectObject(hMemDC, hOldBitmap);
				DeleteDC(hMemDC);
				ReleaseDC(hwnd, hDC);
				DeleteObject(hBitmap);
			}
			break;
		}
		return 0;
	case	WM_KEYDOWN:
		switch (wParam)
		{
		case	VK_LEFT:
			GreedySnake.iDirection	= (GreedySnake.iDirection+3)&3;
			break;
		case	VK_RIGHT:
			GreedySnake.iDirection	= (GreedySnake.iDirection+1)&3;
			break;
		case	'P':
			Pause	= !Pause;
			break;
		case	VK_ESCAPE:
			PostQuitMessage (0) ;
			break;
		}
		return 0;
	case   WM_COMMAND :
		return 0;
	case	WM_LBUTTONUP:
		GreedySnake.iDirection	= (GreedySnake.iDirection+3)&3;
		return 0;
	case	WM_RBUTTONUP:
		GreedySnake.iDirection	= (GreedySnake.iDirection+1)&3;
		return 0;
	case	WM_SIZE:
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;
		rect.right	= rect.left + cxClient;
		rect.bottom	= rect.top + cyClient;
		return 0 ;
	case   WM_PAINT:
		hDC = BeginPaint (hwnd, &ps) ;
		EndPaint (hwnd, &ps) ;
		return 0 ;
	case   WM_DESTROY:
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
