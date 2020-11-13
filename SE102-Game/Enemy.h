#pragma once
#include "GameObject.h"

enum class EEnemyState {
	ONESHOTDIE,
	DIE,
	WILL_DIE,
	LIVE,
};

struct SEnemyState {
	EEnemyState type = EEnemyState::LIVE;
	DWORD time = 0;
};


class CEnemy : public CGameObject
{
protected:
	SEnemyState state;
	Vector2 walkingScope;
	float walkingSpeed;
public:
	
	CEnemy();

	void InitWtandingScope(vector<LPCOLLISIONEVENT>*);

	virtual std::string GetRenderAnimationId(EEnemyState) { return ""; };
	virtual Vector2 GetBoundingBoxSize() { return Vector2(0, 0); }
	virtual void GetBoundingBox(float&, float&, float&, float&);

	virtual void Update(DWORD, vector<LPGAMEOBJECT>*);
	virtual void Render(Vector2);
	
	virtual void SetState(EEnemyState, DWORD = 0);
	virtual void ChangeState(EEnemyState newState) {};
	virtual EEnemyState GetState() { return state.type; };

	virtual void ChangeDirection();
	virtual void ChangeDirectionAfterAxisCollide();
	virtual void BeingCollided(EActionTag) override {};

	virtual void OnHasCollided(LPGAMEOBJECT) override;
};

