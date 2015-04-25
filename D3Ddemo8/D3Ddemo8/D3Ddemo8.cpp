//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
#include <d3d9.h>
#include <windows.h>
#include <tchar.h>
#include <D3dx9core.h>
#include <dinput.h>
#include "D3DUtil.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	L"D3Ddemo8"		//Ϊ���ڱ��ⶨ��ĺ�
#define SAFE_RELEASE(p){if (p){(p)->Release();(p)=NULL;}} //���尲ȫ�ͷź�


//-----------------------------------��ȫ�ֽṹ�塿--------------------------------------------

//���㻺��ʹ��1����ƶ����ʽ
struct CUSTOMVERTEX
{
	FLOAT x,y,z;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE) //FVF�����ʽ



//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );  //���ڹ��̺���
HRESULT Direct3D_Init(HWND hwnd);
HRESULT Objects_Init(HWND hwnd);
VOID Direct3D_Render(HWND hwnd);
VOID Direct3D_CleanUp();
float Get_FPS();
VOID Matrix_Set();
VOID Light_Set(LPDIRECT3DDEVICE9 pd3dDevice, UINT nType);

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; //Direct3D�豸����
ID3DXFont* g_pFont = NULL; //����com�ӿ�
float g_FPS = 0.0f;
wchar_t g_strFPS[50];

LPD3DXMESH g_teapot = NULL;
LPD3DXMESH g_cube = NULL;
LPD3DXMESH g_sphere = NULL;
LPD3DXMESH g_torus = NULL;
D3DXMATRIX g_WorldMatrix[4],R;

LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; //���㻺��������
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL; //�����������

LPDIRECTINPUTDEVICE8 g_pKeyboardDevice = NULL;
char g_pKeyStateBuffer[256] = {0};

//-----------------------------------��WinMain( )������--------------------------------------
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
	wndClass.lpszClassName = L"D3Ddemo8";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow( L"D3Ddemo8",WINDOW_TITLE,		//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );


	if (!(S_OK == Direct3D_Init(hwnd)))
	{
		MessageBox(hwnd,L"��ʼ��ʧ��",L"��ʾ",0);
	}


	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow( hwnd, nShowCmd );    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//PlaySound(L"",NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
	//MessageBox(hwnd,L"",L"",0);

	//��5����Ϣѭ������
	MSG msg = { 0 };		//���岢��ʼ��msg
	while( msg.message != WM_QUIT )			//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage( &msg );			//�ַ�һ����Ϣ�����ڳ���
		}else
		{
			Direct3D_Render(hwnd);
		}
	}

	//��6���������ע��
	UnregisterClass(L"D3Ddemo8", wndClass.hInstance);  //����׼��������ע��������
	return 0;  
}


//-----------------------------------��WndProc( )������--------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{
	switch( message )						//switch��俪ʼ
	{
	case WM_PAINT:						// ���ǿͻ����ػ���Ϣ
		Direct3D_Render(hwnd);
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;									//������switch���

	case WM_KEYDOWN:                // ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;									//������switch���

	case WM_DESTROY:				//���Ǵ���������Ϣ
		PostQuitMessage( 0 );		//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;								//������switch���

	default:									//������case�����������ϣ���ִ�и�default���
		return DefWindowProc( hwnd, message, wParam, lParam );		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;			//�����˳�
}

//Direct3D_Init
//Direct3D�ĳ�ʼ��
HRESULT Direct3D_Init(HWND hwnd)
{
	//1������Direct3D�ӿڶ���
	LPDIRECT3D9 pD3D = NULL;
	if(NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))	//��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return S_FALSE;

	//2����ȡӲ���豸��Ϣ
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps)))
	{
		return E_FAIL;
	}
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//3�����D3DPRESENT_PARAMETERS�ṹ��
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	//4������Direct3D�豸�ӿ�
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,vp,&d3dpp,&g_pd3dDevice)))
		return E_FAIL;

	SAFE_RELEASE(pD3D)

		if(!(S_OK == Objects_Init(hwnd)))
			return E_FAIL;



	return S_OK;
}


//Object_Init
//��Ⱦ��Դ��ʼ��
HRESULT Objects_Init(HWND hwnd)
{
	if(FAILED(D3DXCreateFont(g_pd3dDevice,36,0,0,1,FALSE,DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,0,_T("΢���ź�"),&g_pFont)))
		return E_FAIL;

	srand(timeGetTime());

	if(FAILED(D3DXCreateBox(g_pd3dDevice,2,2,2,&g_cube,NULL)))
		return false;
	if(FAILED(D3DXCreateTeapot(g_pd3dDevice,&g_teapot,NULL)))
		return false;
	if(FAILED(D3DXCreateSphere(g_pd3dDevice,1.5,25,25,&g_sphere,NULL)))
		return false;
	if(FAILED(D3DXCreateTorus(g_pd3dDevice,0.5f,1.2f,25,25,&g_torus,NULL)))
		return false;

	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl,sizeof(mtrl));
	mtrl.Ambient = D3DXCOLOR(0.5f,0.5f,0.7f,1.0f);
	mtrl.Diffuse = D3DXCOLOR(0.6f,0.6f,0.6f,1.0f);
	mtrl.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,0.3f);
	mtrl.Emissive = D3DXCOLOR(0.3f,0.0f,0.1f,1.0f);
	g_pd3dDevice->SetMaterial(&mtrl);

	Light_Set(g_pd3dDevice,1);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	//g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,false);


	return S_OK;
}


