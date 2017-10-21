#pragma once
#ifndef CONFIG_H
#define CONFIG_H
/*
*	bubble config
*/
const static float ANM_TIME = 0.8f;
const static int SHOOT_SPEED = 20;


/*
*  hexmap config
*/
const static int INIT_LAYER = 1;
const static int INIT_BUBBLE_SIZE = 60;
const static int INIT_MAP_WIDTH = 360;
const static float BORDER_COLOR[4] = { 0.4,0.4,0.4,1 };
const static int BG_COLOR[4] = { 255, 204, 153, 200 };
const unsigned int RESOURCE_BUBBLE_SIZE = 200;
const unsigned int NEIGHBOUR_NUMBER = 6;
const static bool BUBBLE_DEBUG = false;




#define SQRT_2  1.414f
#define SQRT_3  1.73205
#define PI 3.14159265
#define OFFSET_UP_LEFT Vec2(-1, SQRT_3)
#define OFFSET_UP_RIGHT Vec2(1, SQRT_3)
#define OFFSET_BOTTOM_RIGHT Vec2(1, -SQRT_3)
#define OFFSET_BOTTOM_LEFT  Vec2(-1, -SQRT_3)


const static float LARGE_DISTANCE_TEST = SQRT_3;
const static float LARGE_DISTANCE_TEST_ANGLE_COS = 0.9f;

const static float SMALL_DISTANCE_TEST = 0.66f;
const static float SMALL_DISTANCE_TEST_ANGLE_COS = 0.4f;

#endif // !CONFIG_H
