#pragma once

#include "Leaf.h"

class COneUpGreen : public CLeaf
{
	const float vyMax = 0.186f;
public:
	COneUpGreen(Vector2);
	void Render(Vector2 finalPos) override;
	void Update(DWORD, vector<LPGAMEOBJECT>*) override;
};

