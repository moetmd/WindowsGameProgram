#include "DirectInputClass.h"

//构造函数
DInputClass::DInputClass()
{
	m_pDirectInput=NULL;
	m_KeyboardDevice=NULL;
	ZeroMemory(m_keyBuffer,sizeof(char)*256);
	m_MouseDevice=NULL;
	ZeroMemory(&m_MouseState,sizeof(m_MouseState));
}

//初始化键盘及鼠标输入设备
HRESULT DInputClass::Init(HWND hWnd,HINSTANCE hInstance,DWORD keyboardCoopFlags,DWORD mouseCoopFlags)
{
	HRESULT hr;
	HR(DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&m_pDirectInput,NULL));

	HR(m_pDirectInput->CreateDevice(GUID_SysKeyboard,&m_KeyboardDevice,NULL));
	HR(m_KeyboardDevice->SetCooperativeLevel(hWnd,keyboardCoopFlags));
	HR(m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	HR(m_KeyboardDevice->Acquire());
	HR(m_KeyboardDevice->Poll());

	HR(m_pDirectInput->CreateDevice(GUID_SysMouse,&m_MouseDevice,NULL));
	HR(m_MouseDevice->SetCooperativeLevel(hWnd,mouseCoopFlags));
	HR(m_MouseDevice->SetDataFormat(&c_dfDIMouse));
	HR(m_MouseDevice->Acquire());
	HR(m_MouseDevice->Poll());

	return S_OK;
}

//获取输入信息
void DInputClass::GetInput()
{
	HRESULT hr = m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer),(void**)&m_keyBuffer);

	if(hr)
	{
		m_KeyboardDevice->Acquire();

		m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer),(LPVOID)m_keyBuffer);

	}

	hr = m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),(void**)&m_MouseState);

	if (hr)
	{
		m_MouseDevice->Acquire();
		m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),(void**)&m_MouseState);
	}
}

//判断键盘上某个键是否按下
bool DInputClass::IsKeyDown(int iKey)
{
	if(m_keyBuffer[iKey]& 0x80)
		return true;
	else
		return false;
}

//判断鼠标上某个键是否按下
bool DInputClass::IsMouseButtonDown(int button)
{
	return(m_MouseState.rgbButtons[button] & 0x80) != 0;
}

//返回鼠标指针的x坐标
float DInputClass::MouseDX()
{
	return (float)m_MouseState.lX;
}

//返回鼠标指针的y坐标
float DInputClass::MouseDY()
{
	return (float)m_MouseState.lY;
}

//返回鼠标指针的z坐标（滚轮）
float DInputClass::MouseDZ()
{
	return (float)m_MouseState.lZ;
}

//析构函数
DInputClass::~DInputClass(void)
{
	if(m_KeyboardDevice != NULL)
		m_KeyboardDevice->Unacquire();
	if(m_MouseDevice != NULL)
		m_MouseDevice->Unacquire();
	SAFE_RELEASE(m_KeyboardDevice);
	SAFE_RELEASE(m_MouseDevice);
	SAFE_RELEASE(m_pDirectInput);
}