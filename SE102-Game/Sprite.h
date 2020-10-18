#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class CSprite
{
	std::string id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	CSprite(std::string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	
	void getSize(int &width, int &height);
	void Draw(float x, float y, int alpha = 255);
	void DrawWithScaling(float x, float y, D3DXVECTOR2 scalingCenter, D3DXVECTOR2 scale, int alpha = 255);
};

typedef CSprite* LPSPRITE;