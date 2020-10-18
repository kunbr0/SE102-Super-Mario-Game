#include "Sprite.h"
#include "Game.h"
#include "debug.h"

CSprite::CSprite(std::string id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}





void CSprite::Draw(Vector2 finalPos, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->Draw(finalPos, texture, left, top, right, bottom, alpha);
}

void CSprite::DrawWithScaling(float x, float y, D3DXVECTOR2 scalingCenter, D3DXVECTOR2 scale, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->DrawWithScaling(x, y, texture, left, top, right, bottom, scalingCenter, scale, 255);
}

void CSprite::getSize(int& width, int& height) {
	width = this->right - this->left;
	height = this->bottom - this->top;
}