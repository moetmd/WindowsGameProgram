#include "DirectX.h"
using namespace std;

const LPCTSTR APPTITLE = L"AnimateSprite";
const int SCREENW = 1024;
const int SCREENH = 576;

LPDIRECT3DTEXTURE9 sunflower = NULL;
LPDIRECT3DTEXTURE9 walkman = NULL;
D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

static float scale = 0.001f;
static float r = 0;
static float s = 1.0f;
int frame(0), columns, width, height;
int startframe, endframe, starttime(0), delay;

float scale1 = 0.004f;
float r1 = 0;
float s1 = 1.0f;
int frame1(0), columns1, width1, height1;
int startframe1, endframe1, starttime1(0), delay1;




//game initialization
bool Game_Init(HWND window)
{
	if (!Direct3D_Init(window, SCREENW, SCREENH, false))
	{
		MessageBox(0, L"Error initialization Direct3D", L"Error", 0);
		return false;
	}
	if (!DirectInput_Init(window))
	{
		MessageBox(0, L"Error initialization DirectInput", L"Error", 0);
		return false;
	}

	sunflower = LoadTexture(L"sunflower.bmp");
	if (!sunflower)
		return false;

	walkman = LoadTexture(L"walkman.png");
	if (!walkman)
		return false;

	return true;
}

//game update
void Game_Run(HWND window)
{
	//make sure the Direct3D device is valid
	if (!d3ddev)
		return;

	//update input devices
	DirectInput_Update();

	//clear the scene
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//start rendering
	if (d3ddev->BeginScene())
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//animate and draw the sprite
		r = timeGetTime() / 500.0f;
		s += scale;
		if (s<0.1 || s>1.15f)
			scale *= -1;

		width = 512;
		height = 512;
		frame = 0;
		columns = 1;
		Sprite_Transform_Draw(sunflower, 10, 10, width, height, frame, columns, r, s, color);


		s1 += scale1;
		if (s1<0.5f || s1>6.0f)
			scale1 *= 1;

		columns1 = 8;
		width1 = height1 = 96;
		startframe1 = 24;
		endframe = 31;
		delay1 = 90;
		Sprite_Animate(frame1, startframe1, endframe1, 1, starttime1, delay1);
		Sprite_Transform_Draw(walkman, 500, 10, width1, height1, frame1, columns1, 0, s1, color);



		//stop drawing
		spriteobj->End();

		//stop rendering
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	//escape key exits
	if (Key_Down(DIK_ESCAPE))
		gameover = true;
}

void Game_End()
{
	//free memory
	sunflower->Release();
	walkman->Release();
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}