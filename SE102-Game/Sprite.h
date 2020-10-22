#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "Transform.h"

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
	void Draw(Vector2 finalPos, int alpha = 255);
	void DrawFlipY(Vector2 finalPos, int alpha);

};

typedef CSprite* LPSPRITE;