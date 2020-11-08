#pragma once
#include "GameObject.h"

enum class EEnemyState {
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
public:
	CEnemy();
	virtual std::string GetRenderAnimationId(EEnemyState) { return ""; };

	virtual void Render(Vector2);
	
	virtual void SetState(EEnemyState state) {};
	virtual void ChangeState(EEnemyState newState) {};
	virtual EEnemyState GetState() { return state.type; };

	
	
	virtual void BeingCollided(ETag) {};

};

