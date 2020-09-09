// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>

#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

UINT A = 1;
UINT n = 0;
UINT t = 1;
INT value;


// buttons
HWND hwndButton;
HWND hText;
HWND hTextEdit;

// sent data
int col = 0;
std::vector<PointF> przyspieszenie;
std::vector<PointF> predkosc;
std::vector<PointF> droga;
RECT drawArea1 = { 0, 0, 900, 199 };
RECT drawArea2 = { 0, 200, 900, 399 };
RECT drawArea3 = { 0, 400, 900, 799 };
RECT drawArea4 = { 0, 0, 900, 799 };


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void MyOnPaint(HDC hdc)
{
	PointF pkt[2];
	pkt[0].X = przyspieszenie[0].X;
	pkt[1].X = przyspieszenie[0].X;
	pkt[0].Y = przyspieszenie[0].Y;
	pkt[1].Y = 0;
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	Pen pen2(Color(255, 25 * col, 0, 255));

	POINT stary_punkt;
	MoveToEx(hdc, 0, 100, &stary_punkt);

	for (int i = 1; i < przyspieszenie.size() / 2; i += t)
	{
		pkt[1].X++;
		for (int j = 0; j < t; j++)
		{
			pkt[1].Y += przyspieszenie[i - 1 + j].Y / t;
		}
		graphics.DrawLine(&pen2, pkt[0].X, 100 - A * pkt[0].Y, pkt[1].X, 100 - A * pkt[1].Y);
		pkt[0].X = pkt[1].X;
		pkt[0].Y = pkt[1].Y;
		pkt[1].Y = 0;

	}
	pkt[0].X = predkosc[0].X;
	pkt[1].X = predkosc[0].X;
	pkt[0].Y = predkosc[0].Y;
	pkt[1].Y = 0;
	for (int i = 1; i < predkosc.size() / 2 + n; i += t)
	{
		pkt[1].X++;
		for (int j = 0; j < t; j++)
		{
			pkt[1].Y += predkosc[i - 1 + j].Y / t;
		}
		graphics.DrawLine(&pen2, pkt[0].X, 300 - A * pkt[0].Y, pkt[1].X, 300 - A * pkt[1].Y);
		pkt[0].X = pkt[1].X;
		pkt[0].Y = pkt[1].Y;
		pkt[1].Y = 0;
	}
	pkt[0].X = droga[0].X;
	pkt[1].X = droga[0].X;
	pkt[0].Y = droga[0].Y;
	pkt[1].Y = 0;
	for (int i = 1; i < droga.size() / 2 + n; i += t)
	{
		pkt[1].X++;
		for (int j = 0; j < t; j++)
		{
			pkt[1].Y += droga[i - 1 + j].Y / t;
		}
		graphics.DrawLine(&pen2, pkt[0].X, 700 - A * pkt[0].Y, pkt[1].X, 700 - A * pkt[1].Y);
		pkt[0].X = pkt[1].X;
		pkt[0].Y = pkt[1].Y;
		pkt[1].Y = 0;
	}

	//graphics.DrawRectangle(&pen, 50 + value, 400, 10, 20);
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}

void inputData()
{
	double skl_stala = 0;
	double v = 0;
	double s = 0;
	//data.push_back(Point(0, 100));
	std::fstream plik;
	std::string liczba;
	plik.open("outputRobotForwardB03.log", std::ios::in);
	for (int i = 0; i < 1800; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			plik >> liczba;
			if (i >= n)
			{
				if (j % 12 == 3)
				{
					przyspieszenie.push_back(PointF(i - n, atof(liczba.c_str()) * 9.81));
					skl_stala += atof(liczba.c_str()) / (1800 - n);
				}
			}
		}
	}
	for (int i = 0; i < 1800 - n; i++)
	{
		przyspieszenie[i].Y -= skl_stala;
		v += przyspieszenie[i].Y * 0.04;
		predkosc.push_back(PointF(i, v));
		if (predkosc[i].Y < 0)
			s -= predkosc[i].Y * 0.04;
		else s += predkosc[i].Y * 0.04;
		droga.push_back(PointF(i, s));
	}

	plik.close();

}

