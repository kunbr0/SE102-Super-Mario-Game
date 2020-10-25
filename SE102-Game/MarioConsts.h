#pragma once



#define MARIO_STATE_IDLE			0
#define MARIO_STATE_NONE_PRESS_KEY	1
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_PRESS_A			111

#define MARIO_STATE_JUMP_X			300
#define MARIO_STATE_JUMP_S			301
#define MARIO_STATE_RACCOON_FLY		302

#define MARIO_STATE_DIE				400

// STATUS
#define STATUS_IS_IDLING_IN_SOMETHING		0
#define STATUS_IS_WALKING					1
#define STATUS_IS_JUMPING					2
#define STATUS_IS_FALLING					3


// RED MARIO
//// Level = 1
#define MARIO_ANI_SMALL_IDLE			"ani-small-mario-idle"
#define MARIO_ANI_SMALL_WALK			"ani-small-mario-walk"
#define MARIO_ANI_SMALL_JUMP			"ani-small-mario-jump"
#define MARIO_ANI_SMALL_FALL			"ani-small-mario-fall"
#define MARIO_ANI_SMALL_SKID			"ani-small-mario-skid"


//// Level = 2
#define MARIO_ANI_BIG_IDLE				"ani-big-mario-idle"
#define MARIO_ANI_BIG_WALK				"ani-big-mario-walk"
#define MARIO_ANI_BIG_JUMP				"ani-big-mario-jump"
#define MARIO_ANI_BIG_FALL				"ani-big-mario-fall"
#define MARIO_ANI_BIG_SKID				"ani-big-mario-skid"


//// Level = 3
#define RACCOON_MARIO_ANI_BIG_IDLE		"ani-raccoon-mario-idle"
#define RACCOON_MARIO_ANI_BIG_WALK		"ani-raccoon-mario-walk"
#define RACCOON_MARIO_ANI_BIG_JUMP		"ani-raccoon-mario-jump"
#define RACCOON_MARIO_ANI_BIG_ATTACK	"ani-raccoon-mario-spin"
#define RACCOON_MARIO_ANI_BIG_FLY	    "ani-raccoon-mario-fly"
#define RACCOON_MARIO_ANI_BIG_SKID		"ani-raccoon-mario-skid"


// FIRE MARIO
//// Level = 1
#define FIRE_MARIO_ANI_IDLE				"ani-fire-mario-idle"
#define FIRE_MARIO_ANI_WALK				"ani-fire-mario-walk"
#define FIRE_MARIO_ANI_JUMP				"ani-fire-mario-jump"

// HAMMER MARIO
//// Level = 1
#define HAMMER_MARIO_ANI_IDLE			"ani-hammer-mario-idle-side"
#define HAMMER_MARIO_ANI_WALK			"ani-hammer-mario-walk"
#define HAMMER_MARIO_ANI_JUMP			"ani-hammer-mario-jump"


// FROG MARIO
//// Level = 1
#define FROG_MARIO_ANI_IDLE			"ani-frog-mario-idle"
#define FROG_MARIO_ANI_WALK			"ani-frog-mario-walk"
#define FROG_MARIO_ANI_JUMP			"ani-frog-mario-jump"





#define MARIO_ANI_DIE				8

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACCOON	3


#define MARIO_SMALL_BBOX_WIDTH  39
#define MARIO_SMALL_BBOX_HEIGHT 45

#define MARIO_BIG_BBOX_WIDTH  45
#define MARIO_BIG_BBOX_HEIGHT 81

#define RACCOON_MARIO_BIG_BBOX_WIDTH  62
#define RACCOON_MARIO_BIG_BBOX_HEIGHT 81

#define FIRE_MARIO_BBOX_WIDTH  45
#define FIRE_MARIO_BBOX_HEIGHT 81

#define HAMMER_MARIO_BBOX_WIDTH  45
#define HAMMER_MARIO_BBOX_HEIGHT 81

#define FROG_MARIO_BBOX_WIDTH  45
#define FROG_MARIO_BBOX_HEIGHT 91
#define FROG_MARIO_BBOX_WIDTH_LIE  45
#define FROG_MARIO_BBOX_HEIGHT_LIE 81


#define MARIO_UNTOUCHABLE_TIME 5000
