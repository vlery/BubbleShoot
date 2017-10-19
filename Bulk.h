
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

	int getConnection() { return connection; }
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
	std::list<BubbleNode*> getAttachedBubble() {
		return bubbles;
	}

	BubbleNode* getFirstConnectOuterNode();
	bool ifHasNextConnectOuterNode();
	BubbleNode* getNextConnectOuterNode();

private:
	std::list<BubbleNode*> bubbles;
	BubbleType type;
	int connection;
	int nodeNum;
	std::list<BubbleNode*>::iterator connectOutItr;
};




#endif // !BULK_H#pragma once
