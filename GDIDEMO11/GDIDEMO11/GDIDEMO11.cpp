
//-----------------------------------������˵����----------------------------------------------
//  ��������:��GDIDEMO5_1 ��Ϸ��ͼ����
//------------------------------------------------------------------------------------------------

//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <time.h>
#pragma  comment(lib,"Msimg32.lib");


//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800						//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	 600						//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE 	L"GDIDEMO11"	//Ϊ���ڱ��ⶨ��ĺ�

struct SwordBullets
{
	int x,y;
	bool exist;
};

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
HDC		g_hdc=NULL,g_mdc=NULL,g_bufdc=NULL;       //ȫ���豸�������
HBITMAP g_hSwordMan=NULL,g_hSwordBlade=NULL,g_hBackGround=NULL;
DWORD g_tPre = 0,g_tNow = 0;
int g_iX=0,g_iY=0,g_iXnow=0,g_iYnow=0;
int g_iBGOffset=0,g_iBulletNum=0;
SwordBullets Bullet[30];



//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );//���ڹ��̺���
BOOL						Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID							Game_Paint( HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL						Game_CleanUp(HWND hwnd );	//�ڴ˺����н�����Դ������

//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra		= 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra		= 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //ָ��������Ĺ������
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"GDIDEMO11";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow( L"GDIDEMO11",WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd,80,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//����������ʾʱ��λ��
	ShowWindow( hwnd, nShowCmd );    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init (hwnd)) 
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����
		return FALSE;
	}

	//��5����Ϣѭ������
	MSG msg = { 0 };				//���岢��ʼ��msg
	while( msg.message != WM_QUIT )		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage( &msg );			//�ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre >= 30)
			{
				Game_Paint(hwnd);
			}
		}
	}

	//��6���������ע��
	UnregisterClass(L"GDIDEMO11", wndClass.hInstance);  //����׼��������ע��������
	return 0;  
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{
	PAINTSTRUCT paintStruct;			//����һ��PAINTSTRUCT�ṹ������¼һЩ������Ϣ

	switch( message )						//switch��俪ʼ
	{
	case WM_TIMER:
		Game_Paint(hwnd);
		break;

	case WM_PAINT:						// ���ǿͻ����ػ���Ϣ
		g_hdc = BeginPaint( hwnd, &paintStruct );  //ָ�����ڽ��л�ͼ������׼�������ý��ͻ�ͼ�йص���Ϣ��䵽paintStruct�ṹ���С�
		Game_Paint( hwnd);
		EndPaint( hwnd, &paintStruct );			//EndPaint�������ָ�����ڵĻ滭���̽���
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;									//������switch���

	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		
		}
		break;									//������switch���

	case WM_LBUTTONDOWN:
		for (int i = 0;i<30;i++)
		{
			if(!Bullet[i].exist)
			{
				Bullet[i].x = g_iXnow;
				Bullet[i].y = g_iYnow + 30;
				Bullet[i].exist = true;
				g_iBulletNum++;
				break;
			}
		}

	case WM_MOUSEMOVE:
		g_iX = LOWORD(lParam);
		if (g_iX>WINDOW_WIDTH)
		{
			g_iX=WINDOW_WIDTH-317;
		}
		else
		{
			if(g_iX<0)
				g_iX=0;
		}
		
		g_iY = HIWORD(lParam);
		if(g_iY > WINDOW_HEIGHT)
		{
			g_iY = WINDOW_HEIGHT-283;
		}else
		{
			if(g_iY < 0)
			g_iY= 0;
		}
		break;


	case WM_DESTROY:					//���Ǵ���������Ϣ
		Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage( 0 );			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc( hwnd, message, wParam, lParam );		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

//-----------------------------------��Game_Init( )������--------------------------------------
//	��������ʼ������������һЩ�򵥵ĳ�ʼ��
//------------------------------------------------------------------------------------------------
BOOL Game_Init( HWND hwnd )
{
	HBITMAP bmp;

	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);

	bmp = CreateCompatibleBitmap(g_hdc,WINDOW_WIDTH,WINDOW_HEIGHT);

	g_iX = 300;
	g_iY = 100;
	g_iXnow = 300;
	g_iYnow = 100;

	SelectObject(g_mdc,bmp);


	g_hBackGround=(HBITMAP)LoadImage(NULL,L"bg.bmp",IMAGE_BITMAP,WINDOW_WIDTH,WINDOW_HEIGHT,LR_LOADFROMFILE);
	g_hSwordMan=(HBITMAP)LoadImage(NULL,L"swordman.bmp",IMAGE_BITMAP,317,283,LR_LOADFROMFILE);
	g_hSwordBlade=(HBITMAP)LoadImage(NULL,L"swordblade.bmp",IMAGE_BITMAP,100,26,LR_LOADFROMFILE);

	POINT pt,lt,rb;
	RECT rect;
	pt.x = 300;
	pt.y = 100;
	ClientToScreen(hwnd,&pt);
	SetCursorPos(pt.x,pt.y);

	ShowCursor(FALSE);

	GetClientRect(hwnd,&rect);
	lt.x=rect.left;
	lt.y=rect.top;
	rb.x=rect.right;
	rb.y=rect.bottom;
	ClientToScreen(hwnd,&lt);
	ClientToScreen(hwnd,&rb);
	rect.left=lt.x;
	rect.top=lt.y;
	rect.right=rb.x;
	rect.bottom=rb.y;

	ClipCursor(&rect);

	Game_Paint(hwnd);
	return TRUE;
}


