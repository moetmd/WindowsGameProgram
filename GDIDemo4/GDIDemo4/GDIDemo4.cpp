#include <Windows.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDO��_TITLE L"GDIDEMO4"

HDC g_hdc = NULL, g_mdc = NULL;//ȫ���豸�������
HBITMAP g_hBitmap = NULL;


LRESULT CALLBACK WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);//���ڹ��̺���
BOOL Game_Init(HWND hwnd);//��Դ��ʼ��
VOID Game_Paint(HWND hwnd);//��ͼ����
BOOL Game_CleanUp(HWND hwnd);//��Դ����


//winmain����
int WINAPI WinMain( _In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	//1��ƴ�����
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

	//2ע�ᴰ����
    if(!RegisterClassEx(&wndClass))return -1;

	//3��������
	HWND hwnd = CreateWindow(L"GDIDEMO4",WINDO��_TITLE,
		WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,WINDOW_WIDTH,WINDOW_HEIGHT,
		NULL,NULL,hInstance,NULL);

	//4���ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);

	ShowWindow(hwnd,nShowCmd);
	UpdateWindow(hwnd);

	//��Դ��ʼ��
	if(!Game_Init(hwnd))
	{
		MessageBox(hwnd,L"��Դ��ʼ��ʧ��",L"��Ϣ����",0);
		return FALSE;
	}

	//5��Ϣѭ������
	MSG msg = {0};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//6�������ע��
	UnregisterClass(L"GDIDEMO4",wndClass.hInstance);
	
	return 0;
}

//WndProc����
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT paintStruct;

	switch (message)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd,&paintStruct);
		Game_Paint(hwnd);
		EndPaint(hwnd,&paintStruct);
		ValidateRect(hwnd,NULL);//���¿ͻ�����ʾ
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

//Game_Init����
BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd);

	//����λͼ
	g_hBitmap = (HBITMAP)LoadImage(NULL,L"1.bmp",IMAGE_BITMAP,800,600,LR_LOADFROMFILE);

	//��������DC
	g_mdc = CreateCompatibleDC(g_hdc);

	Game_Paint(hwnd);
	ReleaseDC(hwnd,g_hdc);
	return TRUE;
}


//Game_Paint����
VOID Game_Paint(HWND hwnd)
{
	//ѡ��λͼ����
	SelectObject(g_mdc,g_hBitmap);

	//������ͼ
	BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_mdc,0,0,SRCCOPY);
}

//Game_CleanUp����
BOOL Game_CleanUp(HWND hwnd)
{
	//�ͷ���Դ
	DeleteObject(g_hBitmap);
	DeleteDC(g_mdc);
	return TRUE;
}

