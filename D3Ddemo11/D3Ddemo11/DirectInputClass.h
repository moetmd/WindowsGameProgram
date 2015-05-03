
#pragma once
#include "D3DUtil.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//DInputClass
class DInputClass
{
private:
	IDirectInput8 * m_pDirectInput;
	IDirectInputDevice8 * m_KeyboardDevice;
	char m_keyBuffer[256];

	IDirectInputDevice8 * m_MouseDevice;
	DIMOUSESTATE m_MouseState;

public:
	HRESULT Init(HWND hWnd,HINSTANCE hInstance,DWORD keyboardCoopFlags,DWORD mouseCoopFlags);
	void GetInput();
	bool IsKeyDown(int iKey);

	bool IsMouseButtonDown(int button);
	float MouseDX();
	float MouseDY();
	float MouseDZ();

public:
	DInputClass(void);
	~DInputClass(void);
};