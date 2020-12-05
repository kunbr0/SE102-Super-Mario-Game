#pragma once
// ACCELERATION
#define ACCELERATION_Y_GRAVITY					0.0019f

#define ACCELERATION_X_WALK						0.000575f
#define ACCELERATION_X_RUN_GREATER_RATIO		0.3f // Run greater than Walk

#define ACCELERATION_FRICTION					0.0002925f

// LIMIT
#define VELOCITY_X_MAX_WALK						0.24f
#define VELOCITY_X_MAX_RUN						0.42f
#define VELOCITY_X_MAX_SPEEDUP					0.45f
#define VELOCITY_X_MIN_FOR_SKID					0.12f
#define VELOCITY_X_MIN_FOR_RUN					0.25f
#define DISTANCE_Y_MIN_FOR_FALL					7.0f


// VELOCITY
#define VELOCITY_X_AFTER_SKID			0.08f
#define VELOCITY_Y_FALL_SLIGHTLY		0.08f
#define VELOCITY_Y_AFTER_COLLIDE_TOP_ENEMY		0.58f
#define VELOCITY_X_BEING_BOUNCED				0.32f

#define MARIO_JUMP_SPEED_Y				0.62f


// RATIO
#define MARIO_WALKING_BOOST_RATE		0.7f
//0.1f
#define MARIO_FLY_SPEED_Y				0.35f

#define MARIO_JUMP_DEFLECT_SPEED		0.2f
#define MARIO_DIE_DEFLECT_SPEED			0.5f

// POWER X
#define POWER_X_MIN_FOR_SPEEDUP			6000
#define POWER_X_LOSE_IN_GROUND			100
#define POWER_X_LOSE_ALWAYS				10
#define POWER_X_RATIO_GAIN				15


#define POWER_X_WHEN_GETTING_SPEEDUP	9000


