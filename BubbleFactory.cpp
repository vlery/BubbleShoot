#include "BubbleFactory.h"
BubbleFactory* BubbleFactory::factory = nullptr;
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

	if (itr != bubbles.end()){
		if ((*itr)->getState() == BubbleState::DEAD) {
			(*itr)->reset();
			pool.push_back(*itr);
			itr=bubbles.erase(itr);

		}else{
			++itr;
		}
	}

	itr = potentialAttachPositions.begin();
	if (itr != potentialAttachPositions.end()) {
		if ((*itr)->getState() == BubbleState::DEAD) {
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
	if (node->isBubble()) {
		auto itr = bubbles.begin();
		while (itr != bubbles.end()) {
			if (*itr == node) {
				pool.push_back(*itr);
				bubbles.erase(itr);
				return;
			}
			++itr;
		}
		return;
	}
	if (node->getType()==BubbleType::Boundry_Attach) {
		auto itr = potentialAttachPositions.begin();
		while (itr != potentialAttachPositions.end()) {
			if (*itr == node) {
				pool.push_back(*itr);
				potentialAttachPositions.erase(itr);
				return;
			}
			++itr;
		}
		return;
	}
	if (node->getType() == BubbleType::Boundry_Top) {
		auto itr = topBoundries.begin();
		while (itr != topBoundries.end()) {
			if (*itr == node) {
				pool.push_back(*itr);
				topBoundries.erase(itr);
				return;
			}
			++itr;
		}
		return;
	}



}
BubbleNode* BubbleFactory::generateBubble(BubbleType type, cocos2d::Point position, cocos2d::Size size) {
	if (pool.size() == 0) {
		return new BubbleNode(type, position, size);
	}
	else {
		auto itr = pool.begin();
		BubbleNode* node;
		while (itr != pool.end()) {
			if ((*itr)->getType() == type) {
				node = *(itr);
				pool.erase(itr);
				node->setPositions(position);
				//node->getBubble()->setContentSize(size);
				return node;
			}
		}
		return new BubbleNode(type, position, size);
	}


}

void BubbleFactory::topListPopFront() {
	topBoundries.erase(topBoundries.begin());
}