#include <Windows.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOＷ_TITLE L"GDIDEMO4"

HDC g_hdc = NULL;//全局设备环境句柄

LRESULT CALLBACK WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);//窗口过程函数
BOOL Game_Init(HWND hwnd);//资源初始化
VOID Game_Paint(HWND hwnd);//绘图函数
BOOL Game_CleanUp(HWND hwnd);//资源清理


//winmain函数
int WINAPI WinMain( _In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	//1设计窗口类
	WNDCLASSEX wndClass = {0};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(NULL,L"icon.icon",IMAGE_ICON,
		0,0,LR_DEFAULTCOLOR|LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"GDIDEMO4";

	//2注册窗口类
    if(!RegisterClassEx(&wndClass))return -1;

	//3创建窗口
	HWND hwnd = CreateWindow(L"GDIDEMO4",WINDOＷ_TITLE,
		WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,WINDOW_WIDTH,WINDOW_HEIGHT,
		NULL,NULL,hInstance,NULL);

	//4窗口的移动、显示与更新
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);

	ShowWindow(hwnd,nShowCmd);
	UpdateWindow(hwnd);

	//资源初始化
	if(!Game_Init(hwnd))
	{
		MessageBox(hwnd,L"资源初始化失败",L"消息窗口",0);
		return FALSE;
	}

	//5消息循环过程
	MSG msg = {0};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//6窗口类的注销
	UnregisterClass(L"GDIDEMO4",wndClass.hInstance);
	
	return 0;
}

//WndProc函数
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

}

//Game_Init函数
BOOL Game_Init(HWND hwnd)
{

}


//Game_Paint函数
VOID Game_Paint(HWND hwnd)
{

}

//Game_CleanUp函数
BOOL Game_CleanUp(HWND hwnd)
{

}

