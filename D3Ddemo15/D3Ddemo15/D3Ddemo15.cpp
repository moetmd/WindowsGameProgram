//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
#include <d3d9.h>
#include <windows.h>
#include <tchar.h>
#include <D3dx9core.h>
#include <dinput.h>
#include "D3DUtil.h"
#include "DirectInputClass.h"

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
#define WINDOW_TITLE	L"D3Ddemo15"		//Ϊ���ڱ��ⶨ��ĺ�
#define SAFE_RELEASE(p){if (p){(p)->Release();(p)=NULL;}} //���尲ȫ�ͷź�


//-----------------------------------��ȫ�ֽṹ�塿--------------------------------------------

//���㻺��ʹ��1����ƶ����ʽ
struct CUSTOMVERTEX
{
	FLOAT _x,_y,_z;
	FLOAT _u,_v;
	CUSTOMVERTEX(FLOAT x,FLOAT y,FLOAT z,FLOAT u,FLOAT v)
		: _x(x),_y(y),_z(z),_u(u),_v(v) {}
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1) //FVF�����ʽ



//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );  //���ڹ��̺���
HRESULT             Direct3D_Init(HWND hwnd);
HRESULT             Objects_Init(HWND hwnd);
VOID                Direct3D_Render(HWND hwnd);
VOID                Direct3D_CleanUp();
VOID                Direct3D_Update(HWND hwnd);
float               Get_FPS();
VOID                Matrix_Set();
VOID                Light_Set(LPDIRECT3DDEVICE9 pd3dDevice, UINT nType);

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
LPDIRECT3DDEVICE9	g_pd3dDevice            = NULL;  //Direct3D�豸����
LPD3DXFONT			g_pTextFPS				= NULL;  //����COM�ӿ�
LPD3DXFONT			g_pTextAdaperName       = NULL;  // �Կ���Ϣ��2D�ı�
LPD3DXFONT			g_pTextHelper           = NULL;  // ������Ϣ��2D�ı�
LPD3DXFONT			g_pTextInfor            = NULL;  // ������Ϣ��2D�ı�
float				g_FPS					= 0.0f;  //һ�������͵ı���������֡����
wchar_t				g_strFPS[50]            = {0};   //����֡���ʵ��ַ�����
wchar_t				g_strAdapterName[60]    = {0};   //�����Կ����Ƶ��ַ�����
D3DXMATRIX			g_matWorld;                      //�������
DInputClass*  	    g_pDInput               = NULL;  //һ��DInputClass���ָ��

LPD3DXMESH          g_pMesh                 = NULL;  // ����Ķ���
D3DMATERIAL9*       g_pMaterials            = NULL;  // ����Ĳ�����Ϣ
LPDIRECT3DTEXTURE9* g_pTextures             = NULL;  // �����������Ϣ
DWORD               g_dwNumMtrls            = 0;     // ���ʵ���Ŀ

LPD3DXMESH          g_pMeshWall             = NULL;  // ǽ���������
D3DMATERIAL9        g_MaterialsWall;                 //ǽ�����

LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer     = NULL;  //���㻺�����
LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer      = NULL;  //�����������
LPDIRECT3DTEXTURE9      g_pTexture          = NULL;  //����ӿڶ���


