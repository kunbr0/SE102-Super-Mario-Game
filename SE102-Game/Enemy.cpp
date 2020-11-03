#include "Enemy.h"

CEnemy::CEnemy() {
	tag = ETag::ENEMY;
}

void CEnemy::Render(Vector2 finalPos) {
	CAnimations::GetInstance()->Get(GetRenderAnimationId(state.type))->Render(finalPos);
}