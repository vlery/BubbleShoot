#ifndef SHOOT_LAYER_H
#define SHOOT_LAYER_H

#include "Config.h"
#include "cocos2d.h"
#include "BubbleNode.h"
#include "BubbleFactory.h"

class ShootLayer: public cocos2d::Layer {

public:
	virtual bool init();
	void initLayout(float bubbleSize);
	CREATE_FUNC(ShootLayer);
	void changeDirection(bool ifClockwise);
	void loadBubble();
	void resetControlPanel();
	BubbleNode* shoot();

	void abandon();
private:
		cocos2d::Layer* controlPanel;
		BubbleNode* currentBubble=nullptr;
		BubbleNode* shootBubble = nullptr;
		float bubbleSize;
		

};
#endif // !SHOOT_LAYER_H#pragma once