//-----------------------------------��WinMain( )������--------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass    = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize        = sizeof( WNDCLASSEX ) ;	        //���ýṹ����ֽ�����С
	wndClass.style         = CS_HREDRAW | CS_VREDRAW;       //���ô��ڵ���ʽ
	wndClass.lpfnWndProc   = WndProc;					    //����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra	   = 0;						        //������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra	   = 0;	   						    //���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance     = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon         = (HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,
		                     0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); 
															//���ؼ����Զ���icoͼ��
	wndClass.hCursor       = LoadCursor( NULL, IDC_ARROW );       //ָ��������Ĺ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName  = NULL;						          //��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"D3Ddemo15";		                  //��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if( !RegisterClassEx( &wndClass ) )				        //����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow( L"D3Ddemo15",WINDOW_TITLE,	//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );


	if (!(S_OK == Direct3D_Init(hwnd)))
	{
		MessageBox(hwnd,L"��ʼ��ʧ��",L"��ʾ",0);
	}


	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow( hwnd, nShowCmd );                                   //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						                        //�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//PlaySound(L"",NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
	//MessageBox(hwnd,L"",L"",0);

	//DirectInput��ĳ�ʼ��
	g_pDInput = new DInputClass();
	g_pDInput->Init(hwnd,hInstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);



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
			Direct3D_Update(hwnd);         //���豸���루������꣩������Ӧ
			Direct3D_Render(hwnd);         //��Ⱦ
		}
	}

	//��6���������ע��
	UnregisterClass(L"D3Ddemo15", wndClass.hInstance);  //����׼��������ע��������
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

	//��ȡ�Կ���Ϣ��g_strAdapterName�У������Կ�����֮ǰ���ϡ���ǰ�Կ��ͺţ����ַ���
	wchar_t TempName[60]=L"��ǰ�Կ��ͺţ�";   //����һ����ʱ�ַ������ҷ����˰�"��ǰ�Կ��ͺţ�"�ַ����������ǵ�Ŀ���ַ�����
	D3DADAPTER_IDENTIFIER9 Adapter;  //����һ��D3DADAPTER_IDENTIFIER9�ṹ�壬���ڴ洢�Կ���Ϣ
	pD3D->GetAdapterIdentifier(0,0,&Adapter);//����GetAdapterIdentifier����ȡ�Կ���Ϣ
	int len = MultiByteToWideChar(CP_ACP,0, Adapter.Description, -1, NULL, 0);//�Կ����������Ѿ���Adapter.Description���ˣ�������Ϊchar���ͣ�����Ҫ����תΪwchar_t����
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_strAdapterName, len);//�ⲽ������ɺ�g_strAdapterName�о�Ϊ��ǰ���ǵ��Կ���������wchar_t���ַ�����
	wcscat_s(TempName,g_strAdapterName);//�ѵ�ǰ���ǵ��Կ����ӵ�����ǰ�Կ��ͺţ����ַ������棬�������TempName��
	wcscpy_s(g_strAdapterName,TempName);//��TempName�еĽ��������ȫ�ֱ���g_strAdapterName��


	SAFE_RELEASE(pD3D)
    
	if(!(S_OK == Objects_Init(hwnd)))
		return E_FAIL;
	return S_OK;
}


//Object_Init
//��Ⱦ��Դ��ʼ��
HRESULT Objects_Init(HWND hwnd)
{

	//��������
	D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1000, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);
	D3DXCreateFont(g_pd3dDevice, 20, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"��������", &g_pTextAdaperName); 
	D3DXCreateFont(g_pd3dDevice, 23, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"΢���ź�", &g_pTextHelper); 
	D3DXCreateFont(g_pd3dDevice, 26, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"����", &g_pTextInfor); 

	
	//��x�ļ�������������
	LPD3DXBUFFER pAdjBuffer = NULL;
	LPD3DXBUFFER pMtrlBuffer = NULL;

	D3DXLoadMeshFromX(L"miki.x",D3DXMESH_MANAGED,g_pd3dDevice,&pAdjBuffer,&pMtrlBuffer,
		NULL,&g_dwNumMtrls,&g_pMesh);

	//��ȡ���ʺ���������
	D3DXMATERIAL *pMtrls = (D3DXMATERIAL *)pMtrlBuffer->GetBufferPointer();
	g_pMaterials = new D3DMATERIAL9[g_dwNumMtrls];
	g_pTextures = new LPDIRECT3DTEXTURE9[g_dwNumMtrls];

	for (DWORD i=0;i<g_dwNumMtrls;i++)
	{
		//��ȡ���ʲ����û�������ɫ
		g_pMaterials[i] = pMtrls[i].MatD3D;
		g_pMaterials[i].Ambient = g_pMaterials[i].Diffuse;



		//�����������
		g_pTextures[i] = NULL;
		D3DXCreateTextureFromFileA(g_pd3dDevice,pMtrls[i].pTextureFilename,&g_pTextures[i]);
	}

	SAFE_RELEASE(pAdjBuffer);
	SAFE_RELEASE(pMtrlBuffer);

	//����ǽ��
	D3DXCreateBox(g_pd3dDevice,120.0f,120.0f,0.3f,&g_pMeshWall,NULL);
	g_MaterialsWall.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_MaterialsWall.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_MaterialsWall.Specular = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
	
	//���ù���
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.5f,0.5f,0.5f,1.0f);
	light.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	light.Specular = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	light.Direction = D3DXVECTOR3(1.0f,0.0f,1.0f);
	g_pd3dDevice->SetLight(0,&light);
	g_pd3dDevice->LightEnable(0,true);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS,true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	
	// ������Ⱦ״̬
	//g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS,true);   //��ʼ�����㷨��
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //������������
	//g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); //���û�����
	g_pd3dDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESS);//��Ȳ��Ժ�������
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,true);//��Ȳ��Գɹ�����»���

	//�������Թ���
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,3);
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);

	//�����������
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);

	//������������
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

	//�����������
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAXMIPLEVEL,16);

	//����alpha�ں�
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		//�����ں�����
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		//�����ں����㷽ʽ
	g_pd3dDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);

	Matrix_Set();//�����Ĵ�任
	


	return S_OK;
}


