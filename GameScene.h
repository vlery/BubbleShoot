#ifndef  _GAME_SCENE_H
#define  _GAME_SCENE_H
#include "cocos2d.h"

#include "HexMap.h"
#include "BubbleLayer.h"
#include "ShootLayer.h"
#include "Config.h"
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