#ifndef HEX_MAP_H
#define HEX_MAP_H
#include<list>
#include "BubbleNode.h"
#include "cocos2d.h"
#include "BubbleFactory.h"
#define OFFSET_UP_LEFT Vec2(-1, SQRT_3)
#define OFFSET_UP_RIGHT Vec2(1, SQRT_3)
#define OFFSET_BOTTOM_RIGHT Vec2(1, -SQRT_3)
#define OFFSET_BOTTOM_LEFT  Vec2(-1, -SQRT_3)
const static float SQRT_3 = 1.73205;



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