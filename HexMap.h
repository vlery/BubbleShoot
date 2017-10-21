#ifndef HEX_MAP_H
#define HEX_MAP_H
#include<list>
#include "BubbleNode.h"
#include "cocos2d.h"
#include "BubbleFactory.h"
#include "Config.h"


bool isSamePos(cocos2d::Point p1, cocos2d::Point p2);
class HexMap {



public:
	BubbleType bubbletype;
	HexMap(float size, float width,int layer);
	void generateRow(std::function<void()> moveDown);
	BubbleNode* generateAttachReplace(BubbleNode* replace);
	void generateAttachNodeAround(BubbleNode* node);
private:  
	float size;
	float width;
	int layer;
	bool ifOffset;
	void initBoundry();
	void generateBubbleLayer();

	BubbleNode* createBubbleToList(BubbleType type,cocos2d::Point pos);
	cocos2d::Vec2 getOffsetByType(ConnectType type);

	
};
#endif // !HEX_MAP_H