//Direct3D_Render
//ʹ��Direct3D������Ⱦ
VOID Direct3D_Render(HWND hwnd)
{
	//1������
	g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);

	RECT formatRect;
	GetClientRect(hwnd,&formatRect);

	//2����ʼ����
	g_pd3dDevice->BeginScene();


	//3����ʽ����

	//���㻺������������ʹ��4������ͼ��
	Matrix_Set();

	if (::GetAsyncKeyState(0x31) & 0x8000f)
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	if (::GetAsyncKeyState(0x32) & 0x8000f)
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

	if (::GetAsyncKeyState(0x51) & 0x8000f)
		Light_Set(g_pd3dDevice,1);
	if (::GetAsyncKeyState(0x57) & 0x8000f)
		Light_Set(g_pd3dDevice,2);
	if (::GetAsyncKeyState(0x45) & 0x8000f)
		Light_Set(g_pd3dDevice,3);

	D3DXMatrixRotationY(&R,::timeGetTime()/1440.0f);

	D3DXMatrixTranslation(&g_WorldMatrix[0],3.0f,-3.0f,0.0f);
	g_WorldMatrix[0] = g_WorldMatrix[0]*R;
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_WorldMatrix[0]);
	g_cube->DrawSubset(0);

	D3DXMatrixTranslation(&g_WorldMatrix[1],-3.0f,-3.0f,0.0f);
	g_WorldMatrix[1] = g_WorldMatrix[1]*R;
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_WorldMatrix[1]);
	g_teapot->DrawSubset(0);

	D3DXMatrixTranslation(&g_WorldMatrix[2],3.0f,3.0f,0.0f);
	g_WorldMatrix[2] = g_WorldMatrix[2]*R;
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_WorldMatrix[2]);
	g_sphere->DrawSubset(0);

	D3DXMatrixTranslation(&g_WorldMatrix[3],-3.0f,3.0f,0.0f);
	g_WorldMatrix[3] = g_WorldMatrix[3]*R;
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_WorldMatrix[3]);
	g_torus->DrawSubset(0);

	//��ʾfps
	int charCount = swprintf_s(g_strFPS,20,_T("FPS:%0.3f"),Get_FPS());
	g_pFont->DrawText(NULL,g_strFPS,charCount,&formatRect,DT_TOP|DT_RIGHT,D3DCOLOR_XRGB(255,30,100));





	//4����������
	g_pd3dDevice->EndScene();

	//5����ת
	g_pd3dDevice->Present(NULL,NULL,NULL,NULL);
}

//Direct3D_CleanUp
//��Դ����
VOID Direct3D_CleanUp()
{
	SAFE_RELEASE(g_torus);
	SAFE_RELEASE(g_sphere);
	SAFE_RELEASE(g_cube);
	SAFE_RELEASE(g_teapot)
		SAFE_RELEASE(g_pIndexBuffer);
	SAFE_RELEASE(g_pVertexBuffer);
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pd3dDevice);
}

//Get_FPS()  ���ڼ���FPS
float Get_FPS()
{
	static float fps = 0;
	static int frameCount = 0;
	static float currentTime = 0.0f;
	static float lastTime = 0.0f;

	frameCount++;
	currentTime = timeGetTime()*0.001f;

	if (currentTime - lastTime > 1.0f)
	{
		fps = (float)frameCount / (currentTime - lastTime);
		lastTime = currentTime;
		frameCount = 0;
	}

	return fps;
}

VOID Matrix_Set()
{
	//1����任
	D3DXMATRIX matWorld, Rx, Ry, Rz;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationX(&Rx,D3DX_PI * (::timeGetTime()/1000.0f));

	D3DXMatrixRotationY(&Rx,D3DX_PI * (::timeGetTime()/1000.0f/2));

	D3DXMatrixRotationZ(&Rx,D3DX_PI * (::timeGetTime()/1000.0f/3));

	matWorld = Rx * Ry * Rz * matWorld;
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matWorld);


	//2ȡ���任
	D3DXMATRIX matView;
	D3DXVECTOR3 vEye(0.0f, 0.0f, -15.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);


	//3ͶӰ�任
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4.0f, 1.0f, 1.0f, 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);


	//4�ӿڱ任
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	g_pd3dDevice->SetViewport(&vp);

}

//���ù�Դ����
VOID Light_Set(LPDIRECT3DDEVICE9 pd3dDevice, UINT nTpye)
{
	static D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));

	switch (nTpye)
	{
	case 1:
		light.Type = D3DLIGHT_POINT;
		light.Ambient = D3DXCOLOR(0.6f,0.6f,0.6f,1.0f);
		light.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		light.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
		light.Position = D3DXVECTOR3(0.0f,200.0f,0.0f);
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Range = 300.0f;
		break;

	case 2:
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Ambient = D3DXCOLOR(0.6f,0.6f,0.6f,1.0f);
		light.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		light.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
		light.Direction = D3DXVECTOR3(1.0f,0.0f,0.0f);
		break;

	case 3:
		light.Type = D3DLIGHT_SPOT;
		light.Ambient = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
		light.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		light.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
		light.Position = D3DXVECTOR3(100.0f,100.0f,100.0f);
		light.Direction = D3DXVECTOR3(-1.0f,-1.0f,-1.0f);
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0.0f;
		light.Attenuation2 = 0.0f;
		light.Falloff = 0.1f;
		light.Phi = D3DX_PI/3.0f;
		light.Theta = D3DX_PI/6.0f;
		light.Range = 300.0f;
		break;
	default:
		break;
	}

	g_pd3dDevice->SetLight(0,&light);
	g_pd3dDevice->LightEnable(0,TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB(36,36,36));
}