#pragma once

#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <XInput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <tchar.h>
using namespace std;

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

extern const LPCTSTR APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameover;

extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;


//sprite structure
struct SPRITE
{
	float x, y;
	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;
	int direction;
	float velx, vely;
	D3DCOLOR color;

	SPRITE()
	{
		frame = 0;
		columns = 1;
		width = height = 0;
		scaling = 1.0f;
		startframe = endframe = 0;
		direction = 1;
		starttime = delay = 0;
		velx = vely = 0.0f;
		color = D3DCOLOR_XRGB(255, 255, 255);
	}
};

//Direct3D functions
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
bool LoadSurface(LPCTSTR filename, D3DXIMAGE_INFO &info, LPDIRECT3DSURFACE9 &image);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
D3DXVECTOR2 GetBitmapSize(LPCTSTR filename);
LPDIRECT3DTEXTURE9 LoadTexture(LPCTSTR filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
extern LPD3DXSPRITE spriteobj;

//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();

//game functions
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();

//draw functions
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
	int framenum, int framew, int frameh, int columns);
void Sprite_Animate(int &frame, int startframe, int endframe,
	int direction, int &starttime, int delay);
void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width,
	int height, int frame, int columns, float rotation, float scaling, D3DCOLOR color);

//physics functions
int Collision(SPRITE sprite1, SPRITE sprite2);
bool CollisionD(SPRITE sprite1, SPRITE sprite2);

//font functions


//get fps functions
float Get_FPS();