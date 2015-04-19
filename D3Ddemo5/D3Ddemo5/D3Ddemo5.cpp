//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
#include <d3d9.h>
#include <windows.h>
#include <tchar.h>
#include<D3dx9core.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	L"D3Ddemo5"		//Ϊ���ڱ��ⶨ��ĺ�
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

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; //Direct3D�豸����
ID3DXFont* g_pFont = NULL; //����com�ӿ�
float g_FPS = 0.0f;
wchar_t g_strFPS[50];
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; //���㻺��������
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL; //�����������



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
	wndClass.lpszClassName = L"D3Ddemo5";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow( L"D3Ddemo5",WINDOW_TITLE,		//ϲ���ּ��Ĵ������ں���CreateWindow
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
	UnregisterClass(L"D3Ddemo5", wndClass.hInstance);  //����׼��������ע��������
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

	//���㻺�桢��������2���������㻺�����������
	//�������㻺��
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(8*sizeof(CUSTOMVERTEX),0,D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,&g_pVertexBuffer,NULL)))
	{
		return E_FAIL;
	}

	//��������
	if(FAILED(g_pd3dDevice->CreateIndexBuffer(36*sizeof(WORD),0,D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,&g_pIndexBuffer,NULL)))
	{
		return E_FAIL;
	}

	//���㻺�桢��������ʹ��3�����ʶ��㻺��
	//�������ݵ�����
	CUSTOMVERTEX Vertices[] =
	{
		{ -20.0f, 20.0f, -20.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ -20.0f, 20.0f, 20.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) }, 
		{ 20.0f, 20.0f, 20.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ 20.0f, 20.0f, -20.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ -20.0f, -20.0f, -20.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ -20.0f, -20.0f, 20.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) }, 
		{ 20.0f, -20.0f, 20.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ 20.0f, -20.0f, -20.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },

	};
	


	//��䶥�㻺����
	VOID* pVertices;
	if(FAILED(g_pVertexBuffer->Lock(0,sizeof(Vertices),(void**)&pVertices,0)))
		return E_FAIL;
	memcpy(pVertices,Vertices,sizeof(Vertices));
	g_pVertexBuffer->Unlock();

	//�����������
	WORD *pIndices = NULL;
	g_pIndexBuffer->Lock(0,0,(void**)&pIndices,0);

	pIndices[0] = 0, pIndices[1] = 1, pIndices[2] = 2;
	pIndices[3] = 0, pIndices[4] = 2, pIndices[5] = 3;
	// ����
	pIndices[6] = 0, pIndices[7]  = 3, pIndices[8]  = 7;
	pIndices[9] = 0, pIndices[10] = 7, pIndices[11] = 4;
	// �����
	pIndices[12] = 0, pIndices[13] = 4, pIndices[14] = 5;
	pIndices[15] = 0, pIndices[16] = 5, pIndices[17] = 1;
	// �Ҳ���
	pIndices[18] = 2, pIndices[19] = 6, pIndices[20] = 7;
	pIndices[21] = 2, pIndices[22] = 7, pIndices[23] = 3;
	// ����
	pIndices[24] = 2, pIndices[25] = 5, pIndices[26] = 6;
	pIndices[27] = 2, pIndices[28] = 1, pIndices[29] = 5;
	// ����
	pIndices[30] = 4, pIndices[31] = 6, pIndices[32] = 5;
	pIndices[33] = 4, pIndices[34] = 7, pIndices[35] = 6;

	g_pIndexBuffer->Unlock();

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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

	//g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);//������Ⱦ״̬

	//���㻺������������ʹ��4������ͼ��
	Matrix_Set();

	if (::GetAsyncKeyState(0x31) & 0x8000f)
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	if (::GetAsyncKeyState(0x32) & 0x8000f)
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

	g_pd3dDevice->SetStreamSource(0,g_pVertexBuffer,0,sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIndexBuffer);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);


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
	D3DXVECTOR3 vEye(0.0f, 0.0f, -200.0f);
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