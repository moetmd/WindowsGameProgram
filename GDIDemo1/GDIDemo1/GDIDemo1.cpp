#include <windows.h>
#include <time.h>
#pragma comment(lib,"winmm.lib")

#define WINDOW_WIDTH 800
#define  WINDOW_HEIGHT 600
#define  WINDOW_TITLE L"����Ϸ����"

//����ȫ�ֱ���
HDC g_hdc = NULL;//ȫ���豸��������
HPEN g_hPen[7] = {0};//���ʾ������
HBRUSH g_hBrush[7] = {0};//��ˢ�������
int g_iPenStyle[7] = {PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,
	PS_DASHDOTDOT,PS_NULL,PS_INSIDEFRAME};//���廭����ʽ����ʼ��
int g_iBrushStyle[7] = {HS_VERTICAL,HS_HORIZONTAL,HS_CROSS,HS_DIAGCROSS,
	HS_FDIAGONAL,HS_BDIAGONAL};//���廭ˢ��ʽ����ʼ��


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

	PlaySound(L"1.wav",NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);

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

