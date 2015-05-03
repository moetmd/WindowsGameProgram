//-----------------------------------【头文件包含部分】---------------------------------------
//	描述：包含程序所依赖的头文件
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


//-----------------------------------【宏定义部分】--------------------------------------------
//	描述：定义一些辅助宏
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	600							//为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE	L"D3Ddemo15"		//为窗口标题定义的宏
#define SAFE_RELEASE(p){if (p){(p)->Release();(p)=NULL;}} //定义安全释放宏


//-----------------------------------【全局结构体】--------------------------------------------

//顶点缓存使用1、设计顶点格式
struct CUSTOMVERTEX
{
	FLOAT _x,_y,_z;
	FLOAT _u,_v;
	CUSTOMVERTEX(FLOAT x,FLOAT y,FLOAT z,FLOAT u,FLOAT v)
		: _x(x),_y(y),_z(z),_u(u),_v(v) {}
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1) //FVF灵活顶点格式



//-----------------------------------【全局函数声明部分】-------------------------------------
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );  //窗口过程函数
HRESULT             Direct3D_Init(HWND hwnd);
HRESULT             Objects_Init(HWND hwnd);
VOID                Direct3D_Render(HWND hwnd);
VOID                Direct3D_CleanUp();
VOID                Direct3D_Update(HWND hwnd);
float               Get_FPS();
VOID                Matrix_Set();
VOID                Light_Set(LPDIRECT3DDEVICE9 pd3dDevice, UINT nType);

//-----------------------------------【全局变量声明部分】-------------------------------------
LPDIRECT3DDEVICE9	g_pd3dDevice            = NULL;  //Direct3D设备对象
LPD3DXFONT			g_pTextFPS				= NULL;  //字体COM接口
LPD3DXFONT			g_pTextAdaperName       = NULL;  // 显卡信息的2D文本
LPD3DXFONT			g_pTextHelper           = NULL;  // 帮助信息的2D文本
LPD3DXFONT			g_pTextInfor            = NULL;  // 绘制信息的2D文本
float				g_FPS					= 0.0f;  //一个浮点型的变量，代表帧速率
wchar_t				g_strFPS[50]            = {0};   //包含帧速率的字符数组
wchar_t				g_strAdapterName[60]    = {0};   //包含显卡名称的字符数组
D3DXMATRIX			g_matWorld;                      //世界矩阵
DInputClass*  	    g_pDInput               = NULL;  //一个DInputClass类的指针

LPD3DXMESH          g_pMesh                 = NULL;  // 网格的对象
D3DMATERIAL9*       g_pMaterials            = NULL;  // 网格的材质信息
LPDIRECT3DTEXTURE9* g_pTextures             = NULL;  // 网格的纹理信息
DWORD               g_dwNumMtrls            = 0;     // 材质的数目

LPD3DXMESH          g_pMeshWall             = NULL;  // 墙面网格对象
D3DMATERIAL9        g_MaterialsWall;                 //墙面材质

LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer     = NULL;  //顶点缓存对象
LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer      = NULL;  //索引缓存对象
LPDIRECT3DTEXTURE9      g_pTexture          = NULL;  //纹理接口对象


//-----------------------------------【WinMain( )函数】--------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	//【1】窗口创建四步曲之一：开始设计一个完整的窗口类
	WNDCLASSEX wndClass    = { 0 };							//用WINDCLASSEX定义了一个窗口类
	wndClass.cbSize        = sizeof( WNDCLASSEX ) ;	        //设置结构体的字节数大小
	wndClass.style         = CS_HREDRAW | CS_VREDRAW;       //设置窗口的样式
	wndClass.lpfnWndProc   = WndProc;					    //设置指向窗口过程函数的指针
	wndClass.cbClsExtra	   = 0;						        //窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra	   = 0;	   						    //窗口的附加内存，依然取0就行了
	wndClass.hInstance     = hInstance;						//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon         = (HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,
		                     0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); 
															//本地加载自定义ico图标
	wndClass.hCursor       = LoadCursor( NULL, IDC_ARROW );       //指定窗口类的光标句柄。
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个灰色画刷句柄	
	wndClass.lpszMenuName  = NULL;						          //用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"D3Ddemo15";		                  //用一个以空终止的字符串，指定窗口类的名字。

	//【2】窗口创建四步曲之二：注册窗口类
	if( !RegisterClassEx( &wndClass ) )				        //设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;		

	//【3】窗口创建四步曲之三：正式创建窗口
	HWND hwnd = CreateWindow( L"D3Ddemo15",WINDOW_TITLE,	//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );


	if (!(S_OK == Direct3D_Init(hwnd)))
	{
		MessageBox(hwnd,L"初始化失败",L"提示",0);
	}


	//【4】窗口创建四步曲之四：窗口的移动、显示与更新
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
	ShowWindow( hwnd, nShowCmd );                                   //调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);						                        //对窗口进行更新，就像我们买了新房子要装修一样

	//PlaySound(L"",NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
	//MessageBox(hwnd,L"",L"",0);

	//DirectInput类的初始化
	g_pDInput = new DInputClass();
	g_pDInput->Init(hwnd,hInstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);



	//【5】消息循环过程
	MSG msg = { 0 };		//定义并初始化msg
	while( msg.message != WM_QUIT )			//使用while循环，如果消息不是WM_QUIT消息，就继续循环
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage( &msg );		//将虚拟键消息转换为字符消息
			DispatchMessage( &msg );			//分发一个消息给窗口程序。
		}else
		{
			Direct3D_Update(hwnd);         //对设备输入（键盘鼠标）进行响应
			Direct3D_Render(hwnd);         //渲染
		}
	}

	//【6】窗口类的注销
	UnregisterClass(L"D3Ddemo15", wndClass.hInstance);  //程序准备结束，注销窗口类
	return 0;  
}


