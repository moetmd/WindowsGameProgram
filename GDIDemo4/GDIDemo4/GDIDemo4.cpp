#include <Windows.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOＷ_TITLE L"GDIDEMO4"

HDC g_hdc = NULL, g_mdc = NULL;//全局设备环境句柄
HBITMAP g_hBitmap = NULL;


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
	PAINTSTRUCT paintStruct;

	switch (message)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd,&paintStruct);
		Game_Paint(hwnd);
		EndPaint(hwnd,&paintStruct);
		ValidateRect(hwnd,NULL);//更新客户区显示
		break;
		
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
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

//Game_Init函数
BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd);

	//加载位图
	g_hBitmap = (HBITMAP)LoadImage(NULL,L"1.bmp",IMAGE_BITMAP,800,600,LR_LOADFROMFILE);

	//建立兼容DC
	g_mdc = CreateCompatibleDC(g_hdc);

	Game_Paint(hwnd);
	ReleaseDC(hwnd,g_hdc);
	return TRUE;
}


//Game_Paint函数
VOID Game_Paint(HWND hwnd)
{
	//选用位图对象
	SelectObject(g_mdc,g_hBitmap);

	//进行贴图
	BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_mdc,0,0,SRCCOPY);
}

//Game_CleanUp函数
BOOL Game_CleanUp(HWND hwnd)
{
	//释放资源
	DeleteObject(g_hBitmap);
	DeleteDC(g_mdc);
	return TRUE;
}

