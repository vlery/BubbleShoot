
#ifndef BULK_H
#define BULK_H
#include "BubbleNode.h"
#include<algorithm>
#include <list>

void combineUnboundBulkAround(Bulk* bulk);

class Bulk {
public:

	Bulk();
	Bulk(BubbleType type);

	void attach(BubbleNode* bubble);
	void detach(BubbleNode* bubble);
	void detachAll();
	void destory();
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
	void addConnectBulk(Bulk* bulk);

	void removeConnectBulk(Bulk* bulk);
	void replaceConnectBulk(Bulk* from, Bulk* to);
	std::list<Bulk*> getConnectBulkList() ;
	void removeAllConnection();
	BubbleType getType() { return type; }
	void connectToTop() {++connectTopCount;}
	void disConnectToTop() { --connectTopCount; }
	bool ifConnectTop() { return (connectTopCount>0); }

private:
	
	std::list<BubbleNode*> bubbles;
	 int connectTopCount;
	BubbleType type;
	int nodeNum;
	std::list<BubbleNode*>::iterator connectOutItr;
	std::list<Bulk*> connectBulkList;
};




#endif // !BULK_H#pragma once