//-----------------------------------【WndProc( )函数】--------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{
	switch( message )						//switch语句开始
	{
	case WM_PAINT:						// 若是客户区重绘消息
		Direct3D_Render(hwnd);
		ValidateRect(hwnd, NULL);		// 更新客户区的显示
		break;									//跳出该switch语句

	case WM_KEYDOWN:                // 若是键盘按下消息
		if (wParam == VK_ESCAPE)    // 如果被按下的键是ESC
			DestroyWindow(hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
		break;									//跳出该switch语句

	case WM_DESTROY:				//若是窗口销毁消息
		PostQuitMessage( 0 );		//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;								//跳出该switch语句

	default:									//若上述case条件都不符合，则执行该default语句
		return DefWindowProc( hwnd, message, wParam, lParam );		//调用缺省的窗口过程
	}

	return 0;			//正常退出
}

//Direct3D_Init
//Direct3D的初始化
HRESULT Direct3D_Init(HWND hwnd)
{
	//1、创建Direct3D接口对象
	LPDIRECT3D9 pD3D = NULL;
	if(NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))	//初始化Direct3D接口对象，并进行DirectX版本协商
		return S_FALSE;

	//2、获取硬件设备信息
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps)))
	{
		return E_FAIL;
	}
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//3、填充D3DPRESENT_PARAMETERS结构体
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


	//4、创建Direct3D设备接口
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,vp,&d3dpp,&g_pd3dDevice)))
		return E_FAIL;

	//获取显卡信息到g_strAdapterName中，并在显卡名称之前加上“当前显卡型号：”字符串
	wchar_t TempName[60]=L"当前显卡型号：";   //定义一个临时字符串，且方便了把"当前显卡型号："字符串引入我们的目的字符串中
	D3DADAPTER_IDENTIFIER9 Adapter;  //定义一个D3DADAPTER_IDENTIFIER9结构体，用于存储显卡信息
	pD3D->GetAdapterIdentifier(0,0,&Adapter);//调用GetAdapterIdentifier，获取显卡信息
	int len = MultiByteToWideChar(CP_ACP,0, Adapter.Description, -1, NULL, 0);//显卡名称现在已经在Adapter.Description中了，但是其为char类型，我们要将其转为wchar_t类型
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_strAdapterName, len);//这步操作完成后，g_strAdapterName中就为当前我们的显卡类型名的wchar_t型字符串了
	wcscat_s(TempName,g_strAdapterName);//把当前我们的显卡名加到“当前显卡型号：”字符串后面，结果存在TempName中
	wcscpy_s(g_strAdapterName,TempName);//把TempName中的结果拷贝到全局变量g_strAdapterName中


	SAFE_RELEASE(pD3D)
    
	if(!(S_OK == Objects_Init(hwnd)))
		return E_FAIL;
	return S_OK;
}


