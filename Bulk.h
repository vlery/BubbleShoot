
#ifndef BULK_H
#define BULK_H
#include "BubbleNode.h"
class Bulk {
public:

	Bulk();
	Bulk(BubbleType type);
	void addConnection() {
		++connection;
	}
	void removeConnection() {
		--connection;
	}


	void attach(BubbleNode* bubble);
	void detach(BubbleNode* bubble);
	void detachAll();
	void destoryBubbles();
	bool ifHasBubble(BubbleNode* bubble);

	void select();
	void unselect();
	void absorb(Bulk* bulk);
	int getNodeNum() {
		return nodeNum;
	}
private:
	std::list<BubbleNode*> bubbles;
	BubbleType type;
	int connection;
	int nodeNum;
};




#endif // !BULK_H#pragma once
