#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#define BUBBLE_IMG_PATH "asset/bubble.png"
#define BUBBLE_DEFAULT_IMG_PATH "asset/default.png"
#define  ARROW_IMG_PATH "asset/up-arrow.png"
const static bool BUBBLE_DEBUG = false;
const static int CLEAN_INTERVAL = 1500;


/*
*	bubble config
*/
const static float ANM_TIME = 0.8f;
const static int SHOOT_SPEED = 20;



/*
* shooting config
*/
const static int SHOOT_INTERVAL = 10;
const static int MAX_DIRECTION_ANGLE = 60;
const static float CHANGE_DIRECTION_SPEED = 1.0f;

/*
*  hexmap config
*/
const static int INIT_LAYER = 0;
const static int INIT_BUBBLE_SIZE = 60;
const static int INIT_MAP_WIDTH = 360;
const static float BORDER_COLOR[4] = { 0.4,0.4,0.4,1 };
const static int BG_COLOR[4] = { 255, 204, 153, 200 };
const unsigned int RESOURCE_BUBBLE_SIZE = 200;
const unsigned int NEIGHBOUR_NUMBER = 6;





#define SQRT_2  1.414f
#define SQRT_3  1.73205
#define PI 3.14159265

const static float LARGE_DISTANCE_TEST = 1.4f;
const static float LARGE_DISTANCE_TEST_ANGLE_COS = 0.9f;

const static float MEDIUM_DISTANCE_TEST = 0.7f;
const static float MEDIUM_DISTANCE_TEST_ANGLE_COS = 0.4f;

#endif // !CONFIG_H