//Object_Init
//渲染资源初始化
HRESULT Objects_Init(HWND hwnd)
{

	//创建字体
	D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1000, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);
	D3DXCreateFont(g_pd3dDevice, 20, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"华文中宋", &g_pTextAdaperName); 
	D3DXCreateFont(g_pd3dDevice, 23, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"微软雅黑", &g_pTextHelper); 
	D3DXCreateFont(g_pd3dDevice, 26, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"黑体", &g_pTextInfor); 

	
	//从x文件加载网格数据
	LPD3DXBUFFER pAdjBuffer = NULL;
	LPD3DXBUFFER pMtrlBuffer = NULL;

	D3DXLoadMeshFromX(L"miki.x",D3DXMESH_MANAGED,g_pd3dDevice,&pAdjBuffer,&pMtrlBuffer,
		NULL,&g_dwNumMtrls,&g_pMesh);

	//读取材质和纹理数据
	D3DXMATERIAL *pMtrls = (D3DXMATERIAL *)pMtrlBuffer->GetBufferPointer();
	g_pMaterials = new D3DMATERIAL9[g_dwNumMtrls];
	g_pTextures = new LPDIRECT3DTEXTURE9[g_dwNumMtrls];

	for (DWORD i=0;i<g_dwNumMtrls;i++)
	{
		//获取材质并设置环境光颜色
		g_pMaterials[i] = pMtrls[i].MatD3D;
		g_pMaterials[i].Ambient = g_pMaterials[i].Diffuse;



		//创建纹理对象
		g_pTextures[i] = NULL;
		D3DXCreateTextureFromFileA(g_pd3dDevice,pMtrls[i].pTextureFilename,&g_pTextures[i]);
	}

	SAFE_RELEASE(pAdjBuffer);
	SAFE_RELEASE(pMtrlBuffer);

	//创建墙面
	D3DXCreateBox(g_pd3dDevice,120.0f,120.0f,0.3f,&g_pMeshWall,NULL);
	g_MaterialsWall.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_MaterialsWall.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_MaterialsWall.Specular = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
	
	//设置光照
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

	
	// 设置渲染状态
	//g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS,true);   //初始化顶点法线
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //开启背面消隐
	//g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); //设置环境光
	g_pd3dDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESS);//深度测试函数设置
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,true);//深度测试成功后更新缓存

	//各项异性过滤
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,3);
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
	g_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);

	//线性纹理过滤
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);

	//最近点采样过滤
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

	//渐进纹理过滤
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	//g_pd3dDevice->SetSamplerState(0,D3DSAMP_MAXMIPLEVEL,16);

	//开启alpha融合
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		//设置融合因子
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		//设置融合运算方式
	g_pd3dDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);

	Matrix_Set();//设置四大变换
	


	return S_OK;
}


//Direct3D_Render
//使用Direct3D进行渲染
VOID Direct3D_Render(HWND hwnd)
{
	//1、清屏
	g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(10,10,10),1.0f,0);

	RECT formatRect;
	GetClientRect(hwnd,&formatRect);

	//2、开始绘制
	g_pd3dDevice->BeginScene();


	//3、正式绘制
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

	//绘制镜子
	D3DXMatrixTranslation(&matWorld,0.0f,0.0f,0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matWorld);
	g_pd3dDevice->SetMaterial(&g_MaterialsWall);
	g_pMeshWall->DrawSubset(0);

	//启用模版缓存并设置绘制状态
	g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILREF,0x1);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILMASK,0xffffffff);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);

	//进行融合，并禁止向深度缓存写数据
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	//绘制作为镜面的区域
	D3DXMatrixTranslation(&matWorld,0.0f,0.0f,0.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matWorld); 
	g_pd3dDevice->SetMaterial(&g_MaterialsWall);
	g_pMeshWall->DrawSubset(0);

	//重新设置渲染状态，将镜面与镜像融合，清理z缓存
	g_pd3dDevice->Clear(0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,true);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);

	//计算镜像变换矩阵
	D3DXMATRIX matReflect;
	D3DXPLANE planeXY(0.0f,0.0f,1.0f,0.0f);
	D3DXMatrixReflect(&matReflect,&planeXY);
	matWorld = matReflect * matHero;

	//绘制镜子中的模型
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&matWorld);
	for (DWORD i=0;i<g_dwNumMtrls;i++)
	{
		g_pd3dDevice->SetMaterial(&g_pMaterials[i]);
		g_pd3dDevice->SetTexture(0,g_pTextures[i]);
		g_pMesh->DrawSubset(i);
	}

	//恢复渲染状态
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
	g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE,false);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	//在窗口右上角处，显示每秒帧数
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));

	//显示显卡类型名
	g_pTextAdaperName->DrawText(NULL,g_strAdapterName, -1, &formatRect, 
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	// 输出绘制信息
	formatRect.top = 30;
	static wchar_t strInfo[256] = {0};
	swprintf_s(strInfo,-1, L"模型坐标: (%.2f, %.2f, %.2f)", g_matWorld._41, g_matWorld._42, g_matWorld._43);
	g_pTextHelper->DrawText(NULL, strInfo, -1, &formatRect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(135,239,136,255));

	// 输出帮助信息
	formatRect.left = 0,formatRect.top = 380;
	g_pTextInfor->DrawText(NULL, L"控制说明:", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(235,123,230,255));
	formatRect.top += 35;
	g_pTextHelper->DrawText(NULL, L"    按住鼠标左键并拖动：平移模型", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    按住鼠标右键并拖动：旋转模型", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    滑动鼠标滚轮：拉伸模型", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    W、S、A、D键：平移模型 ", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    上、下、左、右方向键：旋转模型 ", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));
	formatRect.top += 25;
	g_pTextHelper->DrawText(NULL, L"    ESC键 : 退出程序", -1, &formatRect, 
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255,200,0,255));



	//4、结束绘制
	g_pd3dDevice->EndScene();

	//5、翻转
	g_pd3dDevice->Present(NULL,NULL,NULL,NULL);
}

