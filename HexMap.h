#ifndef HEX_MAP_H
#define HEX_MAP_H
#include<list>
#include "BubbleNode.h"
#include "cocos2d.h"

#define OFFSET_UP_LEFT Vec2(-1, SQRT_3)
#define OFFSET_UP_RIGHT Vec2(1, SQRT_3)
#define OFFSET_BOTTOM_RIGHT Vec2(1, -SQRT_3)
#define OFFSET_BOTTOM_LEFT  Vec2(-1, -SQRT_3)
const static float SQRT_3 = 1.73205;



class HexMap {



public:
	BubbleType bubbletype;
	HexMap(float size, float width,int layer);
	std::list<BubbleNode*>getBubblesList() {
		return bubbles;
	}
	std::list<BubbleNode*> getTopList() {
		return topBoundries;
	}
	std::list<BubbleNode*> getAttachList() {
		return potentialAttachPositions;
	}
	void generateRow(std::function<void()> moveDown);

private:  
	float size;
	float width;
	int layer;
	bool ifOffset;
	void initBoundry();
	void generateLayer();
	std::list<BubbleNode*> topBoundries;
	std::list<BubbleNode*> bubbles;
	std::list<BubbleNode*> potentialAttachPositions;
	void addInList(BubbleNode* node);
	

	
};
#endif // !HEX_MAP_H