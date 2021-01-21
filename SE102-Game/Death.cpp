#include "Death.h"
#include "Mario.h"

CDeath::CDeath(float x, float y, float width, float height) : CRectCollision(x, y, width, height) {

}

void CDeath::BeingCollided(LPGAMEOBJECT obj) {
	if (obj == NULL) return;
	if (dynamic_cast<CMario*>(obj)) {
		((CMario*)obj)->x -= 19;
		
		((CMario*)obj)->SetAction(MarioAction::DIE,500);
		

	}
	else {
		obj->x = obj->y = 0;
		obj->isDisable = true;
	}
}

