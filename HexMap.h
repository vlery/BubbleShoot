#ifndef HEX_MAP_H
#define HEX_MAP_H
#include<list>
#include "BubbleNode.h"

const static float SQRT_3 = 1.73205;
class HexMap {



public:

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

};
#endif // !HEX_MAP_H