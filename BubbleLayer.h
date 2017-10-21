#ifndef BUBBLE_LAYER_H
#define BUBBLE_LAYER_H
#include "cocos2d.h"
#include "HexMap.h"
#include <list>
#include "BubbleFactory.h"


class BubbleLayer : public cocos2d::Layer{

public:
	virtual bool init();
	virtual void update(float fDelta);
	CREATE_FUNC(BubbleLayer);

	void addBubbles();
	void setWidth(float width);
	void initBubbles(int bubbleSize, int layer);
	void addLayer();

	//test
	void setOriginSelect();
	void moveSelect(ConnectType type);
	
	
	
	void processShootBubble(BubbleNode* node);
	
	
	
	void checkThreeMatch(BubbleNode* node, BubbleNode* attachNode);
private:
	HexMap* hexmap;
	float width;
	float bubbleSize;
	void moveDown();
	BubbleNode* select=nullptr;
	std::list<BubbleNode*> shootList;
	void testAttach(BubbleNode* node);
	void attachBubble(BubbleNode* node, BubbleNode* attachNode);
	void cleanShootList();
	void generateAttachReplace(BubbleNode* node);
	
};
#endif // !BubbleLayer#pragma once