//Direct3D_Render
//ʹ��Direct3D������Ⱦ
VOID Direct3D_Render(HWND hwnd)
{
	//1������
	g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(10,10,10),1.0f,0);

	RECT formatRect;
	GetClientRect(hwnd,&formatRect);

	//2����ʼ����
	g_pd3dDevice->BeginScene();


	//3����ʽ����
	D3DXMATRIX matHero,matWorld,matRotation;
	D3DXMatrixTranslation(&matHero,-20.0f,0.0f,-25.0f);
	matHero = matHero*g_matWorld;
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matHero);


	for (DWORD i=0;i<g_dwNumMtrls;i++)
	{
		g_pd3dDevice->SetMaterial(&g_pMaterials[i]);
		g_pd3dDevice->SetTexture(0,g_pTextures[i]);
		g_pMesh->DrawSubset(i);
	}

	//���ƾ���
	D3DXMatrixTranslation(&matWorld,0.0f,0.0f,0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matWorld);
	g_pd3dDevice->SetMaterial(&g_MaterialsWall);
	g_pMeshWall->DrawSubset(0);

	//����ģ�滺�沢���û���״̬
	g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILREF,0x1);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILMASK,0xffffffff);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);

	//�����ںϣ�����ֹ����Ȼ���д����
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	//������Ϊ���������
	D3DXMatrixTranslation(&matWorld,0.0f,0.0f,0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matWorld); 
	g_pd3dDevice->SetMaterial(&g_MaterialsWall);
	g_pMeshWall->DrawSubset(0);

	//����������Ⱦ״̬���������뾵���ںϣ�����z����
	g_pd3dDevice->Clear(0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);

	//���㾵��任����
	D3DXMATRIX matReflect;
	D3DXPLANE planeXY(0.0f,0.0f,1.0f,0.0f);
	D3DXMatrixReflect(&matReflect,&planeXY);
	matWorld = matReflect * matHero;

	//���ƾ����е�ģ��
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matWorld);
	for (DWORD i=0;i<g_dwNumMtrls;i++)
	{
		g_pd3dDevice->SetMaterial(&g_pMaterials[i]);
		g_pd3dDevice->SetTexture(0,g_pTextures[i]);
		g_pMesh->DrawSubset(i);
	}

	//�ָ���Ⱦ״̬
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE,false);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	//�ڴ������ϽǴ�����ʾÿ��֡��
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));

	//��ʾ�Կ�������
	g_pTextAdaperName->DrawText(NULL,g_strAdapterName, -1, &formatRect, 
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	// ���������Ϣ
	formatRect.top = 30;
	static wchar_t strInfo[256] = {0};
	swprintf_s(strInfo,-1, L"ģ������: (%.2f, %.2f, %.2f)", g_matWorld._41, g_matWorld._42, g_matWorld._43);
	g_pTextHelper->DrawText(NULL, strInfo, -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(135,239,136,255));

	// ���������Ϣ
	formatRect.left = 0,formatRect.top = 380;
	g_pTextInfor->DrawText(NULL, L"����˵��:", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(235,123,230,255));
	formatRect.top += 35;
	g_pTextHelper->DrawText(NULL, L"    ��ס���������϶���ƽ��ģ��", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ��ס����Ҽ����϶�����תģ��", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ���������֣�����ģ��", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    W��S��A��D����ƽ��ģ�� ", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    �ϡ��¡����ҷ��������תģ�� ", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ESC�� : �˳�����", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));



	//4����������
	g_pd3dDevice->EndScene();

	//5����ת
	g_pd3dDevice->Present(NULL,NULL,NULL,NULL);
}

//Direct3D_Update
void Direct3D_Update(HWND hwnd)
{
	g_pDInput->GetInput();

	//��������Ѱַ��ʽ
	if (g_pDInput->IsKeyDown(DIK_1))
	{
		//�ظ�����Ѱַģʽ
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	}
	if (g_pDInput->IsKeyDown(DIK_2))
	{
		//����Ѱַģʽ
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);
	}
	if (g_pDInput->IsKeyDown(DIK_3))
	{
		//��ȡ����Ѱַģʽ
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
	}
	if (g_pDInput->IsKeyDown(DIK_4))
	{
		//�߿�����Ѱַģʽ
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_BORDER);
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_BORDER);
	}

	//ͨ��5��6��������������alpha�����ı仯
	if (g_pDInput->IsKeyDown(DIK_5))
	{
		for (DWORD i=0;i<g_dwNumMtrls;i++)
		{
			g_pMaterials[i].Diffuse.a += 0.001f;
		}
	}
	if (g_pDInput->IsKeyDown(DIK_6))
	{
		for (DWORD i=0;i<g_dwNumMtrls;i++)
		{
			g_pMaterials[i].Diffuse.a -= 0.001f;
		}
	}




	//��ס�������϶���ƽ�Ʋ���
	static FLOAT fPosX = 0.0f, fPosY = 0.0f, fPosZ = 0.0f;

	if (g_pDInput->IsMouseButtonDown(0))
	{
		fPosX += (g_pDInput->MouseDX())*0.08f;
		fPosY += (g_pDInput->MouseDY())*-0.08f;
	}

	//�����֣��ӵ�����
	fPosZ += (g_pDInput->MouseDZ())*0.02f;

	//ƽ������
	if(g_pDInput->IsKeyDown(DIK_A)) fPosX -= 0.005f;
	if(g_pDInput->IsKeyDown(DIK_D)) fPosX += 0.005f;
	if(g_pDInput->IsKeyDown(DIK_W)) fPosY += 0.005f;
	if(g_pDInput->IsKeyDown(DIK_S)) fPosY -= 0.005f;

	D3DXMatrixTranslation(&g_matWorld,fPosX,fPosY,fPosZ);

	//��ס����Ҽ�����ת����
	static float fAngleX = 0.0f, fAngleY = 0.0f;

	if(g_pDInput->IsMouseButtonDown(1))
	{
		fAngleX += (g_pDInput->MouseDY())*-0.001f;
		fAngleY += (g_pDInput->MouseDY())*-0.001f;
	}

	//��ת����
	if(g_pDInput->IsKeyDown(DIK_UP)) fAngleX += 0.005f;
	if(g_pDInput->IsKeyDown(DIK_DOWN)) fAngleX -= 0.005f;
	if(g_pDInput->IsKeyDown(DIK_LEFT)) fAngleY -= 0.005f;
	if(g_pDInput->IsKeyDown(DIK_RIGHT)) fAngleY += 0.005f;


	D3DXMATRIX Rx,Ry;
	D3DXMatrixRotationX(&Rx,fAngleX);
	D3DXMatrixRotationY(&Ry,fAngleY);

	//���վ���
	g_matWorld = Rx * Ry * g_matWorld;//���յľ���
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_matWorld);//�����������

}

//Direct3D_CleanUp
//��Դ����
VOID Direct3D_CleanUp()
{
	//�ͷ�COM�ӿڶ���
	SAFE_RELEASE(g_pTextAdaperName)
	SAFE_RELEASE(g_pTextHelper)
	SAFE_RELEASE(g_pTextInfor)
	SAFE_RELEASE(g_pTextFPS)
	
	SAFE_RELEASE(g_pVertexBuffer)
	SAFE_RELEASE(g_pIndexBuffer)

	SAFE_RELEASE(g_pd3dDevice)
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


	//2ȡ���任
	D3DXMATRIX matView;
	D3DXVECTOR3 vEye(100.0f, 0.0f, -300.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);


	//3ͶӰ�任
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f,
		(float)((double)WINDOW_WIDTH/WINDOW_HEIGHT),1.0f, 10000.0f);
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


