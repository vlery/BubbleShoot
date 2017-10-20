#ifndef  _GAME_SCENE_H
#define  _GAME_SCENE_H
#include "cocos2d.h"

#include "HexMap.h"
#include "BubbleLayer.h"
#include "ShootLayer.h"

static int INIT_LAYER=1;
static int INIT_BUBBLE_SIZE = 60;
static int INIT_MAP_WIDTH = 360;
static float BORDER_COLOR[4] = { 0.4,0.4,0.4,1 };
static int BG_COLOR[4] = { 255, 204, 153, 200 };
class GameScene : public cocos2d::Scene {
public :
	//static cocos2d::Scene* createGameScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	
private :
	BubbleLayer* bubbleLayer;
	ShootLayer* shootLayer;
	void initKeyboardListener();
	void initLayout();
	void shootBubble();
	cocos2d::Point correctPosCrt2BB(cocos2d::Point position);
};

#endif