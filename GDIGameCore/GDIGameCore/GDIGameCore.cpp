#include <windows.h>

#define WINDOW_WIDTH 800
#define  WINDOW_HEIGHT 600
#define  WINDOW_TITLE L"致游戏开发"

//定义全局变量
HDC g_hdc = NULL;//全局设备环境变量

//函数声明

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);//窗口过程函数
BOOL Game_Init(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);//游戏资源初始化
VOID Game_Paint(HWND hwnd);//游戏绘图函数
BOOL Game_CleanUp(HWND hwnd);//游戏资源清理


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	//设计窗口类
	WNDCLASSEX wndClass = {0};

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";

	//注册窗口类
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	//创建窗口类
	HWND hwnd = CreateWindow(
		L"ForTheDreamOfGameDevelop",
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	//窗口的移动
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);

	//窗口的显示
	ShowWindow(hwnd,nShowCmd);

	//窗口的更新
	UpdateWindow(hwnd);

	//游戏资源初始化
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd,L"资源初始化失败",L"消息窗口",0);
		return FALSE;
	}

	//消息循环过程
	MSG msg ={0};

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//窗口的注销
	UnregisterClass(L"ForTheDreamOfGameDevelop",wndClass.hInstance);

	return 0;

}

//窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT paintStruct;

	switch (message)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd,&paintStruct);
		Game_Paint(hwnd);
		EndPaint(hwnd);
		ValidateRect(hwnd,NULL);
		break;
	case WM_KEYDOWN:
		if (wParam ==VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
		Game_CleanUp(hwnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}

BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd);
	Game_Paint(hwnd);
	ReleaseDC(hwnd,g_hdc);
	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{

}

BOOL Game_CleanUp(HWND hwnd)
{
	return TRUE;  
}

