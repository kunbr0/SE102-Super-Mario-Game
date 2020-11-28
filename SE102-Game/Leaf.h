#pragma once
#include "GameObject.h"


class CLeaf : public CGameObject
{
public:
	CLeaf(Vector2);
	void Render(Vector2) override;
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;	
	void GetBoundingBox(float&, float&, float&, float&) override;
};