//Direct3D_Update
void Direct3D_Update(HWND hwnd)
{
	g_pDInput->GetInput();

	//设置纹理寻址方式
	if (g_pDInput->IsKeyDown(DIK_1))
	{
		//重复纹理寻址模式
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	}
	if (g_pDInput->IsKeyDown(DIK_2))
	{
		//镜像寻址模式
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);
	}
	if (g_pDInput->IsKeyDown(DIK_3))
	{
		//夹取纹理寻址模式
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
	}
	if (g_pDInput->IsKeyDown(DIK_4))
	{
		//边框纹理寻址模式
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_BORDER);
		g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_BORDER);
	}

	//通过5、6按键控制漫反射alpha分量的变化
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




	//按住鼠标左键拖动，平移操作
	static FLOAT fPosX = 0.0f, fPosY = 0.0f, fPosZ = 0.0f;

	if (g_pDInput->IsMouseButtonDown(0))
	{
		fPosX += (g_pDInput->MouseDX())*0.08f;
		fPosY += (g_pDInput->MouseDY())*-0.08f;
	}

	//鼠标滚轮，视点收缩
	fPosZ += (g_pDInput->MouseDZ())*0.02f;

	//平移物体
	if(g_pDInput->IsKeyDown(DIK_A)) fPosX -= 0.005f;
	if(g_pDInput->IsKeyDown(DIK_D)) fPosX += 0.005f;
	if(g_pDInput->IsKeyDown(DIK_W)) fPosY += 0.005f;
	if(g_pDInput->IsKeyDown(DIK_S)) fPosY -= 0.005f;

	D3DXMatrixTranslation(&g_matWorld,fPosX,fPosY,fPosZ);

	//按住鼠标右键，旋转操作
	static float fAngleX = 0.0f, fAngleY = 0.0f;

	if(g_pDInput->IsMouseButtonDown(1))
	{
		fAngleX += (g_pDInput->MouseDY())*-0.001f;
		fAngleY += (g_pDInput->MouseDY())*-0.001f;
	}

	//旋转物体
	if(g_pDInput->IsKeyDown(DIK_UP)) fAngleX += 0.005f;
	if(g_pDInput->IsKeyDown(DIK_DOWN)) fAngleX -= 0.005f;
	if(g_pDInput->IsKeyDown(DIK_LEFT)) fAngleY -= 0.005f;
	if(g_pDInput->IsKeyDown(DIK_RIGHT)) fAngleY += 0.005f;


	D3DXMATRIX Rx,Ry;
	D3DXMatrixRotationX(&Rx,fAngleX);
	D3DXMatrixRotationY(&Ry,fAngleY);

	//最终矩阵
	g_matWorld = Rx * Ry * g_matWorld;//最终的矩阵
	g_pd3dDevice->SetTransform(D3DTS_WORLD,&g_matWorld);//设置世界矩阵

}

//Direct3D_CleanUp
//资源清理
VOID Direct3D_CleanUp()
{
	//释放COM接口对象
	SAFE_RELEASE(g_pTextAdaperName)
	SAFE_RELEASE(g_pTextHelper)
	SAFE_RELEASE(g_pTextInfor)
	SAFE_RELEASE(g_pTextFPS)
	
	SAFE_RELEASE(g_pVertexBuffer)
	SAFE_RELEASE(g_pIndexBuffer)

	SAFE_RELEASE(g_pd3dDevice)
}

//Get_FPS()  用于计算FPS
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
	//1世界变换


	//2取景变换
	D3DXMATRIX matView;
	D3DXVECTOR3 vEye(100.0f, 0.0f, -300.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);


	//3投影变换
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f,
		(float)((double)WINDOW_WIDTH/WINDOW_HEIGHT),1.0f, 10000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);


	//4视口变换
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	g_pd3dDevice->SetViewport(&vp);

}

//设置光源类型
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


