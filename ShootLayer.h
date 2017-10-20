#ifndef SHOOT_LAYER_H
#define SHOOT_LAYER_H
#define PI 3.14159265

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
