#pragma once
#include <string>
#include "Transform.h"

class CUIDrawer
{
	static CUIDrawer* __instance;

	

public:
	CUIDrawer();
	static CUIDrawer* GetInstance();

	void Draw(std::string, Vector2);
};