#include <windows.h>

#define WINDOW_WIDTH 800
#define  WINDOW_HEIGHT 600
#define  WINDOW_TITLE L"����Ϸ����"

//����ȫ�ֱ���
HDC g_hdc = NULL;//ȫ���豸��������

//��������

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);//���ڹ��̺���
BOOL Game_Init(HWND hwnd);//��Ϸ��Դ��ʼ��
VOID Game_Paint(HWND hwnd);//��Ϸ��ͼ����
BOOL Game_CleanUp(HWND hwnd);//��Ϸ��Դ����


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	//��ƴ�����
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
	  
	//ע�ᴰ����
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	//����������
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

	//���ڵ��ƶ�
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);

	//���ڵ���ʾ
	ShowWindow(hwnd,nShowCmd);

	//���ڵĸ���
	UpdateWindow(hwnd);

	//��Ϸ��Դ��ʼ��
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd,L"��Դ��ʼ��ʧ��",L"��Ϣ����",0);
		return FALSE;
	}

	//��Ϣѭ������
	MSG msg ={0};

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//���ڵ�ע��
	UnregisterClass(L"ForTheDreamOfGameDevelop",wndClass.hInstance);

	return 0;

}

//���ڹ��̺���
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT paintStruct;

	switch (message)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd,&paintStruct);
		Game_Paint(hwnd);
		EndPaint(hwnd,NULL);
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
	HFONT hFont = CreateFont(30,0,0,0,0,0,0,0,GB2312_CHARSET,0,0,0,0,L"΢���ź�");
	SelectObject(g_hdc,hFont);
	SetBkMode(g_hdc,TRANSPARENT);

	wchar_t text1[] = L"���뻹��Ҫ�еģ���һʵ�����أ�";
	wchar_t text2[] = L"������������";

	SetTextColor(g_hdc,RGB(50,255,50));
	TextOut(g_hdc,30,150,text1,wcslen(text1));

	SetTextColor(g_hdc,RGB(50,50,255));
	TextOut(g_hdc,500,200,text2,wcslen(text2));

	DeleteObject(hFont);
}

BOOL Game_CleanUp(HWND hwnd)
{
	return TRUE;  
}

