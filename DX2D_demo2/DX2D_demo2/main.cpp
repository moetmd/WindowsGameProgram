#include "DirectX.h"
using namespace std;

bool gameover = false;

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, 
					int nShowCmd)
{
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

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = (GetSystemMetrics(SM_CXSCREEN) - SCREENW) / 2;
	int posy = (GetSystemMetrics(SM_CYSCREEN) - SCREENW) / 2;

	HWND window;

	window = CreateWindow(APPTITLE, APPTITLE,
		WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,posX,posy,SCREENW,SCREENH,
		NULL,NULL,hInstance,NULL
		);
	
	if (window == 0)
		return false;

	ShowWindow(window, nShowCmd);
	UpdateWindow(window);

	if (!Game_Init(window))
		return 0;

	MSG msg;

	while (!gameover)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Game_Run(window);
	}

	Game_End();

	return msg.wParam;

}
