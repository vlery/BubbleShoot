#ifndef BUBBLE_FACTORY_H
#define BUBBLE_FACTORY_H
#include "BubbleNode.h"
#include "cocos2d.h"
#include <list>
#include <algorithm>
enum class ListType {
	TOP,BUBBLE,ATTACH,POOL
};
BubbleNode* getFirstMatchBubble(std::list<BubbleNode*>* list, std::function<bool(BubbleNode*)> test);


class BubbleFactory {
public:
	static BubbleFactory& getFactory() {
		if (factory == nullptr) {
			factory = new BubbleFactory();
		}
		return *factory;
	}
	void addInList(BubbleNode* node);
	std::list<BubbleNode*>getBubblesList() {
		return bubbles;
	}
	std::list<BubbleNode*> getTopList() {
		return topBoundries;
	}
	std::list<BubbleNode*> getAttachList() {
		return potentialAttachPositions;
	}
	BubbleNode* generateBubble(BubbleType type, cocos2d::Point position, cocos2d::Size size);
	void recycle();
	void recycle(BubbleNode* node);
	~BubbleFactory() {};
	void topListPopFront();

private:
	BubbleFactory() {};
	std::list<BubbleNode*> topBoundries;
	std::list<BubbleNode*> bubbles;
	std::list<BubbleNode*> potentialAttachPositions;
	std::list<BubbleNode*> pool;
	BubbleFactory(BubbleFactory const&) {};
	void operator=(const BubbleFactory&) {};
	static BubbleFactory* factory;

};
#endif // !BUBBLE_FACTORY_H#pragma once
