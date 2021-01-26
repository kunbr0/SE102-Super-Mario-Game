#pragma once
#include "GameObject.h"


class CLeaf : public CGameObject
{
	const float vyMax = 0.056f;
	int beginFalling = 0;
	Vector2 initPosition;
public:
	CLeaf(Vector2);
	virtual void Render(Vector2) override;
	virtual void Update(DWORD, vector<LPGAMEOBJECT>*) override;	
	virtual void Collided(LPGAMEOBJECT) override;
	virtual void GetBoundingBox(float&, float&, float&, float&) override;
};

