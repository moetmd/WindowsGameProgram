#include <Windows.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDO��_TITLE L"GDIDEMO4"

HDC g_hdc = NULL;//ȫ���豸�������

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

}

//Game_Init����
BOOL Game_Init(HWND hwnd)
{

}


//Game_Paint����
VOID Game_Paint(HWND hwnd)
{

}

//Game_CleanUp����
BOOL Game_CleanUp(HWND hwnd)
{

}

