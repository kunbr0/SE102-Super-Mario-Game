#include "CameraLimitController.h"

CCameraLimitController::CCameraLimitController(Vector2 pos) : CGameObject(pos) {
	x = 0;
	y = 800;
}


void CCameraLimitController::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	x += 0.1 * dt;
	DebugOut(ToWSTR(std::to_string(x) + "\n").c_str());
}