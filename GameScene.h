#ifndef  _GAME_SCENE_H
#define  _GAME_SCENE_H
#include "cocos2d.h"

#include "HexMap.h"
class GameScene : public cocos2d::Scene {
public :
	//static cocos2d::Scene* createGameScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
private :
	HexMap* hexmap;
};

#endif