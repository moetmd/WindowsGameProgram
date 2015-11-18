#include "DirectX.h"
using namespace std;

bool gameover = false;

//windows event handling function
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//set the windows properties
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE;
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	//determine the resolution of the clients desktop screen
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//place the window in the middle of screen
	int posX = (GetSystemMetrics(SM_CXSCREEN) - SCREENW) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - SCREENH) / 2;

	//Create a window
	HWND window;
	window = CreateWindow(APPTITLE, APPTITLE,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, posX, posY, SCREENW, SCREENH,
		NULL, NULL, hInstance, NULL);
	if (window == 0)
		return false;

	//display the window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	//initialize the game
	if (!Game_Init(window))
		return 0;

	//main message loop
	MSG msg;
	while (!gameover)
	{
		//process windows events
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//handle any event messages
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//process game loop
		Game_Run(window);
	}

	//free game resources
	Game_End();

	return msg.wParam;
}