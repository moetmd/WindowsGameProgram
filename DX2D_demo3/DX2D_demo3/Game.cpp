#include "DirectX.h"
using namespace std;

const LPCTSTR APPTITLE = L"AnimateSprite";
const int SCREENW = 1024;
const int SCREENH = 576;

LPDIRECT3DTEXTURE9 explosion = NULL;
int frame = 0;
int starttime = 0;

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

	//load explosion sprite
	explosion = LoadTexture(L"explosion_30_128.tga");
	if (!explosion)
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
		Sprite_Animate(frame, 0, 29, 1, starttime, 30);
		Sprite_Draw_Frame(explosion, 450, 200, frame, 128, 128, 6);

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
	explosion->Release();
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}