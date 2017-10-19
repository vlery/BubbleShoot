#include "Bulk.h"
#include "BubbleNode.h"


Bulk::Bulk() :connection(0), nodeNum(0){};

Bulk::Bulk(BubbleType type) {
	connection = 0;
	nodeNum = 0;
	this->type = type;
}
void Bulk::attach(BubbleNode* bubble) {
	bubbles.push_back(bubble);
	nodeNum++;
}
void Bulk::detach(BubbleNode* bubble) {
	for (auto it = bubbles.begin(); it != bubbles.end(); ) {
		if ((*it) == bubble) {
			it = bubbles.erase(it);
			break;
		}
		else {
			++it;
		}
	}
	nodeNum--;
}
void Bulk::destoryBubbles() {
	for (auto it = bubbles.begin(); it != bubbles.end(); )
	{
		(*it)->destorySelf();
		++it;
	}

}

bool Bulk::ifHasBubble(BubbleNode* bubble) {
	for (auto it = bubbles.begin(); it != bubbles.end(); ) {
		if ((*it) == bubble) {
			return true;
		}
		++it;
	}
	return false;
}


void  Bulk::select() {
	auto start = bubbles.begin();
	auto end = bubbles.end();
	while (start != end) {
		(*start)->select();
		++start;
	}

	
}
void Bulk::unselect() {
	auto start = bubbles.begin();
	auto end = bubbles.end();
	while (start != end) {
		(*start)->unselect();
		++start;
	}
}
void Bulk::detachAll() {
	auto start = bubbles.begin();
	auto end = bubbles.end();
	while (start != end) {
		(*start)->setBulk(nullptr);
		++start;
	}
}
void Bulk::absorb(Bulk* bulk) {
	connection += bulk->connection;
	nodeNum += bulk->nodeNum;
	auto itr = bulk->bubbles.begin();
	auto end = bulk->bubbles.end();
	while (itr != end) {
		(*itr)->setBulk(this);
		++ itr;
	}
	this->bubbles.merge(bulk->bubbles);
	delete bulk;
}



BubbleNode* Bulk::getFirstConnectOuterNode() {
	connectOutItr = bubbles.begin();
	while (connectOutItr != bubbles.end()) {
		BubbleNode* node = (*connectOutItr);
		for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
			auto neighbour = node->connect[(int)node->connectType[i]];
			if (neighbour != nullptr) {
				if (neighbour->isBubble() || neighbour->getType() == BubbleType::Boundry_Top) {
					return node;
				}
			}
		}
		++connectOutItr;
	}
	return nullptr;
}
bool Bulk::ifHasNextConnectOuterNode() {
	++connectOutItr;
	while (connectOutItr != bubbles.end()) {
		
		BubbleNode* node = (*connectOutItr);
		for (int i = 0; i < NEIGHBOUR_NUMBER; i++) {
			auto neighbour = node->connect[(int)node->connectType[i]];
			if (neighbour != nullptr) {
				if (neighbour->isBubble() || neighbour->getType() == BubbleType::Boundry_Top) {
					return true;
				}
			}
		}
		++connectOutItr;
	}
	return false;
}
BubbleNode* Bulk::getNextConnectOuterNode() {
	return *connectOutItr;
}
