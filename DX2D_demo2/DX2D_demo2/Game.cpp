#include "DirectX.h"


using namespace std;

LPCTSTR APPTITLE = L"HEHE";
const int SCREENW = 1024;
const int SCREENH = 576;

LPDIRECT3DTEXTURE9 image_colorkey = NULL;
LPDIRECT3DTEXTURE9 image_alpha = NULL;
LPDIRECT3DTEXTURE9 image_notrans = NULL;


bool Game_Init(HWND window)
{
	Direct3D_Init(window, SCREENW, SCREENH, false);
	DirectInput_Init(window);

	image_notrans = LoadTexture(L"shuttle_notrans.bmp", D3DCOLOR_XRGB(0,0,0));
	if (!image_notrans)
		return false;

	image_colorkey = LoadTexture(L"shuttle_colorkey.bmp", D3DCOLOR_XRGB(0, 0, 0));
	if (!image_colorkey)
		return false;

	image_alpha = LoadTexture(L"shuttle_alpha.tga", D3DCOLOR_XRGB(0, 0, 0));
	if (!image_alpha)
		return false;

	return true;


}

void Game_Run(HWND window)
{
	if (!d3ddev)
		return;

	DirectInput_Update();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (d3ddev->BeginScene())
	{
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXVECTOR3 pos1(10, 0, 0);
		spriteobj->Draw(image_notrans, NULL, NULL, &pos1, D3DCOLOR_XRGB(255, 255, 255));

		D3DXVECTOR3 pos2(350, 0, 0);
		spriteobj->Draw(image_colorkey, NULL, NULL, &pos2, D3DCOLOR_XRGB(255, 255, 255));

		D3DXVECTOR3 pos3(700, 0, 0);
		spriteobj->Draw(image_alpha, NULL, NULL, &pos3, D3DCOLOR_XRGB(255, 255, 255));

		spriteobj->End();

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	if (Key_Down(DIK_ESCAPE))
		gameover = true;
}

void Game_End()
{
	image_alpha->Release();
	image_colorkey->Release();
	image_notrans->Release();

	DirectInput_Shutdown();
	Direct3D_Shutdown();
}