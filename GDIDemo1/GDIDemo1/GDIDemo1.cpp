#include <windows.h>
#include <time.h>
#pragma comment(lib,"winmm.lib")

#define WINDOW_WIDTH 800
#define  WINDOW_HEIGHT 600
#define  WINDOW_TITLE L"致游戏开发"

//定义全局变量
HDC g_hdc = NULL;//全局设备环境变量
HPEN g_hPen[7] = {0};//画笔句柄数组
HBRUSH g_hBrush[7] = {0};//画刷句柄数组
int g_iPenStyle[7] = {PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,
	PS_DASHDOTDOT,PS_NULL,PS_INSIDEFRAME};//定义画笔样式并初始化
int g_iBrushStyle[7] = {HS_VERTICAL,HS_HORIZONTAL,HS_CROSS,HS_DIAGCROSS,
	HS_FDIAGONAL,HS_BDIAGONAL};//定义画刷样式并初始化


//函数声明

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);//窗口过程函数
BOOL Game_Init(HWND hwnd);//游戏资源初始化
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

	PlaySound(L"1.wav",NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);

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
		EndPaint(hwnd, NULL);
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
	srand((unsigned)time(NULL));

	for (int i=0;i<=6;i++)
	{
		g_hPen[i] = CreatePen(g_iPenStyle[i],1,RGB(rand()%256,rand()%256,rand()%256));
		
		if (i == 6)
		{
			g_hBrush[i] = CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));
		} 
		else
		{
			g_hBrush[i] = CreateHatchBrush(g_iBrushStyle[i],RGB(rand()%256,rand()%256,rand()%256));
		}
	}

	Game_Paint(hwnd);
	ReleaseDC(hwnd,g_hdc);
	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	int  y=0;

	for (int i=0;i<=6;i++)
	{
		y=(i+1)*70;

		SelectObject(g_hdc,g_hPen[i]);
		MoveToEx(g_hdc,30,y,NULL);
		LineTo(g_hdc,100,y);
	}

	int x1 = 120;
	int x2 = 190;

	for (int i=0;i<=6;i++)
	{
		SelectObject(g_hdc,g_hBrush[i]);
		Rectangle(g_hdc,x1,70,x2,y);
		x1 += 90;
		x2 += 90;
	}
}

BOOL Game_CleanUp(HWND hwnd)
{
	for(int i=0;i<=6;i++)
	{
		DeleteObject(g_hPen[i]);
		DeleteObject(g_hBrush[i]);
	}
	return TRUE;  
}