//-----------------------------------��Game_Paint( )������--------------------------------------
//	���������ƺ������ڴ˺����н��л��Ʋ���
//--------------------------------------------------------------------------------------------------
VOID Game_Paint( HWND hwnd )
{

	//��ͼ
	SelectObject(g_bufdc,g_hBackGround);
	BitBlt(g_mdc,0,0,g_iBGOffset,WINDOW_HEIGHT,g_bufdc,WINDOW_WIDTH-g_iBGOffset,0,SRCCOPY);
	BitBlt(g_mdc,g_iBGOffset,0,WINDOW_WIDTH-g_iBGOffset,WINDOW_HEIGHT,g_bufdc,0,0,SRCCOPY);

	wchar_t str[20] = {};

	if(g_iXnow < g_iX)
	{
		//g_iXnow += 10;
		//if(g_iXnow > g_iX)
			g_iXnow = g_iX;
	}
	else
	{
		//g_iXnow -= 10;
		//if (g_iXnow < g_iX)
			g_iXnow = g_iX;
	}

	if (g_iYnow < g_iY)
	{
		//g_iYnow +=  10;
		//if(g_iYnow > g_iY)
			g_iYnow = g_iY;
	}
	else
	{
		//g_iYnow -= 10;
		//if(g_iYnow<g_iY)
			g_iYnow = g_iY;
	}

	SelectObject(g_bufdc,g_hSwordMan);
	TransparentBlt(g_mdc,g_iXnow,g_iYnow,317,283,g_bufdc,0,0,317,283,RGB(0,0,0));

	SelectObject(g_bufdc,g_hSwordBlade);
	if (g_iBulletNum!=0)
	{
		for (int i = 0; i<30 ; i++)
		{
			if(Bullet[i].exist)
			{
				TransparentBlt(g_mdc,Bullet[i].x-70,Bullet[i].y+100,100,33,g_bufdc,0,0,100,26,RGB(0,0,0));
				Bullet[i].x -=10;
				if(Bullet[i].x < 0)
				{
					g_iBulletNum--;
					Bullet[i].exist = false;
				}
			}
			
		}
	}

	HFONT hFont;
	hFont = CreateFont(20,0,0,0,0,0,0,0,GB2312_CHARSET,0,0,0,0,TEXT("΢���ź�"));
	SelectObject(g_mdc,hFont);
	SetBkMode(g_mdc,TRANSPARENT);
	SetTextColor(g_mdc,RGB(255,255,0));

	swprintf_s(str,L"x:%d",g_iX);
	TextOut(g_mdc,0,0,str,wcslen(str));
	swprintf_s(str,L"y:%d",g_iY);
	TextOut(g_mdc,0,20,str,wcslen(str));

	

	BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_mdc,0,0,SRCCOPY);



	g_tPre = GetTickCount();

	g_iBGOffset +=5;
	if (g_iBGOffset==WINDOW_WIDTH)
	{
		g_iBGOffset = 0;
	}
}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp( HWND hwnd )
{
	KillTimer(hwnd,1);
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd,g_hdc);
	return TRUE;
}