void TakeFromEdit()
{
	DWORD dlugosc = GetWindowTextLength(hTextEdit);
	LPWSTR Bufor = (LPWSTR)GlobalAlloc(GPTR, dlugosc + 1);
	GetWindowText(hTextEdit, Bufor, dlugosc + 1);
	n = _wtoi(Bufor);
}


int OnCreate(HWND window)
{
	inputData();
	return 0;
}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 1100, 800, NULL, NULL, hInstance, NULL);

	// create button and store the handle                                                       

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Rysuj przyspieszenie"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE,  // the styles
		1000, 80,                                  // the left and top co-ordinates
		80, 70,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("Rysuj prêdkoœæ"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE,
		1000, 160,
		80, 70,
		hWnd,
		(HMENU)ID_BUTTON2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("Rysuj drogê"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE,
		1000, 240,
		80, 70,
		hWnd,
		(HMENU)ID_BUTTON3,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Rysuj wszystkie wykresy"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE,  // the styles
		1000, 0,                                  // the left and top co-ordinates
		80, 70,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON4,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	//hText = CreateWindow(L"STATIC", L"Ile odrzuciæ?", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
	//	1000, 440, 100, 30, hWnd, (HMENU)ID_STATIC, hInstance, NULL);

	//hTextEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
	//	1000, 470, 100, 30, hWnd, (HMENU)ID_EDIT, hInstance, NULL);

	hText = CreateWindow(L"STATIC", L"Przyspieszenie", WS_CHILD | WS_VISIBLE | SS_CENTER,
		435, 0, 100, 20, hWnd, (HMENU)ID_STATIC, hInstance, NULL);

	hText = CreateWindow(L"STATIC", L"Prêdkoœæ", WS_CHILD | WS_VISIBLE | SS_CENTER,
		435, 200, 100, 20, hWnd, (HMENU)ID_STATIC, hInstance, NULL);

	hText = CreateWindow(L"STATIC", L"Droga", WS_CHILD | WS_VISIBLE | SS_CENTER,
		435, 400, 100, 20, hWnd, (HMENU)ID_STATIC, hInstance, NULL);


	// create button and store the handle                                                       

	hwndButton = CreateWindow(TEXT("button"), TEXT("Amplituda x1"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		950, 320, 115, 30, hWnd, (HMENU)ID_RBUTTON1, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Amplituda x2"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		950, 360, 115, 30, hWnd, (HMENU)ID_RBUTTON2, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Amplituda x3"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		950, 400, 115, 30, hWnd, (HMENU)ID_RBUTTON3, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Czas 1/25s"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1100, 320, 115, 30, hWnd, (HMENU)ID_RBUTTON4, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Czas 1/5s"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1100, 360, 115, 30, hWnd, (HMENU)ID_RBUTTON5, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Czas 1s"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1100, 400, 115, 30, hWnd, (HMENU)ID_RBUTTON6, GetModuleHandle(NULL), NULL);

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1:
			col++;
			if (col > 10)
				col = 0;
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON2:
			col++;
			if (col > 10)
				col = 0;
			repaintWindow(hWnd, hdc, ps, &drawArea2);
			break;
		case ID_BUTTON3:
			col++;
			if (col > 10)
				col = 0;
			repaintWindow(hWnd, hdc, ps, &drawArea3);
			break;
		case ID_BUTTON4:
			repaintWindow(hWnd, hdc, ps, &drawArea4);
			break;
			//case ID_EDIT:
			//	TakeFromEdit();
			//	break;
		case ID_RBUTTON1:
			A = 1;
			break;
		case ID_RBUTTON2:
			A = 2;
			break;
		case ID_RBUTTON3:
			A = 3;
			break;
		case ID_RBUTTON4:
			t = 1;
			break;
		case ID_RBUTTON5:
			t = 5;
			break;
		case ID_RBUTTON6:
			t = 25;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		/*case WM_TIMER:
			switch (wParam)
			{
			case TMR_1:
				//force window to repaint
				repaintWindow(hWnd, hdc, ps, &drawArea5);
				value++;
				break;
			}*/

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
