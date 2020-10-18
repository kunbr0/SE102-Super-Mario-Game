#pragma once
#include "AnimationFrame.h"
#include "Utils.h"
#include "Transform.h"

class CAnimation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(std::string spriteId, DWORD time = 0);

	void Render(Vector2 finalPos, int alpha = 255);

};

typedef CAnimation* LPANIMATION;

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<std::string, LPANIMATION> animations;

public:
	void Add(std::string id, LPANIMATION ani);
	LPANIMATION Get(std::string id);
	void Clear();

	static CAnimations* GetInstance();
};

typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;