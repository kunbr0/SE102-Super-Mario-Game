#include "Animation.h"
#include "SpriteManager.h"

void CAnimation::Add(std::string spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

// NOTE: sometimes Animation object is NULL ??? HOW ??? 
void CAnimation::Render(float x, float y, int alpha, float scale, bool isReverse)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}
	if (scale == 1.0f && isReverse == false) {
		frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
	}
	else {
		frames[currentFrame]->GetSprite()->DrawWithScale(x, y, D3DXVECTOR2(1.0f*scale, 1.0f));
	}
	
}

CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(std::string id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(std::string id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}