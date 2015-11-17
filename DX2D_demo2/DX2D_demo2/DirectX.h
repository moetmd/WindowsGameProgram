#pragma once

#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <Xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
using namespace std;

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

extern LPCTSTR APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameover;

extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;

//Direct3D functions
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
bool LoadSurface(LPCTSTR filename, D3DXIMAGE_INFO &info, LPDIRECT3DSURFACE9 &image);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
D3DXVECTOR2 GetBitmapSize(LPCTSTR filename);
LPDIRECT3DTEXTURE9 LoadTexture(LPCTSTR filename, D3DCOLOR transcolor);

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
extern XINPUT_GAMEPAD controllers[4];
extern LPD3DXSPRITE spriteobj;

//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void XInput_Vibrate(int contNum = 0, int amount = 65535);
bool XInput_Controller_Found();

//Game functions
bool Game_Init(HWND window);
void Game_Run(HWND windwo);
void Game_End();


