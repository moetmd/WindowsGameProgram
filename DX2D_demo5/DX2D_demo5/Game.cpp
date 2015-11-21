#include "DirectX.h"
using namespace std;

const LPCTSTR APPTITLE = L"Bounding Box";
const int SCREENW = 1024;
const int SCREENH = 576;

ID3DXFont* g_pFont = NULL;

wchar_t strFPS[50];

SPRITE ship, asteroid1, asteroid2;
LPDIRECT3DTEXTURE9 imgShip = NULL;
LPDIRECT3DTEXTURE9 imgAsteroid = NULL;

//game initialization
bool Game_Init(HWND window)
{
	//init direct3d
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

	//load texture
	imgShip = LoadTexture(L"fatship.tga");
	if (!imgShip)
		return false;

	imgAsteroid = LoadTexture(L"asteroid.tga");
	if (!imgAsteroid)
		return false;

	//create font
	if (FAILED(D3DXCreateFont(d3ddev, 36, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Î¢ÈíÑÅºÚ"), &g_pFont)))
		return false;

	//set properties for sprites
	ship.x = 450;
	ship.y = 200;
	ship.width = ship.height = 128;

	asteroid1.x = 50;
	asteroid1.y = 100;
	asteroid1.width = asteroid1.height = 60;
	asteroid1.columns = 8;
	asteroid1.startframe = 0;
	asteroid1.endframe = 63;
	asteroid1.velx = -0.02f;
	asteroid1.vely = -0.02f;

	asteroid2.x = 900;
	asteroid2.y = 400;
	asteroid2.width = asteroid2.height = 60;
	asteroid2.columns = 8;
	asteroid2.startframe = 0;
	asteroid2.endframe = 63;
	asteroid2.velx = 0.02f;
	asteroid2.vely = -0.02f;

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

	//font rect
	RECT fontRect;
	GetClientRect(window, &fontRect);

	//clear the scene
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//move with keys
	if (Key_Down(DIK_W))
	{
		ship.y -= 0.01f;
		if (ship.y < 0)
			ship.y = 0;
	}
	if (Key_Down(DIK_S))
	{
		ship.y += 0.01f;
		if (ship.y > SCREENH - ship.height)
			ship.y = SCREENH - ship.height;
	}
	if (Key_Down(DIK_A))
	{
		ship.x -= 0.01f;
		if (ship.x < 0)
			ship.x = 0;
	}
	if (Key_Down(DIK_D))
	{
		ship.x += 0.01f;
		if (ship.x > SCREENW - ship.width)
			ship.x = SCREENW - ship.width;
	}

	//move and animate 
	if (asteroid1.x < 0 || asteroid1.x > SCREENW - asteroid1.width)
		asteroid1.velx *= -1;
	if(asteroid1.y < 0 || asteroid1.y > SCREENH - asteroid1.height)
		asteroid1.vely *= -1;

	asteroid1.x += asteroid1.velx;
	asteroid1.y += asteroid1.vely;

	Sprite_Animate(asteroid1.frame, asteroid1.startframe, asteroid1.endframe,
		asteroid1.direction, asteroid1.starttime, asteroid1.delay);

	
	if (asteroid2.x < 0 || asteroid2.x > SCREENW - asteroid2.width)
		asteroid2.velx *= -1;
	if (asteroid2.y < 0 || asteroid2.y > SCREENH - asteroid2.height)
		asteroid2.vely *= -1;

	asteroid2.x += asteroid2.velx;
	asteroid2.y += asteroid2.vely;

	Sprite_Animate(asteroid2.frame, asteroid2.startframe, asteroid2.endframe,
		asteroid2.direction, asteroid2.starttime, asteroid2.delay);

	//test for Collisions
	if (CollisionD(ship, asteroid1))
	{
		asteroid1.velx *= -1;
		asteroid1.vely *= -1;
	}

	if (CollisionD(ship, asteroid2))
	{
		asteroid2.velx *= -1;
		asteroid2.vely *= -1;
	}


	//start rendering
	if (d3ddev->BeginScene())
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//animate and draw the sprite
		Sprite_Transform_Draw(imgShip, ship.x, ship.y, ship.width, ship.height,
			ship.frame, ship.columns, 0, 1.0f, ship.color);

		Sprite_Transform_Draw(imgAsteroid, asteroid1.x, asteroid1.y, asteroid1.width,
			asteroid1.height, asteroid1.frame, asteroid1.columns, 0, 1.0f, asteroid1.color);

		Sprite_Transform_Draw(imgAsteroid, asteroid2.x, asteroid2.y, asteroid2.width,
			asteroid2.height, asteroid2.frame, asteroid2.columns, 0, 1.0f, asteroid2.color);

		//draw FPS
		int charCount = swprintf_s(strFPS, 20, _T("FPS:%0.3f"), Get_FPS());
		g_pFont->DrawText(NULL, strFPS, charCount, &fontRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255,39,136));


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
	if (imgShip)
		imgShip->Release();
	if (imgAsteroid)
		imgAsteroid->Release();

	DirectInput_Shutdown();
	Direct3D_Shutdown();
}