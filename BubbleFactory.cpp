#include "BubbleFactory.h"
USING_NS_CC;
BubbleFactory* BubbleFactory::factory = nullptr;

BubbleNode* getFirstMatchBubble(std::list<BubbleNode*>* list, std::function<bool(BubbleNode*)> test) {
	
	auto itr = list->begin();
	while (itr != list->end()) {
		auto bb = *itr;
		if (test(bb)) {
			list->erase(itr);
			return bb;
		}
		++itr;
	}
	return nullptr;

}


void BubbleFactory::addInList(BubbleNode* node) {
	switch (node->getType()) {
	case BubbleType::Boundry_Top:
		topBoundries.push_back(node); return;
	case BubbleType::Boundry_Attach:
		potentialAttachPositions.push_back(node);
		return;
	default:
		bubbles.push_back(node);
		return;
	}
}

void BubbleFactory::recycle() {
	auto itr = bubbles.begin();

	while(itr != bubbles.end()){
		if ((*itr)->isDead()) {
			(*itr)->reset();
			pool.push_back(*itr);
			itr=bubbles.erase(itr);
		}else{
			++itr;
		}
	}

	itr = potentialAttachPositions.begin();
	while(itr != potentialAttachPositions.end()) {
		if ((*itr)->isDead()) {
			(*itr)->reset();
			pool.push_back(*itr);
			itr = potentialAttachPositions.erase(itr);
		}
		else {
			++itr;
		}
	}

}

void BubbleFactory::recycle(BubbleNode* node) {
	node->reset();
	pool.push_back(node);
	if (node->isBubble()) {

		auto find_itr = std::find(bubbles.begin(),bubbles.end(), node);
		if (find_itr != bubbles.end()) {
			bubbles.erase(find_itr);
			return;
		}
		return;
	}
	if (node->isPotentialAttach()) {
		auto find_itr = std::find(potentialAttachPositions.begin(), potentialAttachPositions.end(), node);
		if (find_itr != potentialAttachPositions.end()) {
			potentialAttachPositions.erase(find_itr);
			return;
		}
		return;
	}
	if (node->isTopBoundry()) {
		auto find_itr = std::find(topBoundries.begin(), topBoundries.end(), node);
		if (find_itr != topBoundries.end()) {
			topBoundries.erase(find_itr);
			return;
		}
		return;
	}

}
BubbleNode* BubbleFactory::generateBubble(BubbleType type, cocos2d::Point position, cocos2d::Size size) {
	if (pool.size() == 0) {
		if (BUBBLE_DEBUG) {	
			CCLOG("Total Bubbles:%d",pool.size() + topBoundries.size()+potentialAttachPositions.size()+bubbles.size());
		}
		return new BubbleNode(type, position, size);
	}
	else {	
		auto find_bb = getFirstMatchBubble(&pool, [type](BubbleNode* node) {
			return (node->getType() == type);
		});
		if (find_bb != nullptr) {
			find_bb->setPositions(position);
			return find_bb;
		}
		
		return new BubbleNode(type, position, size);
	}


}

void BubbleFactory::topListPopFront() {
	topBoundries.erase(topBoundries.begin());
}