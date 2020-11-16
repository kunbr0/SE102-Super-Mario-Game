#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "FireBullet.h"

enum class EEnemyState {
	ONESHOTDIE,
	DIE,
	WILL_DIE,
	BEING_KICKED,
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
	virtual Vector2 GetBoundingBoxSize() { return GetBoundingBoxSize(this->state.type); }
	virtual Vector2 GetBoundingBoxSize(EEnemyState) { return Vector2(0, 0); }

	virtual void GetBoundingBox(float&, float&, float&, float&);

	virtual void Update(DWORD, vector<LPGAMEOBJECT>*);
	virtual void Render(Vector2);
	
	virtual float GetDefaultWalkingSpeed() { return 0.0f; }

	virtual void SetState(EEnemyState, DWORD = 0);
	virtual void ChangeState(EEnemyState newState);
	virtual EEnemyState GetState() { return state.type; };

	virtual void BeingKicked(Vector2 pos);


	virtual void ChangeDirection();
	virtual void ChangeDirectionAfterAxisCollide();

	virtual void BeingCollidedTop(LPGAMEOBJECT) override;

	virtual void OnHadCollided(LPGAMEOBJECT) override;

	virtual void KillMario(CMario* );
};

