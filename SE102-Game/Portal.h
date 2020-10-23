#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	std::string scene_id;	// target scene to switch to 

	int width;
	int height;
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	virtual void Render(Vector2 finalPos);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	std::string GetSceneId() { return scene_id; }
};