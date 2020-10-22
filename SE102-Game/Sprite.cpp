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
	RECT r;
	r.left = left; r.right = right; r.top = top; r.bottom = bottom;
	game->Draw(finalPos, texture, r, alpha);
}

void CSprite::DrawFlipY(Vector2 finalPos, int alpha)
{
	CGame* game = CGame::GetInstance();
	RECT r;
	r.left = left; r.right = right; r.top = top; r.bottom = bottom;
	
	game->DrawFlipY(finalPos, Vector2(finalPos.x + (left+right)/2, finalPos.y + (top+bottom)/2), texture, r, alpha);
}



void CSprite::getSize(int& width, int& height) {
	width = this->right - this->left;
	height = this->bottom - this->top;
}