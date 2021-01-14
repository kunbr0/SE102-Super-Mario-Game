#pragma once
#include "GameObject.h"


class CLeaf : public CGameObject
{
	const float vyMax = 0.056f;
	int beginFalling = 0;
	Vector2 initPosition;
public:
	CLeaf(Vector2);
	void Render(Vector2) override;
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;	
	void Collided(LPGAMEOBJECT) override;
	void GetBoundingBox(float&, float&, float&, float&) override;
};

